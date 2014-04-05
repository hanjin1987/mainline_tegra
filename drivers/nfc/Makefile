#
# Makefile for nfc devices
#

obj-$(CONFIG_PN544_NFC)                += pn544.o
obj-$(CONFIG_NFC_PN533)                += pn533.o

ccflags-$(CONFIG_NFC_DEBUG) := -DDEBUG

obj-$(CONFIG_LGE_NFC)	:= pn544_lge.o
obj-$(CONFIG_LGE_NFC)	+= pn544_lge_hwadapter.o
