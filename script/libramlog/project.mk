
PRJ_NAME = "ramlog"
PRJ_VERSION    = "1"
PRJ_PATCHLEVEL = "0"
PRJ_SUBLEVEL   = "0"


# default download.elf,download.dis,download.bin
OUTPUT_ELF	= ramlog.elf
OUTPUT_DIS	=
OUTPUT_BIN	= 
OUTPUT_SO 	= libramlog.so
OUTPUT_A	= libramlog.a
OUTPUT_DIR = lib




INCLUDE_DIR	+= -I./ -I./include -I./src 
LFLAGS		+= -lpthread
LIB_DIR 	+= 
CFLAGS      += -std=gnu99  	 -Wall  -O3
#-D_DEBUG
