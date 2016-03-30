PRJ_VERSION = 1
PRJ_PATCHLEVEL = 2
PRJ_SUBLEVEL = 0
PRJ_NAME = "producetest"

ARCH=armv7

ifeq ("$(ARCH)", "x86")
CROSS_COMPILE = 
endif
ifeq ("$(ARCH)", "armv7")
CROSS_COMPILE	=/opt/iTop-4412/4.3.2/bin/arm-linux-
endif
# default download.elf,download.dis,download.bin
OUTPUT_ELF	= producetest.elf
OUTPUT_DIS	=
OUTPUT_BIN	= 
OUTPUT_SO 	= 
OUTPUT_A	=

OUTPUT_DIR = release


# 




INCLUDE_DIR	+= -I./ -I../tms_gpio/src  -I/usr/4412/install/include -I./src -I./driver
LFLAGS		+= -ltms_gpio -lreadline -lhistory -ltermcap
#-lncurses
LIB_DIR 	+= 
CFLAGS      += -std=gnu99 -g  	 -Wall
