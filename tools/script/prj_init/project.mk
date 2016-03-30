
PRJ_VERSION = "1.0.0"
PRJ_NAME = "libtms_gpio"




ifeq ("$(ARCH)", "x86")
CROSS_COMPILE = 
endif
ifeq ("$(ARCH)", "armv7")
CROSS_COMPILE	=/opt/iTop-4412/4.3.2/bin/arm-linux-
endif

# default download.elf,download.dis,download.bin
OUTPUT_ELF	= initgpio.elf
OUTPUT_DIS	=
OUTPUT_BIN	= 
OUTPUT_SO 	= 
OUTPUT_A	=

OUTPUT_DIR = release


# 




INCLUDE_DIR	+= -I./ -I../tms_gpio/src -I./driver -I./src
LFLAGS		+= -ltms_gpio -lreadline -lhistory -lpthread   -ltermcap 
LIB_DIR 	+= 
CFLAGS      += -std=gnu99 -g  	 -Wall
