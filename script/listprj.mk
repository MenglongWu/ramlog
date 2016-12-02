#################################################################
# rule
# path must be a directory, exist path/project.mk,path/filelist.mk
#
# pixxx=script/default
# pixxx_arg=[all,mlib,elf...] one of us, [all, mlib,elf...] is top path Makefile operation
	pi9=script/libramlog
		pi9_arg=mlib
	pi10=script/test-speed
		pi10_arg=elf
	pi11=script/test-dump
		pi11_arg=elf
	pi12=script/test-log
		pi12_arg=elf
	pi13=script/test-logring
		pi13_arg=elf
	pi14=script/test-tar
		pi14_arg=elf
		

# default project item
export DP=pi14
