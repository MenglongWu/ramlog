
PRJ_VERSION = "1.0.0"
PRJ_NAME = "lcdtest"

# ARCH=armv7

# ifeq ("$(ARCH)", "x86")
# CROSS_COMPILE = 
# endif
# ifeq ("$(ARCH)", "armv7")
# CROSS_COMPILE	=/opt/iTop-4412/4.3.2/bin/arm-linux-
# endif

# ifeq ("$(ARCH)", "arm920t")
# CROSS_COMPILE	=/opt/EmbedSky/4.3.3/bin/arm-linux-
# endif
# default download.elf,download.dis,download.bin
OUTPUT_ELF	= lcdtest.elf
OUTPUT_DIS	=
OUTPUT_BIN	= 
OUTPUT_SO 	= 
OUTPUT_A	=

OUTPUT_DIR = release


# 

# file_list = config_produce_file_list.mk
# SRCS-y += src/lcdtest.c 



INCLUDE_DIR	+= -I./ -I../tms_gpio/src  -I/usr/4412/install/include -I./src
LFLAGS		+= 
ifeq ("$(ARCH)", "x86")
LIB_DIR 	+= -L/usr/local/install/lib
endif
ifeq ("$(ARCH)", "armv7")
LIB_DIR 	+= -L/usr/4412/install/lib 
LIB_DIR 	+= -L./release
endif
CFLAGS      += -std=gnu99 -g  	 -Wall
