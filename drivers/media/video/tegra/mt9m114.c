/*
<<<<<<< HEAD
 * Copyright (c) 2013, NVIDIA CORPORATION.  All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <linux/kernel.h>
=======
 * mt9m114.c - mt9m114 sensor driver
 *
 * Copyright (c) 2011, NVIDIA, All Rights Reserved.
 *
 * Contributors:
 *      hyeongjin.kim<hyeongjin.kim@lge.com>
 *
 * Leverage mt9m114.c
 *
 * This file is licensed under the terms of the GNU General Public License
 * version 2. This program is licensed "as is" without any warranty of any
 * kind, whether express or implied.
 */

/**
 * SetMode Sequence for 640x480. Phase 0. Sensor Dependent.
 * This sequence should put sensor in streaming mode for 640x480
 * This is usually given by the FAE or the sensor vendor.
 */

>>>>>>> f720e99... drivers/: Merge LGE changes [2]
#include <linux/delay.h>
#include <linux/fs.h>
#include <linux/i2c.h>
#include <linux/miscdevice.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
<<<<<<< HEAD
#include <linux/gpio.h>
#include <linux/debugfs.h>
#include <linux/seq_file.h>
#include <linux/atomic.h>
#include <linux/regulator/consumer.h>
#include <linux/export.h>
#include <linux/module.h>

#include <media/mt9m114.h>

#define SIZEOF_I2C_TRANSBUF 128

struct mt9m114_reg {
	u16 cmd; /* command */
=======
#include <media/mt9m114.h>
#include <linux/gpio.h>
#if 1 //defined(CONFIG_REGULATOR_CAM_SUBPMIC_LP8720)
#include <../../../include/linux/regulator/lp8720.h>
extern void subpm_set_output(subpm_output_enum outnum, int onoff);
extern void subpm_set_gpio(int onoff);
#endif
//                                                                           
#if 1
static DEFINE_MUTEX(mt9m114_camera_lock);
typedef struct ExposureValueTypeRec{
    int index;
    int range;
}ExposureValueType;

typedef struct FpsRangeTypeRec{
    int low;
    int high;
}FpsRangeType;
#endif
//                                                                           
struct mt9m114_reg {
>>>>>>> f720e99... drivers/: Merge LGE changes [2]
	u16 addr;
	u16 val;
};

<<<<<<< HEAD
static struct mt9m114_reg mode_1280x960_30fps[] = {
	{MT9M114_SENSOR_WORD_WRITE, 0x001A, 0x0001},
	{MT9M114_SENSOR_WAIT_MS, 0, 10},
	{MT9M114_SENSOR_WORD_WRITE, 0x001A, 0x0000},
	{MT9M114_SENSOR_WAIT_MS, 0, 50},
	{MT9M114_SENSOR_WORD_WRITE, 0x301A, 0x0234},

	{MT9M114_SENSOR_WORD_WRITE, 0x098E, 0x1000},
	{MT9M114_SENSOR_BYTE_WRITE, 0xC97E, 0x01},
	{MT9M114_SENSOR_WORD_WRITE, 0xC980, 0x0120},
	{MT9M114_SENSOR_WORD_WRITE, 0xC982, 0x0700},

	{MT9M114_SENSOR_WORD_WRITE, 0xC800, 0x0004},
	{MT9M114_SENSOR_WORD_WRITE, 0xC802, 0x0004},
	{MT9M114_SENSOR_WORD_WRITE, 0xC804, 0x03CB},
	{MT9M114_SENSOR_WORD_WRITE, 0xC806, 0x050B},
	{MT9M114_SENSOR_WORD_WRITE, 0xC808, 0x02DC},
	{MT9M114_SENSOR_WORD_WRITE, 0xC80A, 0x6C00},
	{MT9M114_SENSOR_WORD_WRITE, 0xC80C, 0x0001},
	{MT9M114_SENSOR_WORD_WRITE, 0xC80E, 0x00DB},
	{MT9M114_SENSOR_WORD_WRITE, 0xC810, 0x05B3},
	{MT9M114_SENSOR_WORD_WRITE, 0xC812, 0x03EE},
	{MT9M114_SENSOR_WORD_WRITE, 0xC814, 0x0636},
	{MT9M114_SENSOR_WORD_WRITE, 0xC816, 0x0060},
	{MT9M114_SENSOR_WORD_WRITE, 0xC818, 0x03C3},
	{MT9M114_SENSOR_WORD_WRITE, 0xC834, 0x0000},
	{MT9M114_SENSOR_WORD_WRITE, 0xC854, 0x0000},
	{MT9M114_SENSOR_WORD_WRITE, 0xC856, 0x0000},
	{MT9M114_SENSOR_WORD_WRITE, 0xC858, 0x0500},
	{MT9M114_SENSOR_WORD_WRITE, 0xC85A, 0x03C0},
	{MT9M114_SENSOR_BYTE_WRITE, 0xC85C, 0x03},
	{MT9M114_SENSOR_WORD_WRITE, 0xC868, 0x0500},
	{MT9M114_SENSOR_WORD_WRITE, 0xC86A, 0x03C0},
	{MT9M114_SENSOR_WORD_WRITE, 0xC88C, 0x1E02},
	{MT9M114_SENSOR_WORD_WRITE, 0xC88E, 0x0780},
	{MT9M114_SENSOR_WORD_WRITE, 0xC914, 0x0000},
	{MT9M114_SENSOR_WORD_WRITE, 0xC916, 0x0000},
	{MT9M114_SENSOR_WORD_WRITE, 0xC918, 0x04FF},
	{MT9M114_SENSOR_WORD_WRITE, 0xC91A, 0x03BF},
	{MT9M114_SENSOR_WORD_WRITE, 0xC91C, 0x0000},
	{MT9M114_SENSOR_WORD_WRITE, 0xC91E, 0x0000},
	{MT9M114_SENSOR_WORD_WRITE, 0xC920, 0x00FF},
	{MT9M114_SENSOR_WORD_WRITE, 0xC922, 0x00BF},
	{MT9M114_SENSOR_BYTE_WRITE, 0xE801, 0x00},

	{MT9M114_SENSOR_WORD_WRITE, 0x316A, 0x8270},
	{MT9M114_SENSOR_WORD_WRITE, 0x316C, 0x8270},
	{MT9M114_SENSOR_WORD_WRITE, 0x3ED0, 0x2305},
	{MT9M114_SENSOR_WORD_WRITE, 0x3ED2, 0x77CF},
	{MT9M114_SENSOR_WORD_WRITE, 0x316E, 0x8202},
	{MT9M114_SENSOR_WORD_WRITE, 0x3180, 0x87FF},
	{MT9M114_SENSOR_WORD_WRITE, 0x30D4, 0x6080},
	{MT9M114_SENSOR_WORD_WRITE, 0xA802, 0x0008},

	{MT9M114_SENSOR_WORD_WRITE, 0x3E14, 0xFF39},
	{MT9M114_SENSOR_WORD_WRITE, 0x301A, 0x8234},

	{MT9M114_SENSOR_WORD_WRITE, 0x098E, 0x495E},
	{MT9M114_SENSOR_WORD_WRITE, 0xC95E, 0x0000},
	{MT9M114_SENSOR_WORD_WRITE, 0x3640, 0x02B0},
	{MT9M114_SENSOR_WORD_WRITE, 0x3642, 0x8063},
	{MT9M114_SENSOR_WORD_WRITE, 0x3644, 0x78D0},
	{MT9M114_SENSOR_WORD_WRITE, 0x3646, 0x50CC},
	{MT9M114_SENSOR_WORD_WRITE, 0x3648, 0x3511},
	{MT9M114_SENSOR_WORD_WRITE, 0x364A, 0x0110},
	{MT9M114_SENSOR_WORD_WRITE, 0x364C, 0xBD8A},
	{MT9M114_SENSOR_WORD_WRITE, 0x364E, 0x0CD1},
	{MT9M114_SENSOR_WORD_WRITE, 0x3650, 0x24ED},
	{MT9M114_SENSOR_WORD_WRITE, 0x3652, 0x7C11},
	{MT9M114_SENSOR_WORD_WRITE, 0x3654, 0x0150},
	{MT9M114_SENSOR_WORD_WRITE, 0x3656, 0x124C},
	{MT9M114_SENSOR_WORD_WRITE, 0x3658, 0x3130},
	{MT9M114_SENSOR_WORD_WRITE, 0x365A, 0x508C},
	{MT9M114_SENSOR_WORD_WRITE, 0x365C, 0x21F1},
	{MT9M114_SENSOR_WORD_WRITE, 0x365E, 0x0090},
	{MT9M114_SENSOR_WORD_WRITE, 0x3660, 0xBFCA},
	{MT9M114_SENSOR_WORD_WRITE, 0x3662, 0x0A11},
	{MT9M114_SENSOR_WORD_WRITE, 0x3664, 0x4F4B},
	{MT9M114_SENSOR_WORD_WRITE, 0x3666, 0x28B1},
	{MT9M114_SENSOR_WORD_WRITE, 0x3680, 0x50A9},
	{MT9M114_SENSOR_WORD_WRITE, 0x3682, 0xA04B},
	{MT9M114_SENSOR_WORD_WRITE, 0x3684, 0x0E2D},
	{MT9M114_SENSOR_WORD_WRITE, 0x3686, 0x73EC},
	{MT9M114_SENSOR_WORD_WRITE, 0x3688, 0x164F},
	{MT9M114_SENSOR_WORD_WRITE, 0x368A, 0xF829},
	{MT9M114_SENSOR_WORD_WRITE, 0x368C, 0xC1A8},
	{MT9M114_SENSOR_WORD_WRITE, 0x368E, 0xB0EC},
	{MT9M114_SENSOR_WORD_WRITE, 0x3690, 0xE76A},
	{MT9M114_SENSOR_WORD_WRITE, 0x3692, 0x69AF},
	{MT9M114_SENSOR_WORD_WRITE, 0x3694, 0x378C},
	{MT9M114_SENSOR_WORD_WRITE, 0x3696, 0xA70D},
	{MT9M114_SENSOR_WORD_WRITE, 0x3698, 0x884F},
	{MT9M114_SENSOR_WORD_WRITE, 0x369A, 0xEE8B},
	{MT9M114_SENSOR_WORD_WRITE, 0x369C, 0x5DEF},
	{MT9M114_SENSOR_WORD_WRITE, 0x369E, 0x27CC},
	{MT9M114_SENSOR_WORD_WRITE, 0x36A0, 0xCAAC},
	{MT9M114_SENSOR_WORD_WRITE, 0x36A2, 0x840E},
	{MT9M114_SENSOR_WORD_WRITE, 0x36A4, 0xDAA9},
	{MT9M114_SENSOR_WORD_WRITE, 0x36A6, 0xF00C},
	{MT9M114_SENSOR_WORD_WRITE, 0x36C0, 0x1371},
	{MT9M114_SENSOR_WORD_WRITE, 0x36C2, 0x272F},
	{MT9M114_SENSOR_WORD_WRITE, 0x36C4, 0x2293},
	{MT9M114_SENSOR_WORD_WRITE, 0x36C6, 0xE6D0},
	{MT9M114_SENSOR_WORD_WRITE, 0x36C8, 0xEC32},
	{MT9M114_SENSOR_WORD_WRITE, 0x36CA, 0x11B1},
	{MT9M114_SENSOR_WORD_WRITE, 0x36CC, 0x7BAF},
	{MT9M114_SENSOR_WORD_WRITE, 0x36CE, 0x5813},
	{MT9M114_SENSOR_WORD_WRITE, 0x36D0, 0xB871},
	{MT9M114_SENSOR_WORD_WRITE, 0x36D2, 0x8913},
	{MT9M114_SENSOR_WORD_WRITE, 0x36D4, 0x4610},
	{MT9M114_SENSOR_WORD_WRITE, 0x36D6, 0x7EEE},
	{MT9M114_SENSOR_WORD_WRITE, 0x36D8, 0x0DF3},
	{MT9M114_SENSOR_WORD_WRITE, 0x36DA, 0xB84F},
	{MT9M114_SENSOR_WORD_WRITE, 0x36DC, 0xB532},
	{MT9M114_SENSOR_WORD_WRITE, 0x36DE, 0x1171},
	{MT9M114_SENSOR_WORD_WRITE, 0x36E0, 0x13CF},
	{MT9M114_SENSOR_WORD_WRITE, 0x36E2, 0x22F3},
	{MT9M114_SENSOR_WORD_WRITE, 0x36E4, 0xE090},
	{MT9M114_SENSOR_WORD_WRITE, 0x36E6, 0x8133},
	{MT9M114_SENSOR_WORD_WRITE, 0x3700, 0x88AE},
	{MT9M114_SENSOR_WORD_WRITE, 0x3702, 0x00EA},
	{MT9M114_SENSOR_WORD_WRITE, 0x3704, 0x344F},
	{MT9M114_SENSOR_WORD_WRITE, 0x3706, 0xEC88},
	{MT9M114_SENSOR_WORD_WRITE, 0x3708, 0x3E91},
	{MT9M114_SENSOR_WORD_WRITE, 0x370A, 0xF12D},
	{MT9M114_SENSOR_WORD_WRITE, 0x370C, 0xB0EF},
	{MT9M114_SENSOR_WORD_WRITE, 0x370E, 0x77CD},
	{MT9M114_SENSOR_WORD_WRITE, 0x3710, 0x7930},
	{MT9M114_SENSOR_WORD_WRITE, 0x3712, 0x5C12},
	{MT9M114_SENSOR_WORD_WRITE, 0x3714, 0x500C},
	{MT9M114_SENSOR_WORD_WRITE, 0x3716, 0x22CE},
	{MT9M114_SENSOR_WORD_WRITE, 0x3718, 0x2370},
	{MT9M114_SENSOR_WORD_WRITE, 0x371A, 0x258F},
	{MT9M114_SENSOR_WORD_WRITE, 0x371C, 0x3D30},
	{MT9M114_SENSOR_WORD_WRITE, 0x371E, 0x370C},
	{MT9M114_SENSOR_WORD_WRITE, 0x3720, 0x03ED},
	{MT9M114_SENSOR_WORD_WRITE, 0x3722, 0x9AD0},
	{MT9M114_SENSOR_WORD_WRITE, 0x3724, 0x7ECF},
	{MT9M114_SENSOR_WORD_WRITE, 0x3726, 0x1093},
	{MT9M114_SENSOR_WORD_WRITE, 0x3740, 0x2391},
	{MT9M114_SENSOR_WORD_WRITE, 0x3742, 0xAAD0},
	{MT9M114_SENSOR_WORD_WRITE, 0x3744, 0x28F2},
	{MT9M114_SENSOR_WORD_WRITE, 0x3746, 0xBA4F},
	{MT9M114_SENSOR_WORD_WRITE, 0x3748, 0xC536},
	{MT9M114_SENSOR_WORD_WRITE, 0x374A, 0x1472},
	{MT9M114_SENSOR_WORD_WRITE, 0x374C, 0xD110},
	{MT9M114_SENSOR_WORD_WRITE, 0x374E, 0x2933},
	{MT9M114_SENSOR_WORD_WRITE, 0x3750, 0xD0D1},
	{MT9M114_SENSOR_WORD_WRITE, 0x3752, 0x9F37},
	{MT9M114_SENSOR_WORD_WRITE, 0x3754, 0x34D1},
	{MT9M114_SENSOR_WORD_WRITE, 0x3756, 0x1C6C},
	{MT9M114_SENSOR_WORD_WRITE, 0x3758, 0x3FD2},
	{MT9M114_SENSOR_WORD_WRITE, 0x375A, 0xCB72},
	{MT9M114_SENSOR_WORD_WRITE, 0x375C, 0xBA96},
	{MT9M114_SENSOR_WORD_WRITE, 0x375E, 0x1551},
	{MT9M114_SENSOR_WORD_WRITE, 0x3760, 0xB74F},
	{MT9M114_SENSOR_WORD_WRITE, 0x3762, 0x1672},
	{MT9M114_SENSOR_WORD_WRITE, 0x3764, 0x84F1},
	{MT9M114_SENSOR_WORD_WRITE, 0x3766, 0xC2D6},
	{MT9M114_SENSOR_WORD_WRITE, 0x3782, 0x01E0},
	{MT9M114_SENSOR_WORD_WRITE, 0x3784, 0x0280},
	{MT9M114_SENSOR_WORD_WRITE, 0x37C0, 0xA6EA},
	{MT9M114_SENSOR_WORD_WRITE, 0x37C2, 0x874B},
	{MT9M114_SENSOR_WORD_WRITE, 0x37C4, 0x85CB},
	{MT9M114_SENSOR_WORD_WRITE, 0x37C6, 0x968A},
	{MT9M114_SENSOR_WORD_WRITE, 0x098E, 0x0000},
	{MT9M114_SENSOR_WORD_WRITE, 0xC960, 0x0AF0},
	{MT9M114_SENSOR_WORD_WRITE, 0xC962, 0x79E2},
	{MT9M114_SENSOR_WORD_WRITE, 0xC964, 0x5EC8},
	{MT9M114_SENSOR_WORD_WRITE, 0xC966, 0x791F},
	{MT9M114_SENSOR_WORD_WRITE, 0xC968, 0x76EE},
	{MT9M114_SENSOR_WORD_WRITE, 0xC96A, 0x0FA0},
	{MT9M114_SENSOR_WORD_WRITE, 0xC96C, 0x7DFA},
	{MT9M114_SENSOR_WORD_WRITE, 0xC96E, 0x7DAF},
	{MT9M114_SENSOR_WORD_WRITE, 0xC970, 0x7E02},
	{MT9M114_SENSOR_WORD_WRITE, 0xC972, 0x7E0A},
	{MT9M114_SENSOR_WORD_WRITE, 0xC974, 0x1964},
	{MT9M114_SENSOR_WORD_WRITE, 0xC976, 0x7CDC},
	{MT9M114_SENSOR_WORD_WRITE, 0xC978, 0x7838},
	{MT9M114_SENSOR_WORD_WRITE, 0xC97A, 0x7C2F},
	{MT9M114_SENSOR_WORD_WRITE, 0xC97C, 0x7792},
	{MT9M114_SENSOR_WORD_WRITE, 0xC95E, 0x0003},

	{MT9M114_SENSOR_WORD_WRITE, 0x098E, 0x0000},
	{MT9M114_SENSOR_WORD_WRITE, 0xC95E, 0x0003},

	{MT9M114_SENSOR_WORD_WRITE, 0xC892, 0x0267},
	{MT9M114_SENSOR_WORD_WRITE, 0xC894, 0xFF1A},
	{MT9M114_SENSOR_WORD_WRITE, 0xC896, 0xFFB3},
	{MT9M114_SENSOR_WORD_WRITE, 0xC898, 0xFF80},
	{MT9M114_SENSOR_WORD_WRITE, 0xC89A, 0x0166},
	{MT9M114_SENSOR_WORD_WRITE, 0xC89C, 0x0003},
	{MT9M114_SENSOR_WORD_WRITE, 0xC89E, 0xFF9A},
	{MT9M114_SENSOR_WORD_WRITE, 0xC8A0, 0xFEB4},
	{MT9M114_SENSOR_WORD_WRITE, 0xC8A2, 0x024D},
	{MT9M114_SENSOR_WORD_WRITE, 0xC8A4, 0x01BF},
	{MT9M114_SENSOR_WORD_WRITE, 0xC8A6, 0xFF01},
	{MT9M114_SENSOR_WORD_WRITE, 0xC8A8, 0xFFF3},
	{MT9M114_SENSOR_WORD_WRITE, 0xC8AA, 0xFF75},
	{MT9M114_SENSOR_WORD_WRITE, 0xC8AC, 0x0198},
	{MT9M114_SENSOR_WORD_WRITE, 0xC8AE, 0xFFFD},
	{MT9M114_SENSOR_WORD_WRITE, 0xC8B0, 0xFF9A},
	{MT9M114_SENSOR_WORD_WRITE, 0xC8B2, 0xFEE7},
	{MT9M114_SENSOR_WORD_WRITE, 0xC8B4, 0x02A8},
	{MT9M114_SENSOR_WORD_WRITE, 0xC8B6, 0x01D9},
	{MT9M114_SENSOR_WORD_WRITE, 0xC8B8, 0xFF26},
	{MT9M114_SENSOR_WORD_WRITE, 0xC8BA, 0xFFF3},
	{MT9M114_SENSOR_WORD_WRITE, 0xC8BC, 0xFFB3},
	{MT9M114_SENSOR_WORD_WRITE, 0xC8BE, 0x0132},
	{MT9M114_SENSOR_WORD_WRITE, 0xC8C0, 0xFFE8},
	{MT9M114_SENSOR_WORD_WRITE, 0xC8C2, 0xFFDA},
	{MT9M114_SENSOR_WORD_WRITE, 0xC8C4, 0xFECD},
	{MT9M114_SENSOR_WORD_WRITE, 0xC8C6, 0x02C2},
	{MT9M114_SENSOR_WORD_WRITE, 0xC8C8, 0x0075},
	{MT9M114_SENSOR_WORD_WRITE, 0xC8CA, 0x011C},
	{MT9M114_SENSOR_WORD_WRITE, 0xC8CC, 0x009A},
	{MT9M114_SENSOR_WORD_WRITE, 0xC8CE, 0x0105},
	{MT9M114_SENSOR_WORD_WRITE, 0xC8D0, 0x00A4},
	{MT9M114_SENSOR_WORD_WRITE, 0xC8D2, 0x00AC},
	{MT9M114_SENSOR_WORD_WRITE, 0xC8D4, 0x0A8C},
	{MT9M114_SENSOR_WORD_WRITE, 0xC8D6, 0x0F0A},
	{MT9M114_SENSOR_WORD_WRITE, 0xC8D8, 0x1964},

	{MT9M114_SENSOR_WORD_WRITE, 0xC914, 0x0000},
	{MT9M114_SENSOR_WORD_WRITE, 0xC916, 0x0000},
	{MT9M114_SENSOR_WORD_WRITE, 0xC918, 0x04FF},
	{MT9M114_SENSOR_WORD_WRITE, 0xC91A, 0x03BF},
	{MT9M114_SENSOR_WORD_WRITE, 0xC904, 0x003B},
	{MT9M114_SENSOR_WORD_WRITE, 0xC906, 0x0041},
	{MT9M114_SENSOR_BYTE_WRITE, 0xC8F2, 0x03},
	{MT9M114_SENSOR_BYTE_WRITE, 0xC8F3, 0x02},
	{MT9M114_SENSOR_WORD_WRITE, 0xC906, 0x003C},
	{MT9M114_SENSOR_WORD_WRITE, 0xC8F4, 0x0000},
	{MT9M114_SENSOR_WORD_WRITE, 0xC8F6, 0x0000},
	{MT9M114_SENSOR_WORD_WRITE, 0xC8F8, 0x0000},
	{MT9M114_SENSOR_WORD_WRITE, 0xC8FA, 0xE724},
	{MT9M114_SENSOR_WORD_WRITE, 0xC8FC, 0x1583},
	{MT9M114_SENSOR_WORD_WRITE, 0xC8FE, 0x2045},
	{MT9M114_SENSOR_WORD_WRITE, 0xC900, 0x05DC},
	{MT9M114_SENSOR_WORD_WRITE, 0xC902, 0x007C},
	{MT9M114_SENSOR_BYTE_WRITE, 0xC90C, 0x80},
	{MT9M114_SENSOR_BYTE_WRITE, 0xC90D, 0x80},
	{MT9M114_SENSOR_BYTE_WRITE, 0xC90E, 0x80},
	{MT9M114_SENSOR_BYTE_WRITE, 0xC90F, 0x88},
	{MT9M114_SENSOR_BYTE_WRITE, 0xC910, 0x80},
	{MT9M114_SENSOR_BYTE_WRITE, 0xC911, 0x80},

	{MT9M114_SENSOR_WORD_WRITE, 0xC926, 0x0060},
	{MT9M114_SENSOR_WORD_WRITE, 0xC928, 0x009A},
	{MT9M114_SENSOR_WORD_WRITE, 0xC946, 0x0070},
	{MT9M114_SENSOR_WORD_WRITE, 0xC948, 0x00F3},
	{MT9M114_SENSOR_WORD_WRITE, 0xC952, 0x0060},
	{MT9M114_SENSOR_WORD_WRITE, 0xC954, 0x009A},
	{MT9M114_SENSOR_BYTE_WRITE, 0xC92A, 0x80},
	{MT9M114_SENSOR_BYTE_WRITE, 0xC92B, 0x4B},
	{MT9M114_SENSOR_BYTE_WRITE, 0xC92C, 0x00},
	{MT9M114_SENSOR_BYTE_WRITE, 0xC92D, 0xFF},
	{MT9M114_SENSOR_BYTE_WRITE, 0xC92E, 0x3C},
	{MT9M114_SENSOR_BYTE_WRITE, 0xC92F, 0x02},
	{MT9M114_SENSOR_BYTE_WRITE, 0xC930, 0x06},
	{MT9M114_SENSOR_BYTE_WRITE, 0xC931, 0x64},
	{MT9M114_SENSOR_BYTE_WRITE, 0xC932, 0x01},
	{MT9M114_SENSOR_BYTE_WRITE, 0xC933, 0x0C},
	{MT9M114_SENSOR_BYTE_WRITE, 0xC934, 0x3C},
	{MT9M114_SENSOR_BYTE_WRITE, 0xC935, 0x3C},
	{MT9M114_SENSOR_BYTE_WRITE, 0xC936, 0x3C},
	{MT9M114_SENSOR_BYTE_WRITE, 0xC937, 0x0F},
	{MT9M114_SENSOR_BYTE_WRITE, 0xC938, 0x64},
	{MT9M114_SENSOR_BYTE_WRITE, 0xC939, 0x64},
	{MT9M114_SENSOR_BYTE_WRITE, 0xC93A, 0x64},
	{MT9M114_SENSOR_BYTE_WRITE, 0xC93B, 0x32},
	{MT9M114_SENSOR_WORD_WRITE, 0xC93C, 0x0060},
	{MT9M114_SENSOR_WORD_WRITE, 0xC93E, 0x009A},
	{MT9M114_SENSOR_WORD_WRITE, 0xC940, 0x00DC},
	{MT9M114_SENSOR_BYTE_WRITE, 0xC942, 0x38},
	{MT9M114_SENSOR_BYTE_WRITE, 0xC943, 0x30},
	{MT9M114_SENSOR_BYTE_WRITE, 0xC944, 0x50},
	{MT9M114_SENSOR_BYTE_WRITE, 0xC945, 0x19},
	{MT9M114_SENSOR_WORD_WRITE, 0xC94A, 0x00F0},
	{MT9M114_SENSOR_WORD_WRITE, 0xC94C, 0x0010},
	{MT9M114_SENSOR_WORD_WRITE, 0xC94E, 0x01CD},
	{MT9M114_SENSOR_BYTE_WRITE, 0xC950, 0x05},
	{MT9M114_SENSOR_BYTE_WRITE, 0xC951, 0x40},
	{MT9M114_SENSOR_BYTE_WRITE, 0xC87A, 0x42},
	{MT9M114_SENSOR_BYTE_WRITE, 0xC87B, 0x21},
	{MT9M114_SENSOR_BYTE_WRITE, 0xC878, 0x0E},
	{MT9M114_SENSOR_WORD_WRITE, 0xC890, 0x0080},
	{MT9M114_SENSOR_WORD_WRITE, 0xC886, 0x0100},
	{MT9M114_SENSOR_WORD_WRITE, 0xC87C, 0x0010},
	{MT9M114_SENSOR_BYTE_WRITE, 0xB42A, 0x05},
	{MT9M114_SENSOR_BYTE_WRITE, 0xA80A, 0x20},
	{MT9M114_SENSOR_WORD_WRITE, 0x098E, 0x0000},
	{MT9M114_SENSOR_WORD_WRITE, 0xC984, 0x8041},
	{MT9M114_SENSOR_WORD_WRITE, 0x001E, 0x0777},
	{MT9M114_SENSOR_WORD_WRITE, 0x098E, 0xDC00},
	{MT9M114_SENSOR_BYTE_WRITE, 0xDC00, 0x28},
	{MT9M114_SENSOR_WAIT_MS, 0, 50},
	{MT9M114_SENSOR_WORD_WRITE, 0x0080, 0x8002},
	{MT9M114_SENSOR_TABLE_END, 0x0000}
};

struct mt9m114_info {
	struct miscdevice		miscdev_info;
	struct mt9m114_power_rail	power;
	struct mt9m114_sensordata	sensor_data;
	struct i2c_client		*i2c_client;
	struct mt9m114_platform_data	*pdata;
	atomic_t			in_use;
	const struct mt9m114_reg		*mode;
#ifdef CONFIG_DEBUG_FS
	struct dentry			*debugfs_root;
	u32				debug_i2c_offset;
#endif
	u8				i2c_trans_buf[SIZEOF_I2C_TRANSBUF];
};

struct mt9m114_mode_desc {
	u16			xres;
	u16			yres;
	const struct mt9m114_reg *mode_tbl;
	struct mt9m114_modeinfo	mode_info;
};

static struct mt9m114_mode_desc mode_table[] = {
	{
		.xres = 1280,
		.yres = 960,
		.mode_tbl = mode_1280x960_30fps,
	},
	{ },
};

static long mt9m114_ioctl(struct file *file,
			unsigned int cmd, unsigned long arg);

static inline void mt9m114_msleep(u32 t)
{
	usleep_range(t*1000, t*1000 + 500);
}

static int mt9m114_read_reg(struct i2c_client *client, u16 addr, u8 *val)
{
	int err;
	struct i2c_msg msg[2];
	unsigned char data[3];
=======
struct mt9m114_info {
	int mode;
	struct i2c_client *i2c_client;
	struct mt9m114_platform_data *pdata;
};

#ifdef MT9M114_DEBUG
	#define mt_info pr_info
#else
	#define mt_info(arg...) do {} while (0)
#endif

#define  mt_err pr_err

#define MT9M114_TABLE_WAIT_MS 0
#define MT9M114_TABLE_END 1
#define MT9M114_MAX_RETRIES 3

static struct mt9m114_reg mode_640x480[] = {
//##### Reset
//DELAY 100
{MT9M114_TABLE_WAIT_MS, 100},
{0x301A, 0x0230}, 	// RESET_REGISTER
//DELAY 100
{MT9M114_TABLE_WAIT_MS, 100},
//REG = 0x301A, 0x0230 	// RESET_REGISTER
{0x301A, 0x0230},

//[Step2-PLL_Timing]
{0x098E, 0x1000}, 	// LOGICAL_ADDRESS_ACCESS
{0x098E, 0xC97E}, 	// LOGICAL_ADDRESS_ACCESS
{0x0990, 0x0100},	// CAM_SYSCTL_PLL_ENABLE		
{0xC980, 0x0660},	//cam_sysctl_pll_divider_m_n = 1632
{0xC982, 0x0700},	//cam_sysctl_pll_divider_p = 1792
//MIPI Timing            
{0xC984, 0x8041},	//cam_port_output_control = 32833
{0xC988, 0x0F00},	//cam_port_mipi_timing_t_hs_zero = 3840
{0xC98A, 0x0B07},	//cam_port_mipi_timing_t_hs_exit_hs_trail = 2823
{0xC98C, 0x0D01},	//cam_port_mipi_timing_t_clk_post_clk_pre = 3329
{0xC98E, 0x071D},	//cam_port_mipi_timing_t_clk_trail_clk_zero = 1821
{0xC990, 0x0006},	//cam_port_mipi_timing_t_lpx = 6
{0xC992, 0x0A0C},	//cam_port_mipi_timing_init_timing = 2572
//Timing_settings        
{0xC800, 0x0004},	//cam_sensor_cfg_y_addr_start = 4
{0xC802, 0x0004},	//cam_sensor_cfg_x_addr_start = 4
{0xC804, 0x03CB},	//cam_sensor_cfg_y_addr_end = 971
{0xC806, 0x050B},	//cam_sensor_cfg_x_addr_end = 1291
{0x098E, 0x4808}, 	// LOGICAL_ADDRESS_ACCESS		===== 32
{0x0990, 0x02DC},	// cam_sensor_cfg_pixclk
{0x0992, 0x6C00},	// cam_sensor_cfg_pixclk
{0xC80C, 0x0001},		//cam_sensor_cfg_row_speed = 1
{0xC80E, 0x00DB},		//cam_sensor_cfg_fine_integ_time_min = 219
{0xC810, 0x05B3},		//cam_sensor_cfg_fine_integ_time_max = 1469
{0xC812, 0x03EE},		//cam_sensor_cfg_frame_length_lines = 1500
{0xC814, 0x0636},		//cam_sensor_cfg_line_length_pck = 1600
{0xC816, 0x0060},	//cam_sensor_cfg_fine_correction = 96
{0xC818, 0x03C3},	//cam_sensor_cfg_cpipe_last_row = 963
{0xC826, 0x0020},	//cam_sensor_cfg_reg_0_data = 32
{0xC834, 0x0000},	//cam_sensor_control_read_mode = 0
{0xC854, 0x0000},	//cam_crop_window_xoffset = 0
{0xC856, 0x0000},	//cam_crop_window_yoffset = 0
{0xC858, 0x0500},	//cam_crop_window_width = 1280              
{0xC85A, 0x03C0},	//cam_crop_window_height = 960              
{0x098E, 0xC85C},	//LOGICAL_ADDRESS_ACCESS			===== 8
{0x0990, 0x0300},	//cam_crop_cropmode = 3	                    
{0xC868, 0x0500},	//cam_output_width = 1280                   
{0xC86A, 0x03C0},	//cam_output_height = 960                   
{0x098E, 0xC878},	//LOGICAL_ADDRESS_ACCESS			===== 8
{0x0990, 0x0000},	//cam_aet_aemode = 0	
{0xC88C, 0x1E02},	//cam_aet_max_frame_rate = 5120
{0xC88E, 0x0A00},	//cam_aet_min_frame_rate = 2560
{0xC914, 0x0000},	//cam_stat_awb_clip_window_xstart = 0
{0xC916, 0x0000},	//cam_stat_awb_clip_window_ystart = 0
{0xC918, 0x04FF},		//cam_stat_awb_clip_window_xend = 1279
{0xC91A, 0x03BF},		//cam_stat_awb_clip_window_yend = 959
{0xC91C, 0x0000},	//cam_stat_ae_initial_window_xstart = 0
{0xC91E, 0x0000},	//cam_stat_ae_initial_window_ystart = 0
{0xC920, 0x00FF},		//cam_stat_ae_initial_window_xend = 255
{0xC922, 0x00BF},		//cam_stat_ae_initial_window_yend = 191
{0x098E, 0xE801},		//LOGICAL_ADDRESS_ACCESS			===== 8
{0x0990, 0x0000},	//cam_aet_aemode = 0	
{0x098E, 0xCC03}, // LOGICAL_ADDRESS_ACCESS [UVC_POWER_LINE_FREQUENCY_CONTROL]			===== 8
{0x0990, 0x0200}, // UVC_POWER_LINE_FREQUENCY_CONTROL  ==> 60Hz : 0x0200 , 50Hz ==>0x0100      
{0x098E, 0xC88B}, // LOGICAL_ADDRESS_ACCESS [CAM_AET_FLICKER_FREQ_HZ]			===== 8
{0x0990, 0x3400}, // CAM_AET_FLICKER_FREQ_HZ ==> 60Hz : 0x3400 , 50Hz ==>0x2C00
//[Step3-Recommended] //Optimization
{0x316A, 0x8270},
{0x316C, 0x8270},
{0x3ED0, 0x2305},
{0x3ED2, 0x77CF},
{0x316E, 0x8202},
{0x3180, 0x87FF},
{0x30D4, 0x6080},
{0xA802, 0x0008},	// AE_TRACK_MODE
{0x3E14, 0xFF39},	
{0x301A, 0x0234},


//[Change-Config]
{0x098E, 0xDC00}, 	// LOGICAL_ADDRESS_ACCESS			===== 8
{0x0990, 0x2800},	// AE_TRACK_AE_TRACKING_DAMPENING_SPEED
{0x0080, 0x8002}, 	// COMMAND_REGISTER
//DELAY=100

{MT9M114_TABLE_WAIT_MS, 100},

 {MT9M114_TABLE_END, 0x0000}
};

enum {
	MT9M114_MODE_680x480,
};
//                                                                           
#if 1
static struct mt9m114_reg mt9m114_Color_Effect_Normal[]= {
	{0x098E, 0xC874},	// LOGICAL_ADDRESS_ACCESS			===== 8
	{0x0990, 0x0000},	// AE_TRACK_AE_TRACKING_DAMPENING_SPEED
    {MT9M114_TABLE_WAIT_MS, 10},
    {MT9M114_TABLE_END, 0x0000}
};
static struct mt9m114_reg mt9m114_Color_Effect_MONO[]= {
	{0x098E, 0xC874},	// LOGICAL_ADDRESS_ACCESS			===== 8
	{0x0990, 0x0100},	// AE_TRACK_AE_TRACKING_DAMPENING_SPEED
    {MT9M114_TABLE_WAIT_MS, 10},
    {MT9M114_TABLE_END, 0x0000}
};
static struct mt9m114_reg mt9m114_Color_Effect_NEGATIVE[]= {
	{0x098E, 0xC874}, 	// LOGICAL_ADDRESS_ACCESS [CAM_SFX_CONTROL]
	{0x0990, 0x0300}, 	// CAM_SFX_CONTROL				========== 8
    {MT9M114_TABLE_WAIT_MS, 10},
    {MT9M114_TABLE_END, 0x0000}
};
static struct mt9m114_reg mt9m114_Color_Effect_SEPIA[]= {
	{0x098E, 0xC874}, 	// LOGICAL_ADDRESS_ACCESS [CAM_SFX_CONTROL]
	{0x0990, 0x0200}, 	// CAM_SFX_CONTROL				========== 8
	{0x098E, 0xC876}, 	// LOGICAL_ADDRESS_ACCESS [CAM_SFX_SEPIA_CR]
	{0x0990, 0x1E00},	// CAM_SFX_SEPIA_CR				========== 8
	{0x098E, 0xC877}, 	// LOGICAL_ADDRESS_ACCESS [CAM_SFX_SEPIA_CB]
	{0x0990, 0xE800},	// CAM_SFX_SEPIA_CR				========== 8
    {MT9M114_TABLE_WAIT_MS, 10},
    {MT9M114_TABLE_END, 0x0000}
};
static struct mt9m114_reg mt9m114_Color_Effect_Aqua[]= {
	{0x098E, 0xC874}, 	// LOGICAL_ADDRESS_ACCESS [CAM_SFX_CONTROL]
	{0x0990, 0x0200}, 	// CAM_SFX_CONTROL				========== 8
	{0x098E, 0xC876}, 	// LOGICAL_ADDRESS_ACCESS [CAM_SFX_SEPIA_CR]
	{0x0990, 0x0600},	// CAM_SFX_SEPIA_CR				========== 8
	{0x098E, 0xC877}, 	// LOGICAL_ADDRESS_ACCESS [CAM_SFX_SEPIA_CB]
	{0x0990, 0xE000},	// CAM_SFX_SEPIA_CR				========== 8
    {MT9M114_TABLE_WAIT_MS, 10},
    {MT9M114_TABLE_END, 0x0000}
};
static struct mt9m114_reg mt9m114_Color_Effect_Sketch[]= {
	{0x098E, 0xC874}, 	// LOGICAL_ADDRESS_ACCESS [CAM_SFX_CONTROL]
	{0x0990, 0x0400}, 	// CAM_SFX_CONTROL				========== 8
    {MT9M114_TABLE_WAIT_MS, 10},
    {MT9M114_TABLE_END, 0x0000}
};

static struct mt9m114_reg mt9m114_MWB_Auto[]= {
	{0x098E, 0xC909}, 	// LOGICAL_ADDRESS_ACCESS [CAM_AWB_AWBMODE]
	{0x0990, 0x0300}, 	// CAM_AWB_AWBMODE					========== 8
	{0x098E, 0xC90F}, 	// LOGICAL_ADDRESS_ACCESS [CAM_AWB_K_R_R]
	{0X0990, 0X8000},
	{0x098E, 0xC910}, 	// LOGICAL_ADDRESS_ACCESS [CAM_AWB_K_G_R]
	{0X0990, 0X8000},
	{0x098E, 0xC911}, 	// LOGICAL_ADDRESS_ACCESS [CAM_AWB_K_B_R]
	{0X0990, 0X8000},
    {MT9M114_TABLE_WAIT_MS, 10},
    {MT9M114_TABLE_END, 0x0000}
};
static struct mt9m114_reg mt9m114_MWB_Incandescent[]= {
	{0x098E, 0xC909}, 	// LOGICAL_ADDRESS_ACCESS [CAM_AWB_AWBMODE]
	{0x0990, 0x0100}, 	// CAM_AWB_AWBMODE					========== 8
	{0x098E, 0xC90F}, 	// LOGICAL_ADDRESS_ACCESS [CAM_AWB_K_R_R]
	{0X0990, 0X1800},
	{0x098E, 0xC910}, 	// LOGICAL_ADDRESS_ACCESS [CAM_AWB_K_G_R]
	{0X0990, 0X8000},
	{0x098E, 0xC911}, 	// LOGICAL_ADDRESS_ACCESS [CAM_AWB_K_B_R]
	{0X0990, 0XA300},
	{0xC8F0, 0x0C80}, 	// CAM_AWB_COLOR_TEMPERATURE
    {MT9M114_TABLE_WAIT_MS, 10},
    {MT9M114_TABLE_END, 0x0000}
};
static struct mt9m114_reg mt9m114_MWB_Fluorescent[]= {
	{0x098E, 0xC909}, 	// LOGICAL_ADDRESS_ACCESS [CAM_AWB_AWBMODE]
	{0x0990, 0x0100}, 	// CAM_AWB_AWBMODE					========== 8
	{0x098E, 0xC90F}, 	// LOGICAL_ADDRESS_ACCESS [CAM_AWB_K_R_R]
	{0X0990, 0X9000},
	{0x098E, 0xC910}, 	// LOGICAL_ADDRESS_ACCESS [CAM_AWB_K_G_R]
	{0X0990, 0X8000},
	{0x098E, 0xC911}, 	// LOGICAL_ADDRESS_ACCESS [CAM_AWB_K_B_R]
	{0X0990, 0X9F00},
	{0xC8F0, 0x11E6}, 	// CAM_AWB_COLOR_TEMPERATURE
    {MT9M114_TABLE_WAIT_MS, 10},
    {MT9M114_TABLE_END, 0x0000}
};
static struct mt9m114_reg mt9m114_MWB_Daylight[]= {
	{0x098E, 0xC909}, 	// LOGICAL_ADDRESS_ACCESS [CAM_AWB_AWBMODE]
	{0x0990, 0x0100}, 	// CAM_AWB_AWBMODE					========== 8
	{0x098E, 0xC90F}, 	// LOGICAL_ADDRESS_ACCESS [CAM_AWB_K_R_R]
	{0X0990, 0X8000},
	{0x098E, 0xC910}, 	// LOGICAL_ADDRESS_ACCESS [CAM_AWB_K_G_R]
	{0X0990, 0X8000},
	{0x098E, 0xC911}, 	// LOGICAL_ADDRESS_ACCESS [CAM_AWB_K_B_R]
	{0X0990, 0X7800},
	{0xC8F0, 0x1964}, 	// CAM_AWB_COLOR_TEMPERATURE
    {MT9M114_TABLE_WAIT_MS, 10},
    {MT9M114_TABLE_END, 0x0000}
};
static struct mt9m114_reg mt9m114_MWB_CloudyDaylight[]= {
	{0x098E, 0xC909}, 	// LOGICAL_ADDRESS_ACCESS [CAM_AWB_AWBMODE]
	{0x0990, 0x0100}, 	// CAM_AWB_AWBMODE					========== 8
	{0x098E, 0xC90F}, 	// LOGICAL_ADDRESS_ACCESS [CAM_AWB_K_R_R]
	{0X0990, 0x9D00},
	{0x098E, 0xC910}, 	// LOGICAL_ADDRESS_ACCESS [CAM_AWB_K_G_R]
	{0X0990, 0x8000},
	{0x098E, 0xC911}, 	// LOGICAL_ADDRESS_ACCESS [CAM_AWB_K_B_R]
	{0X0990, 0x5C00},
	{0xC8F0, 0x1964}, 	// CAM_AWB_COLOR_TEMPERATURE
    {MT9M114_TABLE_WAIT_MS, 10},
    {MT9M114_TABLE_END, 0x0000}
};
static struct mt9m114_reg mt9m114_Framerate_7Fps[]= {
    {0x098E, 0x4812},
    {0xC810, 0x15EC},
    {0xC812, 0x04AA},
    {0xC814, 0x166F},
    {0xC88C, 0x0700},
    {0xC88E, 0x0700},
	{0x098E, 0xDC00},	// LOGICAL_ADDRESS_ACCESS			===== 8
	{0x0990, 0x2800},	// AE_TRACK_AE_TRACKING_DAMPENING_SPEED
	{0x0080, 0x8002}, 	// COMMAND_REGISTER
    {MT9M114_TABLE_WAIT_MS, 10},
    {MT9M114_TABLE_END, 0x0000}
};
static struct mt9m114_reg mt9m114_Framerate_10Fps[]= {
	{0x098E, 0x4812},
	{0xC810, 0x05BD},
	{0xC812, 0x0BB8},
	{0xC814, 0x0640},
	{0xC88C, 0x0A00},
	{0xC88E, 0x0A00},
	{0x098E, 0xDC00},	// LOGICAL_ADDRESS_ACCESS			===== 8
	{0x0990, 0x2800},	// AE_TRACK_AE_TRACKING_DAMPENING_SPEED
	{0x0080, 0x8002},	// COMMAND_REGISTER
	{MT9M114_TABLE_WAIT_MS, 10},
	{MT9M114_TABLE_END, 0x0000}

};
static struct mt9m114_reg mt9m114_Framerate_15Fps[]= {
	{0x098E, 0x4812},
	{0xC810, 0x05B3},
	{0xC812, 0x07E0},
	{0xC814, 0x0636},
	{0xC88C, 0x1E02},
	{0xC88E, 0x1E02},
	{0x098E, 0xDC00},	// LOGICAL_ADDRESS_ACCESS			===== 8
	{0x0990, 0x2800},	// AE_TRACK_AE_TRACKING_DAMPENING_SPEED
	{0x0080, 0x8002},	// COMMAND_REGISTER
	{MT9M114_TABLE_WAIT_MS, 10},
	{MT9M114_TABLE_END, 0x0000}

};
//10~ 30fps
static struct mt9m114_reg mt9m114_Framerate_VarFps[]= {
    {0x098E, 0x4812},
    {0xC810, 0x05B3},
    {0xC812, 0x03EE},
    {0xC814, 0x0636},
    {0xC88C, 0x1E02},
    {0xC88E, 0x0A00},
	{0x098E, 0xDC00},	// LOGICAL_ADDRESS_ACCESS			===== 8
	{0x0990, 0x2800},	// AE_TRACK_AE_TRACKING_DAMPENING_SPEED
	{0x0080, 0x8002}, 	// COMMAND_REGISTER
    {MT9M114_TABLE_WAIT_MS, 10},
    {MT9M114_TABLE_END, 0x0000}
};
static struct mt9m114_reg *mt9m114_Set_FramerateList[] =
{
	mt9m114_Framerate_7Fps,
	mt9m114_Framerate_10Fps,
	mt9m114_Framerate_15Fps,
	mt9m114_Framerate_VarFps

};
//10~ 30fps
static struct mt9m114_reg mt9m114_ScenMode_Auto[]= {
	    {0x098E, 0x4812},
        {0xC810, 0x05B3},
	    {0xC812, 0x03EE},
	    {0xC814, 0x0636},
        {0xC88C, 0x1E02},
	    {0xC88E, 0x0A00},
		{0x098E, 0xDC00},	// LOGICAL_ADDRESS_ACCESS			===== 8
		{0x0990, 0x2800},	// AE_TRACK_AE_TRACKING_DAMPENING_SPEED
		{0x0080, 0x8002}, 	// COMMAND_REGISTER
	    {MT9M114_TABLE_WAIT_MS, 10},
	    {MT9M114_TABLE_END, 0x0000}
};
//7.5~30fps
static struct mt9m114_reg mt9m114_ScenMode_Night[]= {
	    {0x098E, 0x4812},
        {0xC810, 0x05B3},
	    {0xC812, 0x03EE},
	    {0xC814, 0x0636},
        {0xC88C, 0x1E02},
	    {0xC88E, 0x0780},
		{0x098E, 0xDC00},	// LOGICAL_ADDRESS_ACCESS			===== 8
		{0x0990, 0x2800},	// AE_TRACK_AE_TRACKING_DAMPENING_SPEED
		{0x0080, 0x8002}, 	// COMMAND_REGISTER
        {MT9M114_TABLE_WAIT_MS, 10},
	    {MT9M114_TABLE_END, 0x0000}
};
static struct mt9m114_info *info;

static int prev_brightness_mode;
#endif

//                                                                           
static struct mt9m114_reg *mode_table[] = {
	[MT9M114_MODE_680x480] = mode_640x480,
};

static int mt9m114_read_reg(struct i2c_client *client, u16 addr, u16 *val)
{
	int err;
	struct i2c_msg msg[2];
	unsigned char data[4];
>>>>>>> f720e99... drivers/: Merge LGE changes [2]

	if (!client->adapter)
		return -ENODEV;

	msg[0].addr = client->addr;
	msg[0].flags = 0;
	msg[0].len = 2;
	msg[0].buf = data;

<<<<<<< HEAD
=======
	/* high byte goes out first */
>>>>>>> f720e99... drivers/: Merge LGE changes [2]
	data[0] = (u8) (addr >> 8);
	data[1] = (u8) (addr & 0xff);

	msg[1].addr = client->addr;
	msg[1].flags = I2C_M_RD;
<<<<<<< HEAD
	msg[1].len = 1;
=======
	msg[1].len = 2;
>>>>>>> f720e99... drivers/: Merge LGE changes [2]
	msg[1].buf = data + 2;

	err = i2c_transfer(client->adapter, msg, 2);

	if (err != 2)
		return -EINVAL;

<<<<<<< HEAD
	*val = data[2];
=======
	*val = data[2] << 8 | data[3];
>>>>>>> f720e99... drivers/: Merge LGE changes [2]

	return 0;
}

<<<<<<< HEAD
static int mt9m114_write_bulk_reg(struct i2c_client *client, u8 *data, int len)
{
	int err;
	struct i2c_msg msg;

	if (!client->adapter)
		return -ENODEV;

	msg.addr = client->addr;
	msg.flags = 0;
	msg.len = len;
	msg.buf = data;

	dev_dbg(&client->dev,
		"%s {0x%04x,", __func__, (int)data[0] << 8 | data[1]);
	for (err = 2; err < len; err++)
		dev_dbg(&client->dev, " 0x%02x", data[err]);
	dev_dbg(&client->dev, "},\n");

	err = i2c_transfer(client->adapter, &msg, 1);
	if (err == 1)
		return 0;

	dev_err(&client->dev, "mt9m114: i2c bulk transfer failed at %x\n",
		(int)data[0] << 8 | data[1]);

	return err;
}

static int mt9m114_write_reg8(struct i2c_client *client, u16 addr, u8 val)
{
	unsigned char data[3];

	if (!client->adapter)
		return -ENODEV;

	data[0] = (u8) (addr >> 8);
	data[1] = (u8) (addr & 0xff);
	data[2] = (u8) (val & 0xff);

	dev_dbg(&client->dev, "0x%x = 0x%x\n", addr, val);
	return mt9m114_write_bulk_reg(client, data, sizeof(data));
}

static int mt9m114_write_reg16(struct i2c_client *client, u16 addr, u16 val)
{
	unsigned char data[4];
=======
static int mt9m114_write_reg(struct i2c_client *client, u16 addr, u16 val)
{
	int err;
	struct i2c_msg msg;
	unsigned char data[4];
	int retry = 0;
>>>>>>> f720e99... drivers/: Merge LGE changes [2]

	if (!client->adapter)
		return -ENODEV;

	data[0] = (u8) (addr >> 8);
	data[1] = (u8) (addr & 0xff);
	data[2] = (u8) (val >> 8);
	data[3] = (u8) (val & 0xff);

<<<<<<< HEAD
	dev_dbg(&client->dev, "0x%x = 0x%x\n", addr, val);
	return mt9m114_write_bulk_reg(client, data, sizeof(data));
}

static int mt9m114_write_table(
	struct mt9m114_info *info,
	const struct mt9m114_reg table[])
{
	int err;
	const struct mt9m114_reg *next;
	u16 val;

	dev_dbg(&info->i2c_client->dev, "yuv %s\n", __func__);

	for (next = table; next->cmd != MT9M114_SENSOR_TABLE_END; next++) {
		if (next->cmd == MT9M114_SENSOR_WAIT_MS) {
			msleep(next->val);
		continue;
=======
	msg.addr = client->addr;
	msg.flags = 0;
	msg.len = 4;
	msg.buf = data;

	do {
		err = i2c_transfer(client->adapter, &msg, 1);
		//mt_info("mt9m114: i2c transfer under transferring %x %x\n", addr, val);
		if (err == 1)
			return 0;
		retry++;
		mt_err("mt9m114: i2c transfer failed, retrying %x %x\n",
		       addr, val);
		msleep(3);
	} while (retry <= MT9M114_MAX_RETRIES);

	return err;
}

static int mt9m114_write_table(struct i2c_client *client,
			      const struct mt9m114_reg table[],
			      const struct mt9m114_reg override_list[],
			      int num_override_regs)
{
	int err;
	const struct mt9m114_reg *next;
	int i;
	u16 val;

	mt_info("mt9m114: mt9m114_write_table entered");
	for (next = table; next->addr != MT9M114_TABLE_END; next++) {
		//mt_info("mt9m114: mt9m114_write_table 1");
		if (next->addr == MT9M114_TABLE_WAIT_MS) {
			mt_info("mt9m114: mt9m114_write_table : MT9M114_TABLE_WAIT_MS ");
			msleep(next->val);
			continue;
>>>>>>> f720e99... drivers/: Merge LGE changes [2]
		}

		val = next->val;

<<<<<<< HEAD
		if (next->cmd == MT9M114_SENSOR_BYTE_WRITE)
			err = mt9m114_write_reg8(info->i2c_client, next->addr, val);
		else if (next->cmd == MT9M114_SENSOR_WORD_WRITE)
			err = mt9m114_write_reg16(info->i2c_client, next->addr, val);
		if (err)
			return err;
=======
		/* When an override list is passed in, replace the reg */
		/* value to write if the reg is in the list            */
		if (override_list) {
			for (i = 0; i < num_override_regs; i++) {
				if (next->addr == override_list[i].addr) {
					val = override_list[i].val;
					break;
				}
			}
		}

		//mt_info("mt9m114: mt9m114_write_table 2");
		err = mt9m114_write_reg(client, next->addr, val);
		if (err) {
			mt_err("mt9m114: mt9m114_write_table : err");
			return err;
		}
>>>>>>> f720e99... drivers/: Merge LGE changes [2]
	}
	return 0;
}

<<<<<<< HEAD
static int mt9m114_open(struct inode *inode, struct file *file)
{
	struct miscdevice	*miscdev = file->private_data;
	struct mt9m114_info *info = dev_get_drvdata(miscdev->parent);

	dev_dbg(&info->i2c_client->dev, "mt9m114: open.\n");
	info = container_of(miscdev, struct mt9m114_info, miscdev_info);
	/* check if the device is in use */
	if (atomic_xchg(&info->in_use, 1)) {
		dev_info(&info->i2c_client->dev, "%s:BUSY!\n", __func__);
		return -EBUSY;
	}

	file->private_data = info;

	if (info->pdata && info->pdata->power_on)
		info->pdata->power_on(&info->power);
	else {
		dev_err(&info->i2c_client->dev,
			"%s:no valid power_on function.\n", __func__);
		return -EFAULT;
	}
	return 0;
}

int mt9m114_release(struct inode *inode, struct file *file)
{
	struct mt9m114_info *info = file->private_data;

	if (info->pdata && info->pdata->power_off)
		info->pdata->power_off(&info->power);
	file->private_data = NULL;

	/* warn if device is already released */
	WARN_ON(!atomic_xchg(&info->in_use, 0));
	return 0;
}

static int mt9m114_regulator_get(struct mt9m114_info *info,
	struct regulator **vreg, char vreg_name[])
{
	struct regulator *reg = NULL;
	int err = 0;

	reg = devm_regulator_get(&info->i2c_client->dev, vreg_name);
	if (unlikely(IS_ERR(reg))) {
		dev_err(&info->i2c_client->dev, "%s %s ERR: %d\n",
			__func__, vreg_name, (int)reg);
		err = PTR_ERR(reg);
		reg = NULL;
	} else {
		dev_dbg(&info->i2c_client->dev, "%s: %s\n",
			__func__, vreg_name);
	}

	*vreg = reg;
	return err;
}

static int mt9m114_power_get(struct mt9m114_info *info)
{
	struct mt9m114_power_rail *pw = &info->power;

	dev_dbg(&info->i2c_client->dev, "mt9m114: %s\n", __func__);

	/* note: mt9m114 uses i2c address 0x90, which is different from
	 * most of other sensors that using 0x64 or 0x20.
	 *
	 * This needs us to define a new vif2 as 2-0048
	 * for platform board file that uses mt9m114
	 * otherwise below could not get the regulator
	 *
	 * This rails of "vif2" and "vana" can be modified as needed
	 * for a new platform.
	 *
	 * mt9m114: need to get 1.8v first 
	 */
	mt9m114_regulator_get(info, &pw->iovdd, "vif2"); /* interface 1.8v */
	mt9m114_regulator_get(info, &pw->avdd, "vana"); /* ananlog 2.7v */

	return 0;
}

static const struct file_operations mt9m114_fileops = {
	.owner = THIS_MODULE,
	.open = mt9m114_open,
	.unlocked_ioctl = mt9m114_ioctl,
	.release = mt9m114_release,
};

static struct miscdevice mt9m114_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "mt9m114",
	.fops = &mt9m114_fileops,
};

#ifdef CONFIG_DEBUG_FS
static int mt9m114_stats_show(struct seq_file *s, void *data)
{
	static struct mt9m114_info *info;

	seq_printf(s, "%-20s : %-20s\n", "Name", "mt9m114-debugfs-testing");
	seq_printf(s, "%-20s : 0x%X\n", "Current i2c-offset Addr",
			info->debug_i2c_offset);
	return 0;
}

static int mt9m114_stats_open(struct inode *inode, struct file *file)
{
	return single_open(file, mt9m114_stats_show, inode->i_private);
}

static const struct file_operations mt9m114_stats_fops = {
	.open       = mt9m114_stats_open,
	.read       = seq_read,
	.llseek     = seq_lseek,
	.release    = single_release,
};

static int debug_i2c_offset_w(void *data, u64 val)
{
	struct mt9m114_info *info = (struct mt9m114_info *)(data);
	dev_info(&info->i2c_client->dev,
			"mt9m114:%s setting i2c offset to 0x%X\n",
			__func__, (u32)val);
	info->debug_i2c_offset = (u32)val;
	dev_info(&info->i2c_client->dev,
			"mt9m114:%s new i2c offset is 0x%X\n", __func__,
			info->debug_i2c_offset);
	return 0;
}

static int debug_i2c_offset_r(void *data, u64 *val)
{
	struct mt9m114_info *info = (struct mt9m114_info *)(data);
	*val = (u64)info->debug_i2c_offset;
	dev_info(&info->i2c_client->dev,
			"mt9m114:%s reading i2c offset is 0x%X\n", __func__,
			info->debug_i2c_offset);
	return 0;
}

static int debug_i2c_read(void *data, u64 *val)
{
	struct mt9m114_info *info = (struct mt9m114_info *)(data);
	u8 temp1 = 0;
	u8 temp2 = 0;
	dev_info(&info->i2c_client->dev,
			"mt9m114:%s reading offset 0x%X\n", __func__,
			info->debug_i2c_offset);
	if (mt9m114_read_reg(info->i2c_client,
				info->debug_i2c_offset, &temp1)
		|| mt9m114_read_reg(info->i2c_client,
			info->debug_i2c_offset+1, &temp2)) {
		dev_err(&info->i2c_client->dev,
				"mt9m114:%s failed\n", __func__);
		return -EIO;
	}
	dev_info(&info->i2c_client->dev,
			"mt9m114:%s read value is 0x%X\n", __func__,
			temp1<<8 | temp2);
	*val = (u64)(temp1<<8 | temp2);
	return 0;
}

static int debug_i2c_write(void *data, u64 val)
{
	struct mt9m114_info *info = (struct mt9m114_info *)(data);
	dev_info(&info->i2c_client->dev,
			"mt9m114:%s writing 0x%X to offset 0x%X\n", __func__,
			(u16)val, info->debug_i2c_offset);
	if (mt9m114_write_reg16(info->i2c_client,
				info->debug_i2c_offset, (u16)val)) {
		dev_err(&info->i2c_client->dev,
			"mt9m114:%s failed\n", __func__);
		return -EIO;
	}
	return 0;
}

DEFINE_SIMPLE_ATTRIBUTE(i2c_offset_fops, debug_i2c_offset_r,
		debug_i2c_offset_w, "0x%llx\n");
DEFINE_SIMPLE_ATTRIBUTE(i2c_read_fops, debug_i2c_read,
		/*debug_i2c_dummy_w*/ NULL, "0x%llx\n");
DEFINE_SIMPLE_ATTRIBUTE(i2c_write_fops, /*debug_i2c_dummy_r*/NULL,
		debug_i2c_write, "0x%llx\n");

static int mt9m114_debug_init(struct mt9m114_info *info)
{
	dev_dbg(&info->i2c_client->dev, "%s", __func__);

	info->debugfs_root = debugfs_create_dir(mt9m114_device.name, NULL);

	if (!info->debugfs_root)
		goto err_out;

	if (!debugfs_create_file("stats", S_IRUGO,
			info->debugfs_root, info, &mt9m114_stats_fops))
		goto err_out;

	if (!debugfs_create_file("i2c_offset", S_IRUGO | S_IWUSR,
			info->debugfs_root, info, &i2c_offset_fops))
		goto err_out;

	if (!debugfs_create_file("i2c_read", S_IRUGO,
			info->debugfs_root, info, &i2c_read_fops))
		goto err_out;

	if (!debugfs_create_file("i2c_write", S_IWUSR,
			info->debugfs_root, info, &i2c_write_fops))
		goto err_out;

	return 0;

err_out:
	dev_err(&info->i2c_client->dev, "ERROR:%s failed", __func__);
	if (info->debugfs_root)
		debugfs_remove_recursive(info->debugfs_root);
	return -ENOMEM;
}
#endif	/* CONFIG_DEBUG_FS */

static struct mt9m114_modeinfo def_modeinfo = {
	.xres = 1280,
	.yres = 960,
};

static struct mt9m114_mode_desc *mt9m114_get_mode(
	struct mt9m114_info *info, struct mt9m114_mode *mode)
{
	struct mt9m114_mode_desc *mt = mode_table;

	while (mt->xres) {
		if ((mt->xres == mode->xres) &&
			(mt->yres == mode->yres))
				break;
		mt++;
	}

	if (!mt->xres)
		mt = NULL;
	return mt;
}

static int mt9m114_mode_info_init(struct mt9m114_info *info)
{
	struct mt9m114_mode_desc *md = mode_table;
	const struct mt9m114_reg *mt;
	struct mt9m114_modeinfo *mi;

	dev_dbg(&info->i2c_client->dev, "%s", __func__);
	while (md->xres) {
		mi = &md->mode_info;
		mt = md->mode_tbl;
		memcpy(mi, &def_modeinfo, sizeof(*mi));
		dev_dbg(&info->i2c_client->dev, "mode %d x %d ",
			md->xres, md->yres);
		mi->xres = md->xres;
		mi->yres = md->yres;
		md++;
	}
	return 0;
}

static int mt9m114_set_mode(struct mt9m114_info *info,
	struct mt9m114_mode *mode)
{
	struct mt9m114_mode_desc *sensor_mode;
	int err;

	dev_info(&info->i2c_client->dev,
		"%s: xres %u yres %u\n", __func__, mode->xres, mode->yres);

	sensor_mode = mt9m114_get_mode(info, mode);
	if (sensor_mode == NULL) {
		dev_err(&info->i2c_client->dev,
			"%s: invalid params supplied to set mode %d %d\n",
				__func__, mode->xres, mode->yres);
		return -EINVAL;
	}

	err = mt9m114_write_table(
		info, sensor_mode->mode_tbl);
	if (err)
		return err;

	info->mode = sensor_mode->mode_tbl;

	return 0;
}

static long mt9m114_ioctl(struct file *file,
			 unsigned int cmd, unsigned long arg)
{
	int err = 0;
	struct mt9m114_info *info = file->private_data;

	switch (cmd) {
	case MT9M114_SENSOR_IOCTL_SET_MODE:
	{
		struct mt9m114_mode mode;

		dev_dbg(&info->i2c_client->dev, "MT9M114_IOCTL_SET_MODE\n");
		if (copy_from_user(&mode, (const void __user *)arg,
			sizeof(struct mt9m114_mode))) {
			err = -EFAULT;
			break;
		}
		err = mt9m114_set_mode(info, &mode);
		break;
	}

	default:
		dev_dbg(&info->i2c_client->dev, "INVALID IOCTL\n");
		err = -EINVAL;
	}

	if (err)
		dev_err(&info->i2c_client->dev,
			"%s - %x: ERR = %d\n", __func__, cmd, err);
	return err;
}

=======
//                                                                           
#if 1
static int mt9m114_set_color_effect(struct mt9m114_info *info, unsigned int color_effect)
{
	int err = 0;
	pr_info("%s : %d\n", __func__, color_effect);

	switch(color_effect)
	{
		case YUVCamColorEffect_None :
			err = mt9m114_write_table(info->i2c_client, mt9m114_Color_Effect_Normal,NULL,0);
			break;

		case YUVCamColorEffect_Negative :
			err = mt9m114_write_table(info->i2c_client, mt9m114_Color_Effect_NEGATIVE,NULL,0);

			break;

		case YUVCamColorEffect_Aqua :
			err = mt9m114_write_table(info->i2c_client, mt9m114_Color_Effect_Aqua,NULL,0);

			break;

		case YUVCamColorEffect_Posterize :
			err = mt9m114_write_table(info->i2c_client, mt9m114_Color_Effect_Sketch,NULL,0);

			break;

		case YUVCamColorEffect_Sepia :
			err = mt9m114_write_table(info->i2c_client, mt9m114_Color_Effect_SEPIA,NULL,0);

			break;

		case YUVCamColorEffect_Mono :
			err = mt9m114_write_table(info->i2c_client, mt9m114_Color_Effect_MONO,NULL,0);

			break;

		default :
			//err = mt9m114_write_table(info->i2c_client, mt9m114_Color_Effect_Normal,NULL,0);

			break;
	}
	err = mt9m114_write_reg(info->i2c_client,0x0080, 0x8004);
	msleep(100);
	if (err < 0)
		return err;
#if 0
	{
		unsigned short test_data = 0, i;
		for(i=0; i<50; i++){ // max delay ==> 500 ms
			rc  = mt9m114_read_reg(mt9m114_client->addr, 0x0080, &test_data);
			if (rc < 0)
				return rc;

			if((test_data & 0x0004)==0)
				break;
			else
				mdelay(10);

			CDBG_VT("### %s :  Refresh Polling set, 0x0080 Reg : 0x%x\n", __func__, test_data);
		}
	}
#endif
	if (err !=0)
		pr_info("%s : Color Effect : %d,  error : %d\n", __func__, color_effect, err);

	return err;
}

static int mt9m114_set_white_balance(struct mt9m114_info *info, unsigned int wb_mode)
{
	int err = 0;
	pr_info("%s : %d\n", __func__, wb_mode);
	switch(wb_mode)
	{
		case YUVCamWhitebalance_Auto :
			err = mt9m114_write_table(info->i2c_client, mt9m114_MWB_Auto,NULL,0);
			break;

		case YUVCamWhitebalance_Incandescent :
			err = mt9m114_write_table(info->i2c_client, mt9m114_MWB_Incandescent,NULL,0);

			break;

		case YUVCamWhitebalance_SunLight :
			err = mt9m114_write_table(info->i2c_client, mt9m114_MWB_Daylight,NULL,0);

			break;
		case YUVCamWhitebalance_CloudyDayLight:
			err = mt9m114_write_table(info->i2c_client, mt9m114_MWB_CloudyDaylight,NULL,0);

			break;

		case YUVCamWhitebalance_Fluorescent :
			err = mt9m114_write_table(info->i2c_client, mt9m114_MWB_Fluorescent,NULL,0);

			break;

		default :
			//err = mt9m114_write_table(info->i2c_client, mt9m114_MWB_Auto,NULL,0);

			break;
	}
	if (err !=0)
		pr_info("%s : White Balance : %d,  error : %d\n", __func__, wb_mode, err);
	return err;
}

static int brightness_table[] = {0x0020, 0x0024, 0x0028, 0x002E, 0x0032, 0x0035, 0x003E, 0x0048, 0x0050, 0x0054, 0x0058, 0x005C, 0x005F}; // 13 step
																		// value < 0x80 --> value should be increased , max value => 0x7F00
							// value > 0x80 --> value should be decreased	, max value => 0x8100
static int gamma_table_sub[] = {0x00C4, 0x00C8, 0x00CC, 0x00D0, 0x00D4, 0x00D8, 0x00DC, 0x00DF, 0x00E3, 0x00E8, 0x00ED, 0x00F2, 0x00F7}; // 13 step
							// 2.2 gamma
static int mt9m114_set_exposure(struct mt9m114_info *info, ExposureValueType *exposure)
{
	int32_t rc = 0;
	int index;

	if(exposure == NULL || exposure->range == 0)
	{
		pr_info("%s : invalid pointer or range value\n", __func__);
	    return -1;
	}
    if ( exposure->index == 0)
		index = 6;
	else
		index = (exposure->index / 5) + 6;

    if(prev_brightness_mode == index)
	{
		pr_info("###  [CHECK]%s: skip this function, brightness_mode -> %d\n", __func__, index);
		return rc;
	}
	pr_info(" %s : exp(%d) index(%d) range(%d)\n", __func__, exposure->index, index, exposure->range);

	rc = mt9m114_write_reg(info->i2c_client,0x098E, 0x4940);
	if (rc < 0)
		return rc;

	// CAM_LL_GAMMA
	rc = mt9m114_write_reg(info->i2c_client,0xC940, gamma_table_sub[index]);
	if (rc < 0)
		return rc;
       // UVC_BRIGHTNESS_CONTROL
	rc = mt9m114_write_reg(info->i2c_client,0xCC0A, brightness_table[index]);
	if (rc < 0)
		return rc;

	prev_brightness_mode = index;

	return rc;
}

static int mt9m114_set_fpsrange(struct mt9m114_info *info, FpsRangeType *fpsRange)
{
	int err = 0;

	int lo = fpsRange->low;
	int hi = fpsRange->high;
	pr_info("%s:lo=%d, hi=%d\n", __func__, lo, hi);
	if (lo == hi){//fixed framerate
		if (lo==7){
			err =  mt9m114_write_table(info->i2c_client,mt9m114_Set_FramerateList[0],NULL,0);
		}
		else if (lo == 10){
			err =  mt9m114_write_table(info->i2c_client,mt9m114_Set_FramerateList[1],NULL,0);
		}
		else if (lo == 15){
			err =  mt9m114_write_table(info->i2c_client,mt9m114_Set_FramerateList[2],NULL,0);
		}

	}
	else{//variable framerate
		err = mt9m114_write_table(info->i2c_client,mt9m114_Set_FramerateList[3],NULL,0);
	}

	if(prev_brightness_mode != 6) // Set again when mode is not center
	{
		pr_info("### mt9m114_set_fpsrange: Set again when mode is not center, value is %d\n ",prev_brightness_mode);
		err = mt9m114_write_reg(info->i2c_client,0x337E, brightness_table[prev_brightness_mode]);
		if (err < 0)
			return err;

		err = mt9m114_write_reg(info->i2c_client,0xC940, gamma_table_sub[prev_brightness_mode]);
		if (err < 0)
			return err;
	}

	return err;
}
static int mt9m114_set_scene_mode(struct mt9m114_info *info, unsigned int scene_mode)
{
	int err = 0;
	pr_info("%s : %d\n", __func__, scene_mode);

	switch(scene_mode)
	{
		case YUVCamSceneMode_Auto :
			err = mt9m114_write_table(info->i2c_client, mt9m114_ScenMode_Auto,NULL,0);
			break;

		case YUVCamSceneMode_Night :
			err = mt9m114_write_table(info->i2c_client, mt9m114_ScenMode_Night,NULL,0);
			break;

		default :
			//err = hi702_write_table(info->i2c_client, hi702_ScenMode_Auto,NULL,0);

			break;
	}

    if(prev_brightness_mode != 6) // Set again when mode is not center
	{
		pr_info("### mt9m114_set_scene_mode: Set again when mode is not center, value is %d\n ",prev_brightness_mode);
		err = mt9m114_write_reg(info->i2c_client,0x337E, brightness_table[prev_brightness_mode]);
		if (err < 0)
			return err;

		err = mt9m114_write_reg(info->i2c_client,0xC940, gamma_table_sub[prev_brightness_mode]);
		if (err < 0)
			return err;
	}
	if (err !=0)
		pr_info("%s : Scene Mode : %d,  error : %d\n", __func__, scene_mode, err);

	return err;
}

#endif
//                                                                           
static int mt9m114_set_mode(struct mt9m114_info *info, struct mt9m114_mode *mode)
{
	int sensor_mode;
	int err;

	mt_info("%s: xres %u yres %u\n", __func__, mode->xres, mode->yres);
	if (mode->xres == 640 && mode->yres == 480)
		sensor_mode = MT9M114_MODE_680x480;
	else if (mode->xres == 1280 && mode->yres == 960)
		sensor_mode = MT9M114_MODE_680x480;
	else {
		mt_err("%s: invalid resolution supplied to set mode %d %d\n",
		       __func__, mode->xres, mode->yres);
		return -EINVAL;
	}

	mt_info("mt9m114: mt9m114_set_mode before write table");
	err = mt9m114_write_table(info->i2c_client, mode_table[sensor_mode],
		NULL, 0);
	if (err)
		return err;

	info->mode = sensor_mode;
	return 0;
}

static int mt9m114_get_status(struct mt9m114_info *info,
		struct mt9m114_status *dev_status)
{
	int err;

	err = mt9m114_write_reg(info->i2c_client, 0x98C, dev_status->data);
	if (err)
		return err;

	err = mt9m114_read_reg(info->i2c_client, 0x0990,
		(u16 *) &dev_status->status);
	if (err)
		return err;

	return err;
}

static long mt9m114_ioctl(struct file *file,
			 unsigned int cmd, unsigned long arg)
{
	int err;
	struct mt9m114_info *info = file->private_data;

	mt_info("%s, cmd : %d\n", __func__, cmd);

	switch (cmd) {
	case MT9M114_IOCTL_SET_MODE:
	{
		struct mt9m114_mode mode;
		if (copy_from_user(&mode,
				   (const void __user *)arg,
				   sizeof(struct mt9m114_mode))) {
			return -EFAULT;
		}

		mt_info("mt9m114: mt9m114_ioctl : MT9M114_IOCTL_SET_MODE");
		return mt9m114_set_mode(info, &mode);
	}
//                                                                           
#if 1
	case MT9M114_IOCTL_SET_COLOR_EFFECT :
	{
		unsigned int color_effect;

		if (copy_from_user(&color_effect,
					(const void __user *)arg,
					sizeof(unsigned int))) {
			return -EFAULT;
		}
		//mutex_lock(&mt9m114_camera_lock);
		err = mt9m114_set_color_effect(info, color_effect);
		//mutex_unlock(&mt9m114_camera_lock);
		pr_info("    :MT9M114_IOCTL_SET_COLOR_EFFECT(%d)\n", color_effect);
		return err;

	}
	case MT9M114_IOCTL_SET_WHITE_BALANCE :
	{
		unsigned int white_balance;

		if (copy_from_user(&white_balance,
					(const void __user *)arg,
					sizeof(unsigned int))) {
			return -EFAULT;
		}
		//mutex_lock(&mt9m114_camera_lock);
        err = mt9m114_set_white_balance(info, white_balance);
		//mutex_unlock(&mt9m114_camera_lock);
		pr_info("    :MT9M114_IOCTL_SET_WHITE_BALANCE(%d)\n", white_balance);
		return err;

	}

	case MT9M114_IOCTL_SET_EXPOSURE:
	{
		ExposureValueType exposure;

		if (copy_from_user(&exposure,
					(const void __user *)arg,
					sizeof(ExposureValueType))) {
			return -EFAULT;
		}
		//mutex_lock(&mt9m114_camera_lock);
        err = mt9m114_set_exposure(info, &exposure);
		//mutex_unlock(&mt9m114_camera_lock);
		pr_info("    :MT9M114_IOCTL_SET_EXPOSURE(%d/%d)\n", exposure.index, exposure.range);
		return err;

	}

	case MT9M114_IOCTL_SET_FPSRANGE:
	{
		FpsRangeType FpsRange;

		if (copy_from_user(&FpsRange,
					(const void __user *)arg,
					sizeof(FpsRangeType))) {
			return -EFAULT;
		}
		//mutex_lock(&mt9m114_camera_lock);
        err = mt9m114_set_fpsrange(info, &FpsRange);
		//mutex_unlock(&mt9m114_camera_lock);
		pr_info("    :MT9M114_IOCTL_SET_FPSRANGE(%d:%d)\n", FpsRange.low, FpsRange.high);
		return err;
	}
    case MT9M114_IOCTL_SET_SCENE_MODE:
	{
		unsigned int scene_mode;

		if (copy_from_user(&scene_mode,
					(const void __user *)arg,
					sizeof(unsigned int))) {
			return -EFAULT;
		}
		//mutex_lock(&mt9m114_camera_lock);
		err = mt9m114_set_scene_mode(info, scene_mode);
		//mutex_unlock(&mt9m114_camera_lock);
		pr_info("    :MT9M114_IOCTL_SET_SCENE_MODE(%d)\n", scene_mode);
		return err;
	}
#endif
//                                                                           
	default:
		mt_info("mt9m114: mt9m114_ioctl : default");
		return -EINVAL;
	}
	return 0;
}

#if 1 //defined(CONFIG_MACH_LGHDK_REV_P3) || defined(MACH_X3_REV_C)
#define SUB_CAM_RESET_N     221 //TEGRA_GPIO_PBB5
#define VT_1V8V_EN          194 //TEGRA_GPIO_PY2
#endif

static int mt9m114_power_on(void)
{
	pr_info(" mt9m114_power_on\n");

    //IOVDD ->DVDD->AVDD->Reset
	subpm_set_gpio(1);

    // IOVDD 1.8v ON
	gpio_direction_output(VT_1V8V_EN, 1); //1.8V, VIF =>should be first
	gpio_set_value(VT_1V8V_EN, 1);
	udelay(1000);

    //DVDD 1.8V ON
	subpm_set_output(LDO1,1);//1.2V, VDIG =>should be 1.8V and should be second
	udelay(1000);
	//AVDD 2.8V ON
	subpm_set_output(LDO2,1);//2.7V, VANA=>should be third
	udelay(1000);

	//Reset low->high
	gpio_direction_output(SUB_CAM_RESET_N, 0);
    udelay(10);
	gpio_set_value(SUB_CAM_RESET_N, 0);
	udelay(10);
	gpio_direction_output(SUB_CAM_RESET_N, 1);
	udelay(10);
	gpio_set_value(SUB_CAM_RESET_N, 1);
    udelay(100);

	return 0;
}

static int mt9m114_power_off(void)
{
	pr_info(" mt9m114_power_off\n");

    //AVDD 2.8V OFF
    subpm_set_output(LDO2,0);
	udelay(400);
	//DVDD 1.8V OFF
    subpm_set_output(LDO1,0);
	udelay(400);
    //IOVDD 1.8V OFF
    gpio_set_value(VT_1V8V_EN, 0);
    udelay(10);
    gpio_direction_output(VT_1V8V_EN, 0);
	udelay(100);

    //Reset low
	gpio_set_value(SUB_CAM_RESET_N, 0);
    udelay(10);
	gpio_direction_output(SUB_CAM_RESET_N, 0);
    udelay(100);

	subpm_set_gpio(0);
	return 0;
}

static int mt9m114_open(struct inode *inode, struct file *file)
{
	struct mt9m114_status dev_status;
	int err;

	mt_info("mt9m114: mt9m114_ioctl : mt9m114_open");
//                                          
#if defined(CONFIG_VU_1_0_GLOBAL)
	pr_info("%s\n", __func__);
#endif
//                                          

	file->private_data = info;
	if (info->pdata && info->pdata->power_on)
		info->pdata->power_on();
        else 
                mt9m114_power_on();

	dev_status.data = 0;
	dev_status.status = 0;
	err = mt9m114_get_status(info, &dev_status);
	return err;
}

int mt9m114_release(struct inode *inode, struct file *file)
{
	mt_info("mt9m114: mt9m114_ioctl : mt9m114_release");
	if (info->pdata && info->pdata->power_off)
		info->pdata->power_off();
	else
		mt9m114_power_off();

	file->private_data = NULL;
	return 0;
}

static const struct file_operations mt9m114_fileops = {
	.owner = THIS_MODULE,
	.open = mt9m114_open,
	.unlocked_ioctl = mt9m114_ioctl,
	.release = mt9m114_release,
};

static struct miscdevice mt9m114_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "mt9m114",
	.fops = &mt9m114_fileops,
};

>>>>>>> f720e99... drivers/: Merge LGE changes [2]
static int mt9m114_probe(struct i2c_client *client,
			const struct i2c_device_id *id)
{
	int err;
<<<<<<< HEAD
	struct mt9m114_info *info;
	dev_dbg(&client->dev, "mt9m114: probing sensor.\n");

	info = devm_kzalloc(&client->dev, sizeof(*info), GFP_KERNEL);
	if (info == NULL) {
		dev_err(&client->dev, "%s: kzalloc error\n", __func__);
		return -ENOMEM;
	}

	info->pdata = client->dev.platform_data;
	info->i2c_client = client;
	atomic_set(&info->in_use, 0);
	info->mode = NULL;

	i2c_set_clientdata(client, info);

	mt9m114_power_get(info);
	mt9m114_mode_info_init(info);

	memcpy(&info->miscdev_info,
		&mt9m114_device,
		sizeof(struct miscdevice));

	err = misc_register(&info->miscdev_info);
	if (err) {
		dev_err(&info->i2c_client->dev, "mt9m114: Unable to register misc device!\n");
		kfree(info);
		return err;
	}
#ifdef CONFIG_DEBUG_FS
	mt9m114_debug_init(info);
#endif
=======

	mt_info("mt9m114: probing sensor.\n");

	info = kzalloc(sizeof(struct mt9m114_info), GFP_KERNEL);
	if (!info) {
		mt_err("mt9m114: Unable to allocate memory!\n");
		return -ENOMEM;
	}

	err = misc_register(&mt9m114_device);
	if (err) {
		mt_err("mt9m114: Unable to register misc device!\n");
		kfree(info);
		return err;
	}

	info->pdata = client->dev.platform_data;
	info->i2c_client = client;

	i2c_set_clientdata(client, info);

	tegra_gpio_enable(VT_1V8V_EN);
	gpio_request(VT_1V8V_EN, "vt_1.8v_en");
	tegra_gpio_enable(SUB_CAM_RESET_N);
	gpio_request(SUB_CAM_RESET_N, "sub_cam_reset_n");
>>>>>>> f720e99... drivers/: Merge LGE changes [2]

	return 0;
}

static int mt9m114_remove(struct i2c_client *client)
{
	struct mt9m114_info *info;
	info = i2c_get_clientdata(client);
	misc_deregister(&mt9m114_device);
	kfree(info);
<<<<<<< HEAD

#ifdef CONFIG_DEBUG_FS
	if (info->debugfs_root)
		debugfs_remove_recursive(info->debugfs_root);
#endif

=======
>>>>>>> f720e99... drivers/: Merge LGE changes [2]
	return 0;
}

static const struct i2c_device_id mt9m114_id[] = {
	{ "mt9m114", 0 },
	{ },
};

MODULE_DEVICE_TABLE(i2c, mt9m114_id);

static struct i2c_driver mt9m114_i2c_driver = {
	.driver = {
		.name = "mt9m114",
		.owner = THIS_MODULE,
	},
	.probe = mt9m114_probe,
	.remove = mt9m114_remove,
	.id_table = mt9m114_id,
};

static int __init mt9m114_init(void)
{
<<<<<<< HEAD
	pr_info("mt9m114 sensor driver loading\n");
=======
	mt_info("mt9m114 sensor driver loading\n");
>>>>>>> f720e99... drivers/: Merge LGE changes [2]
	return i2c_add_driver(&mt9m114_i2c_driver);
}

static void __exit mt9m114_exit(void)
{
	i2c_del_driver(&mt9m114_i2c_driver);
}

module_init(mt9m114_init);
module_exit(mt9m114_exit);
<<<<<<< HEAD
MODULE_LICENSE("GPL v2");
=======
>>>>>>> f720e99... drivers/: Merge LGE changes [2]
