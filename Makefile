#
# Makefile Template for "Application Program"
#
# Description:
#	Automatic load file config list "config_xxx_file_list.mk"
#	which will be complie,when use this Makefile Template,
#	you only edit the config_xxx_file_list.mk
#
#
# Author: Menglong Wu
# E-mail:DreagonWoo@163.com
#
#
# V0.1		2014-08-23
#		2 demo makefile AppMakefile.mk and LibMakefile.mk
# V0.2		2015-08-21

# V1.0
# - 2015-12-01,Menglong Wu,MenglongWoo@aliyun.com
#	- script/listprj.mk
#	- script/default/project.mk
#	- script/default/listprj.mk
SHELL=/bin/bash
export TOP_DIR = $(realpath ./)

#################################################################
# load common config
ifeq ("$(file_common)", "")
	ifeq ($(TOP_DIR)/script/common.mk, $(wildcard $(TOP_DIR)/script/common.mk))
		file_common = $(TOP_DIR)/script/common.mk
		include $(TOP_DIR)/script/common.mk
	else
		file_common = 
	endif
else
	include $(common)
endif

ifeq ("$(file_config)", "")
	ifeq ($(TOP_DIR)/.config, $(wildcard $(TOP_DIR)/.config))
		file_config = .config
		include $(file_config)
	else
		file_config = ========== no such file .config
	endif
else
	include $(file_config)
endif


#################################################################
# load all project items
# DP,ARG defined in listprj.mk
include script/listprj.mk
mconf=script/kconfig
	mconf_arg=mlib


ifeq ("$($(DP)_arg)", "")
$(warning  "project '$(DP)' unfind")
	exit
else
	ARG=$($(DP)_arg)
endif

#################################################################
# load default project configure xxx/project.mk
# load file list xxx/filelist.mk,SRCS-y defined in xxx/filelist.mk
file_list =$($(DP))/filelist.mk
file_prj  =$($(DP))/project.mk

# checking
ifeq ($(file_prj), $(wildcard $(file_prj)))
	include $(file_prj)
else
$(warning  "file_prj undefined")
	file_prj = 
	exit
endif

# checking
ifeq ($(file_list), $(wildcard $(file_list)))
	include $(file_list)
else
$(warning  "file_list undefined")
	 file_list = 
	 exit
endif


# checking
ifeq ("$(SRCS-y)", "")
$(warning  "SRCS-y is empty")
	exit
endif






#################################################################
# select which file be complie,it edit in config_app_file.mk
# Import all files,it edit in config_xxx_file_list.mk


ifeq ("$(file_lds)", "")
	ifeq ($(TOP_DIR)/script/default/boot.lds, $(wildcard $(TOP_DIR)/script/default/boot.lds))
		file_lds = $(TOP_DIR)/script/default/boot.lds
		# include $(file_lds)
	else
		file_lds = ========== no such file ./script/default/boot.lds 
	endif	
endif


#################################################################
OBJS = 	$(patsubst %.S,%.o,\
		$(patsubst %.cpp,%.o,\
		$(patsubst %.c,%.o,$(SRCS-y))))

#################################################################
# Output files name and directory
# 

ifeq ("$(OUTPUT_ELF)", "")
	OUTPUT_ELF	= download.elf
endif
ifeq ("$(OUTPUT_DIS)", "")
	OUTPUT_DIS	= download.dis
endif
ifeq ("$(OUTPUT_BIN)", "")
	OUTPUT_BIN	= download.bin
endif
ifeq ("$(OUTPUT_SO)", "")
	OUTPUT_SO	= download.so
endif
ifeq ("$(OUTPUT_A)", "")
	OUTPUT_A	= download.a
endif
ifeq ("$(OUTPUT_DIR)", "")
	OUTPUT_DIR	= release
endif

MAKE_DIR	+= include doxygen




#################################################################
# macro NOWTIME "yyyy-mm-dd_HH:MM:SS"
NOWTIME="$(shell date "+%Y-%m-%d_%H:%M:%S")"

#################################################################
# INCLUDE_DIR	- Where will be search *.h file
# LFLAGS		- Linking option
# LIB_DIR		- Where will be search *.so/*.a file
#-Wl,-rpath=./:./lib/

#when app.elf run will select *.so/a from $(PATH) -> ./ -> ./lib/
# INCLUDE_DIR += 
# LFLAGS	    += 
# LIB_DIR     += 
CFLAGS      += -DBUILD_DATE=\"$(NOWTIME)\"		\
		-DPRJ_VERSION=\"$(PRJ_VERSION)\"	\
		-DPRJ_PATCHLEVEL=\"$(PRJ_PATCHLEVEL)\"	\
		-DPRJ_SUBLEVEL=\"$(PRJ_SUBLEVEL)\"	\
		-DPRJ_NAME=\"$(PRJ_NAME)\"


#################################################################
GCC_G++ = gcc
CC 	= $(CROSS_COMPILE)$(GCC_G++)
LD 	= $(CROSS_COMPILE)ld
AR  = $(CROSS_COMPILE)ar
OBJDUMP = $(CROSS_COMPILE)objdump
OBJCOPY = $(CROSS_COMPILE)objcopy
STRIP = $(CROSS_COMPILE)strip

#################################################################
# CFLAGS		- Compile general option
# CC_FLAGS		- Compile only for *.c file option
# CS_FLAGS		- Compile only for *.S file option
CFLAGS		+= 	 -Wall  -rdynamic
ifeq ("$(GCC_G++)","gcc") # only compile gcc use -std=gnu99 option
	CC_FLAGS    = -std=gnu99
else
	CC_FLAGS    = 
endif



CC_FLAGS   += $(CFLAGS)
CS_FLAGS   += $(CFLAGS)

ifeq ("$(load_lds)","y") 
	load_lds = load_lds-y
else
	load_lds = load_lds-n
endif


#################################################################
NORMAL = "\e[0;1m"
BLACK = "\e[30;1m"
RED  =  "\e[31;1m"
GREEN = "\e[32;1m"
YELLOW = "\e[33;1m"
BLUE  = "\e[34;1m"
PURPLE = "\e[35;1m"
CYAN  = "\e[36;1m"
WHITE = "\e[37;1m"

#################################################################
# def target beyond DP,ARG
def:$(ARG)

# do something for all target
include script/allprj.mk

se:
	$(MAKE) -C ./ 2>&1 | grep error --color=auto -A 3
sw:
	$(MAKE) -C ./ 2>&1 | grep warning --color=auto -A 3
# list all project
lp:
	@cat script/listprj.mk | grep "=script/.\|arg=" | grep -v "#"
	@echo DP=$(DP)
ep:
	vi -o $(file_prj) $(file_list)
#################################################################
# 
one:echo-arch elf bin dis

#################################################################
# create autoconfig.h and directory
configure: init_dir mkheader
	echo $(file_config) include/autoconfig.h $(PRJ_NAME)
	@mkheader $(file_config) include/autoconfig.h $(PRJ_NAME)

menuconfig:mconf mkheader
	./script/kconfig/mconf Kconfig
	./script/mkheader/mkheader .config include/autoconfig.h $(PRJ_NAME)

mconf:
	$(MAKE) -C script/kconfig

mkheader:
	$(MAKE) -C script/mkheader

# 
dis:echo-arch elf
	@echo -e $(YELLOW)"    create     $(OUTPUT_DIR)-$(ARCH)/$(OUTPUT_DIS)"   			$(NORMAL)
	@$(OBJDUMP) -S $(OUTPUT_DIR)-$(ARCH)/$(OUTPUT_ELF) > $(OUTPUT_DIR)-$(ARCH)/$(OUTPUT_DIS)	

# create bin file, for system on chip without operating system
bin:echo-arch elf
	@echo -e $(YELLOW)"    create     $(OUTPUT_DIR)-$(ARCH)/$(OUTPUT_BIN)"   			$(NORMAL)
	@$(OBJCOPY) -O binary -S $(OUTPUT_DIR)-$(ARCH)/$(OUTPUT_ELF) $(OUTPUT_DIR)-$(ARCH)/$(OUTPUT_BIN)
#################################################################
# it's a ELF application program file on linux,*.lds is auto loaded 
# by system from default path
elf:echo-arch $(load_lds)


load_lds-n:$(OUTPUT_DIR)-$(ARCH) $(OBJS)
	@echo -e $(YELLOW)"    create     $(OUTPUT_DIR)-$(ARCH)/$(OUTPUT_ELF)" 				$(NORMAL)
	@$(CC) -o $(OUTPUT_DIR)-$(ARCH)/$(OUTPUT_ELF) $(OBJS) $(LIB_DIR) $(LFLAGS)

# it's a bootloader bin file,user have to select *.lds file by your self
# default file_lds = boot.lds
load_lds-y:$(OUTPUT_DIR)-$(ARCH) $(OBJS)

	@echo -e $(YELLOW)"    create     $(OUTPUT_DIR)-$(ARCH)/$(OUTPUT_ELF)"				$(NORMAL)
	@$(LD) -T$(file_lds) $(OBJS) -o $(OUTPUT_DIR)-$(ARCH)/$(OUTPUT_ELF) $(LFLAGS) $(LIB_DIR)  
#################################################################
.PHONY: mlib
mlib:echo-arch  $(OUTPUT_DIR)-$(ARCH) $(OBJS)
	@echo -e $(YELLOW)"    create     $(OUTPUT_DIR)-$(ARCH)/$(OUTPUT_SO)"				$(NORMAL)
	@$(CC) -shared -fPIC -o $(OUTPUT_DIR)-$(ARCH)/$(OUTPUT_SO) $(OBJS)
	@echo -e $(YELLOW)"    create     $(OUTPUT_DIR)-$(ARCH)/$(OUTPUT_A)"				$(NORMAL)
	@$(AR) rcs $(OUTPUT_DIR)-$(ARCH)/$(OUTPUT_A) $(OBJS)
#################################################################
echo-arch:
	@echo -e $(YELLOW)"    ARCH       [$(ARCH)]"										$(NORMAL)
%.o:%.c
	@echo -e $(CYAN)"    compile    $^"													$(NORMAL)
	@$(CC) -o $@ -c $^ $(CC_FLAGS) $(INCLUDE_DIR)
%.o:%.cpp
	@echo -e $(CYAN)"    compile    $^"													$(NORMAL)
	@$(CC) -o $@ -c $^ $(CC_FLAGS) $(INCLUDE_DIR) 
%.o:%.S
	@echo -e $(CYAN)"    compile    $^"													$(NORMAL)
	@$(CC) -o $@ -c $^ $(CS_FLAGS) $(INCLUDE_DIR)



#################################################################
# make directory
$(OUTPUT_DIR)-$(ARCH):
	@mkdir $(OUTPUT_DIR)-$(ARCH)

# .PHONY:$(MAKE_DIR)
init_dir:$(CHECK_TARGET) $(MAKE_DIR)

CHECK_TARGET:
ifneq ("$(ARCH)","$(filter $(SUPPORT_TARGET), $(ARCH))") 
$(error  "unsupport target [$(ARCH)]")
endif

$(MAKE_DIR):
	@mkdir $@



#################################################################
.PHONY: aclean
aclean:
	@-rm -f $(OBJS)  \
		$(OUTPUT_DIR)-$(ARCH)/$(OUTPUT_DIS) \
		$(OUTPUT_DIR)-$(ARCH)/$(OUTPUT_ELF) \
		$(OUTPUT_DIR)-$(ARCH)/$(OUTPUT_BIN) \
		$(OUTPUT_DIR)-$(ARCH)/$(OUTPUT_SO) \
		$(OUTPUT_DIR)-$(ARCH)/$(OUTPUT_A)
	@-rm -f core

.PHONY: adistclean
adistclean:aclean
	@rmdir $(OUTPUT_DIR)-$(ARCH) --ignore

#################################################################

astrip:strip-$(ARG)

strip-elf:
	@echo -e $(YELLOW)"    strip     $(OUTPUT_DIR)-$(ARCH)/$(OUTPUT_ELF)"				$(NORMAL)
	@$(STRIP) $(OUTPUT_DIR)-$(ARCH)/$(OUTPUT_ELF)

strip-mlib:
	@echo -e $(YELLOW)"    strip     $(OUTPUT_DIR)-$(ARCH)/$(OUTPUT_SO)"				$(NORMAL)
	@$(STRIP) $(OUTPUT_DIR)-$(ARCH)/$(OUTPUT_SO)
	@echo -e $(YELLOW)"    strip     $(OUTPUT_DIR)-$(ARCH)/$(OUTPUT_A)"					$(NORMAL)
	@$(STRIP) $(OUTPUT_DIR)-$(ARCH)/$(OUTPUT_A) 



#################################################################
# copy/install output file to other directory
acopy:acopy_$(ARG)

acopy_elf:acopy_all
acopy_all:
	cp $(OUTPUT_DIR)-$(ARCH)/$(OUTPUT_ELF) /work/armdebug/
acopy_bin:
	cp $(OUTPUT_DIR)-$(ARCH)/$(OUTPUT_BIN) /work/armdebug/
acopy_mlib:
	cp $(OUTPUT_DIR)-$(ARCH)/$(OUTPUT_SO) /work/armdebug/$(OUTPUT_DIR)-$(ARCH)
	cp $(OUTPUT_DIR)-$(ARCH)/$(OUTPUT_A) /work/armdebug/$(OUTPUT_DIR)-$(ARCH)




run:
	./$(OUTPUT_DIR)-$(ARCH)/$(OUTPUT_ELF)

gdb:
	gdb ./$(OUTPUT_DIR)-$(ARCH)/$(OUTPUT_ELF)
gdb-core:
	gdb ./$(OUTPUT_DIR)-$(ARCH)/$(OUTPUT_ELF) core


print_env:
	@echo =========================================================

	@echo PRJ_VERSION "  "= $(PRJ_VERSION)
	@echo PRJ_NAME "     "= $(PRJ_NAME)

	@echo 
	@echo file_common "  "= $(file_common)
	@echo file_prj "     "= $(file_prj)
	@echo file_config "  "= $(file_config)
	@echo file_list "    "= $(file_list)
	@echo file_lds "     "= $(file_lds)

	@echo 
	@echo CROSS_COMPILE = $(CROSS_COMPILE)
	@echo CC "           "= $(CC)
	@echo LD "           "= $(LD)
	@echo OBJDUMP "      "= $(OBJDUMP)
	@echo OBJCOPY "      "= $(OBJCOPY)

	@echo 
	@echo OUTPUT_ELF "   "= $(OUTPUT_ELF)
	@echo OUTPUT_DIS "   "= $(OUTPUT_DIS)
	@echo OUTPUT_BIN "   "= $(OUTPUT_BIN)
	@echo OUTPUT_SO "    "= $(OUTPUT_SO)
	@echo OUTPUT_A "     "= $(OUTPUT_A)
	@echo OUTPUT_DIR "   "= $(OUTPUT_DIR)

	@echo 
	@echo INCLUDE_DIR " " = $(INCLUDE_DIR)
	@echo LFLAGS "      " = $(LFLAGS)
	@echo LIB_DIR "     " = $(LIB_DIR)
	@echo CFLAGS "      " = $(CFLAGS)

	@echo $(SRCS-y)
help:
	@echo ======================== Makefile help ========================
	@echo "    "configure"    "make autoconfig.h from config file default config.mk
	@echo "    "all"          "compil all sub project
	@echo "    "clean"        "clean output file depend on *.o and OUTPUT_xxx\(OUTPUT_ELF and so on\)
	@echo "    "distclean"    "clean all ourput file and directory
	@echo "    "strip"        "remove section
	@echo "    "copy"         "copy output file to default directory

	@echo "    "lp"           "list project
	@echo "    "mlib"         "create *.so, *.a
	@echo "    "print_env"    "display environment,only for debug Makefile
	@echo "    "run"          "run *elf 
	@echo "    "gdb"          "gdb debug
	@echo "    "gdb-core"     "gdb debug and load core
	@echo 
	@echo "Environment"
	@echo "    "CONFIG_XXX"   "select module be compiled,define on config.mk,
	@echo "                   "CONFIG_MODULE = y
	@echo "                   "CONFIG_ENV      = "abc"
	@echo "    "SRCS-y"       "select file be compiled
	@echo "                   "SRCS-y += src/main.c src/foo.c
	@echo "                   "SRCS-\(CONFIG_MODULE\) += mod/module.c
	@echo "    "PRJS"         "sub project list
	@echo "                   "PRJS += pix piy piz
	@echo "                   "compile 3 project one by one

# user define

rmdb:
	@-rm /etc/xx.db
sqlite3:
	sqlite3 /etc/xx.db

splint:
	@echo TODO ...
	















#################################################################
# all sub project

each-all       := $(foreach n,$(PRJS),all-$(n))
each-clean     := $(foreach n,$(PRJS),clean-$(n))  clean-mconf
each-distclean := $(foreach n,$(PRJS),distclean-$(n))
each-strip     := $(foreach n,$(PRJS),strip-$(n))
each-copy      := $(foreach n,$(PRJS),copy-$(n))


.PHONY:all
all:$(each-all)
$(each-all):
	$(MAKE) DP=$(patsubst all-%,%,$@) --no-print-directory

# clean all project output
.PHONY:clean
clean:$(each-clean)
	@$(MAKE) clean -C script/kconfig  --no-print-directory
	@$(MAKE) clean -C script/mkheader  --no-print-directory
$(each-clean):
	@$(MAKE) DP=$(patsubst clean-%,%,$@) aclean --no-print-directory
	
# echo DP=$@
# remote all output file and empty directory which create by Makefile
.PHONY:distclean
distclean:$(each-distclean)
$(each-distclean):
	@$(MAKE) DP=$(patsubst distclean-%,%,$@) adistclean --no-print-directory

# strip all output file STRTAB section
.PHONY:strip
strip:$(each-strip)
$(each-strip):
	$(MAKE) DP=$(patsubst strip-%,%,$@) astrip --no-print-directory

# copy all output file
.PHONY:copy
copy:$(each-copy)
$(each-copy):
	@$(MAKE) DP=$(patsubst copy-%,%,$@) acopy --no-print-directory
	


# End all sub project
