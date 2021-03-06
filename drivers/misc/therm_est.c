/*
 * drivers/misc/therm_est.c
 *
 * Copyright (C) 2010-2012 NVIDIA Corporation.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/kernel.h>
#include <linux/cpufreq.h>
#include <linux/delay.h>
#include <linux/mutex.h>
#include <linux/init.h>
#include <linux/err.h>
#include <linux/clk.h>
#include <linux/debugfs.h>
#include <linux/seq_file.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/syscalls.h>
#include <linux/therm_est.h>

#ifdef CONFIG_MACH_X3
static long bcoeff[] = {-38, -11, -3, -2, -2, -1, -1, -1, 0, 11, -13, 6, 7, 5, 6, 6, 6, 6, 13, 55};
static long bhist[HIST_LEN] ={250,250,250,250,250,250,250,250,250,250,250,250,250,250,250,250,250,250,250,250};

static long cur_skin_temp1=30000;
extern int nct1008_is_disabled(void);
extern int batt_Temp_C;
#endif

int therm_est_get_temp(struct therm_estimator *est, long *temp)
{
	*temp = est->cur_temp;
	return 0;
}

int therm_est_set_limits(struct therm_estimator *est,
				long lo_limit,
				long hi_limit)
{
	est->therm_est_lo_limit = lo_limit;
	est->therm_est_hi_limit = hi_limit;
	return 0;
}

int therm_est_set_alert(struct therm_estimator *est,
			void (*cb)(void *),
			void *cb_data)
{
	if ((!cb) || est->callback)
		BUG();

	est->callback = cb;
	est->callback_data = cb_data;

	return 0;
}

#ifdef CONFIG_MACH_X3
static int btemp_get(long *btemp)
{
#if 0
        char buf[64];
        int fd, ret, count;
        mm_segment_t old_fs;

        /* Hack: should get battery temp from somewhere else */
        old_fs = get_fs();
        set_fs(KERNEL_DS);
        fd = sys_open("/sys/class/power_supply/battery/temp", O_RDONLY, 0644);

        if (fd >= 0) {
                count = sys_read(fd, buf, 64);
                if (count < 64)
                        buf[count] = '\0';
                ret = kstrtol(buf, 10, btemp);
                sys_close(fd);
        } else {
                ret = -1;
        }
        set_fs(old_fs);
		return ret;
#else
	if (batt_Temp_C!=0x10000) {
		*btemp = batt_Temp_C;
		return 0;
	}
	else
		return -1;
#endif
}
#endif

static void therm_est_work_func(struct work_struct *work)
{
	int i, j, index, sum = 0;
#ifdef CONFIG_MACH_X3
	int bsum = 0;
#endif
	long temp;
	struct delayed_work *dwork = container_of (work,
					struct delayed_work, work);
	struct therm_estimator *est = container_of(
					dwork,
					struct therm_estimator,
					therm_est_work);

#ifdef CONFIG_MACH_X3
	if (nct1008_is_disabled() == 0) {
#endif
	for (i = 0; i < est->ndevs; i++) {
		if (est->devs[i]->get_temp(est->devs[i]->dev_data, &temp))
			continue;
		est->devs[i]->hist[(est->ntemp % HIST_LEN)] = temp;
	}

	for (i = 0; i < est->ndevs; i++) {
		for (j = 0; j < HIST_LEN; j++) {
			index = (est->ntemp - j + HIST_LEN) % HIST_LEN;
			sum += est->devs[i]->hist[index] *
				est->devs[i]->coeffs[j];
		}
	}

#ifdef CONFIG_MACH_X3
	if (btemp_get(&bhist[est->ntemp % HIST_LEN])) {
		pr_err("%s:error to get battery temp\n", __func__);
		goto get_temp_err;
	}

	for (j = 0; j < HIST_LEN; j++) {
		index = (est->ntemp - j + HIST_LEN) % HIST_LEN;
		bsum += bhist[index] * bcoeff[j];
	}
	est->cur_temp = sum / 100 + est->toffset + bsum;

	cur_skin_temp1=est->cur_temp;
#else
	est->cur_temp = sum / 100 + est->toffset;
#endif

	est->ntemp++;

#ifdef CONFIG_MACH_X3
get_temp_err:
#endif
	if (est->callback && ((est->cur_temp >= est->therm_est_hi_limit) ||
			 (est->cur_temp <= est->therm_est_lo_limit)))
		est->callback(est->callback_data);
#ifdef CONFIG_MACH_X3
	} // nct1008_is_disabled() == 0
#endif
	queue_delayed_work(est->workqueue, &est->therm_est_work,
				msecs_to_jiffies(est->polling_period));
}

#ifdef CONFIG_MACH_X3
long tegra_get_cur_skin_temp(void)
{
        return cur_skin_temp1;
}
#endif

struct therm_estimator *therm_est_register(
			struct therm_est_subdevice **devs,
			int ndevs,
			long toffset,
			long polling_period)
{
	int i, j;
	long temp;
	struct therm_estimator *est;
	struct therm_est_subdevice *dev;

	est = kzalloc(sizeof(struct therm_estimator), GFP_KERNEL);
	if (IS_ERR_OR_NULL(est))
		return ERR_PTR(-ENOMEM);

	est->devs = devs;
	est->ndevs = ndevs;
	est->toffset = toffset;
	est->polling_period = polling_period;

	/* initialize history */
	for (i = 0; i < ndevs; i++) {
		dev = est->devs[i];

		if (dev->get_temp(dev->dev_data, &temp)) {
			kfree(est);
			return ERR_PTR(-EINVAL);
		}

		for (j = 0; j < HIST_LEN; j++) {
			dev->hist[j] = temp;
		}
	}

#ifdef CONFIG_MAGH_X3
	btemp_get(&temp);
	for (j = 0; j < HIST_LEN; j++) {
		bhist[j] = temp;
	}
#endif

	est->workqueue = alloc_workqueue("therm_est",
				    WQ_HIGHPRI | WQ_UNBOUND | WQ_RESCUER, 1);
	INIT_DELAYED_WORK(&est->therm_est_work, therm_est_work_func);

	queue_delayed_work(est->workqueue,
				&est->therm_est_work,
				msecs_to_jiffies(est->polling_period));

	return est;
}
