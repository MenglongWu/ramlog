
PRJ_NAME = "ramlog"
PRJ_VERSION    = "1"
PRJ_PATCHLEVEL = "0"
PRJ_SUBLEVEL   = "0"


# default download.elf,download.dis,download.bin
OUTPUT_ELF	= test-ramlog.elf
OUTPUT_DIS	=
OUTPUT_BIN	= 
OUTPUT_SO 	= 
OUTPUT_A	= 
OUTPUT_DIR = release




INCLUDE_DIR	+= -I./ -I./include -I./src 
LFLAGS		+= -lramlog
LIB_DIR 	+= 
CFLAGS      += -std=gnu99  	 -Wall  -g
#-D_DEBUG
