/**
 ******************************************************************************
 * @file	example/t-log.c
 * @brief	ramlog 简单使用例子

演示如何使用ramlog

程序启动后创建ramlog子进程，父子进程之间共享内存

$ ./t-log &
$ ps
  PID TTY          TIME CMD
 1343 pts/5    00:00:00 t-log.elf
 1345 pts/5    00:00:00 t-log.elf

主程序向缓存里里写入日志，此时日志并没有真正写入存储器，程序在即将推出前暂停10秒，
日志目录里看不到任何新内容，
$ ls log/t-log 
ls: cannot access ./log/t-log: No such file or directory

主程序整个运行期间不做任何保存日志操作。在主程序退出后由ramlog子进程自动将日志保存
$ ls ./log/t-log
mylog-20161202-14-27-11.0.log

 *
 @section Platform
	-# 
 @section Library
	-# libramlog.so libpthread.so
- 2016-12-2,Menglong Woo,MenglongWoo@aliyun.com
 	- 

*/

#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <stdbool.h>
#include <stdlib.h>

#include "ramlog.h"

#define _1K (1024)
#define _1M (1024*_1K)

int main(int argc, char **argv)
{

	printf("ppid %d pid %d tid %d\n", getppid(), getpid(), syscall(SYS_gettid));

	rl_prefix("mylog");
	rl_path("./log/t-log/");

	printf("Create [ramlog] sub process\n");
	rl_clone();


	rl_log("1111111111");
	rl_log("2222222222");

	printf("Program [%s] will be exit after 10 second\n", argv[0]);
	printf("Current don't create new log files\n");

	printf("---------------------------------\n");
	printf("$ ls ./log/t-log/\n");
	system("ls ./log/t-log/");
	sleep(10);
	printf("main end\n");
	return 0;
}