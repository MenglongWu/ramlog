
PRJ_VERSION = "1.0.0"
PRJ_NAME = "qttest"

ARCH=x86

ifeq ("$(ARCH)", "x86")
CROSS_COMPILE = 
endif
ifeq ("$(ARCH)", "armv7")
CROSS_COMPILE	=/opt/iTop-4412/4.3.2/bin/arm-linux-
endif
# default download.elf,download.dis,download.bin
OUTPUT_ELF	= qt.elf
OUTPUT_DIS	=
OUTPUT_BIN	= 
OUTPUT_SO 	= 
OUTPUT_A	=

OUTPUT_DIR = release


# 


INCLUDE_DIR	+= -I./ -I../tms_gpio/src  -I/usr/4412/install/include -I./src -I./driver \
				-I/opt/iTop-4412/qt-everywhere-opensource-src-4.7.1/qt-everywhere-opensource-src-4.7.1/include \
				-I/opt/iTop-4412/qt-everywhere-opensource-src-4.7.1/qt-everywhere-opensource-src-4.7.1/include/Qt \
				-I/opt/iTop-4412/qt-everywhere-opensource-src-4.7.1/qt-everywhere-opensource-src-4.7.1/include/QtGui \
				-I/opt/iTop-4412/qt-everywhere-opensource-src-4.7.1/qt-everywhere-opensource-src-4.7.1/include/QtCore
LFLAGS		+= 
# -lQtGui -lQtCore -lQtNetwork
LIB_DIR 	+= -L/work/iTop-4412/Linux_QT_TMSxx_utf-8/root/opt/qt-4.7.1/lib
CFLAGS      += -std=gnu99 -g  	 -Wall