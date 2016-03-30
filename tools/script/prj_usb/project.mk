
PRJ_VERSION = "1.0.0"
PRJ_NAME = "usbcard"

ARCH=armv7

# ifeq ("$(ARCH)", "x86")
# CROSS_COMPILE = 
# endif
# ifeq ("$(ARCH)", "armv7")
# CROSS_COMPILE	=/opt/iTop-4412/4.3.2/bin/arm-linux-
# endif
# default download.elf,download.dis,download.bin
OUTPUT_ELF	= usbcard.elf
OUTPUT_DIS	=
OUTPUT_BIN	= 
OUTPUT_SO 	= 
OUTPUT_A	=

OUTPUT_DIR = release


# 

# file_list = $(TOP_DIR)/script/prj_usb/filelist.mk


INCLUDE_DIR	+= -I./src
LFLAGS		+= 
LIB_DIR 	+=-l_easydaqcard_dll 
CFLAGS      += -std=gnu99 -g  	 -Wall 
