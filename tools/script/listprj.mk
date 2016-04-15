#################################################################
# rule
# path must be a directory, exist path/project.mk,path/filelist.mk
#
# pixxx=script/default
# pixxx_arg=[all,mlib,elf...] one of us, [all, mlib,elf...] is top path Makefile operation
	pi1=script/prj_produce
		pi1_arg=elf
	pi2=script/prj_test
		pi2_arg=elf
	pi3=script/prj_init
		pi3_arg=elf
	pi4=script/prj_lcd
		pi4_arg=elf
	pi5=script/prj_usb
		pi5_arg=elf
	pi6=script/prj_libgpio
		pi6_arg=mlib
	pi7=script/prj_libusb
		pi7_arg=mlib
	pi8=script/prj_qt
		pi8_arg=elf
	pi9=script/prj_log
		pi9_arg=elf
		
	# pi7=script/prj_tms_gpio
	# 	pi7_arg=kmod
# default project item
export DP=pi9
