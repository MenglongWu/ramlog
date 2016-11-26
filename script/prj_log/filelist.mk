# # 最小系统代码
SRCS-y += src/ramlog.c

ifeq ("$(pi9_arg)", "elf") 
SRCS-y += src/testramlog.c
endif