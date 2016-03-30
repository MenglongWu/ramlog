##================================================================
##     Davicom Semiconductor Inc.  	For DM9620 V0.00
##   --------------------------------------------------------
## Description:
##              Compile driver dm9620.c to dm9620.ko
##
## Modification List:
## 09/05/2000	Fixed SMPFALGS wrong on smp & smp_mod
## 08/02/2000	Changed some description string & include file path
## 07/25/2000	Append smp_mod and changed some descriptions
## 01/25/2000	by Sten Wang
## 03/24/2009   Modifiy for Linux kernel 2.6.28
##================================================================
# Comment/uncomment the following line to disable/enable debugging
# DEBUG = y

# Add your debugging flag (or not) to CFLAGS
#ifeq ($(DEBUG),y)
#  DEBFLAGS = -O -g # "-O" is needed to expand inlines
#else
#  DEBFLAGS = -O2
#endif

#CFLAGS += $(DEBFLAGS) -I$(LDDINCDIR)

#ifneq ($(KERNELRELEASE),)
# call from kernel build system



obj-m	:= lcdsw.o
# unexport 
CPU=armv7

ifeq ("$(CPU)", "arm920t")
	KERNELDIR = /opt/EmbedSky/linux-2.6.30.4
endif

ifeq ("$(CPU)", "armv7")
	KERNELDIR = /work/iTop-4412/iTop4412_Kernel_3.0
#	KERNELDIR = /opt/iTop-4412/iTop4412_kernel_public-kernel_pop

endif


# KBUILD_CFLAGS +=  -DCONFIG_BOARD=12

PWD       := $(shell pwd)
KBUILD_CFLAGS += -std=gnu99 \
		-I/mnt/cifs/linuxproject/TMSxxTC/kit-TMSxxTC/tools/include


all:
	echo $(KBUILD_CFLAGS)
	$(MAKE) -C $(KERNELDIR) M=$(PWD)

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean



# obj-m	:=   tms_gpio.o

# #else
# MODULE_INSTALDIR ?= /lib/modules/$(shell uname -r)/kernel/drivers/net/usb
# # KERNELDIR ?= /lib/modules/$(shell uname -r)/build
# KERNELDIR = /opt/iTop-4412/iTop4412_Kernel_3.0
# PWD       := $(shell pwd)

# # CROSS_COMPILE	=/opt/iTop-4412/4.3.2/bin/arm-linux-
# ELFCC 	= /opt/iTop-4412/4.3.2/bin/arm-linux-gcc

# # Menglong Woo
# # KBUILD_CFLAGS   += -Wno-unused-result \
# # 	           -Wno-return-type
# KBUILD_CFLAGS += -std=gnu99 -DCONFIG_BOARD=12
# # end Menglong Woo


# default:
# 	$(MAKE) -C $(KERNELDIR) M=$(PWD) 

# #endif

# clean:
# 	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
# 	# -rm -rf src/*.o src/*~ core src/.depend src/*.cmd src/*.ko src/*.mod.c .tmp_versions

# install:
# 	#modprobe -r dm9620
# 	# install -c -m 0644 dm9620.ko $(MODULE_INSTALDIR)
# 	# depmod -a -e
# 	install -c -m 0644 itop4412_leds.ko /lib/modules/3.0.15-Hello_KuGo/
	# depmod -a -e

copy:
	cp *.ko /usr/armdebug
