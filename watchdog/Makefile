ifneq ($(KERNELRELEASE),)
obj-m := Exynos4412_wdt.o 
else	
PWD  := $(shell pwd)
KVER = 3.0.0
KDIR := /work/iTop-4412/iTop4412_Kernel_3.0
all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
endif


