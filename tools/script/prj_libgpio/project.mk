
PRJ_VERSION = "1.0.0"
PRJ_NAME = "libgpio"

ARCH=armv7

# ifeq ("$(ARCH)", "x86")
# CROSS_COMPILE = 
# endif
# ifeq ("$(ARCH)", "armv7")
# CROSS_COMPILE	=/opt/iTop-4412/4.3.2/bin/arm-linux-
# endif
# default download.elf,download.dis,download.bin
OUTPUT_ELF	= 
OUTPUT_DIS	=
OUTPUT_BIN	= 
OUTPUT_SO 	= libtms_gpio.so
OUTPUT_A	= libtms_gpio.a

OUTPUT_DIR = lib


# 

# file_list = $(TOP_DIR)/script/prj_usb/filelist.mk


INCLUDE_DIR	+= -I../tms_gpio/src -I./driver
LFLAGS		+= 
LIB_DIR 	+= 
CFLAGS      += -std=gnu99 -g  	 -Wall
