/**
 ******************************************************************************
 * @file	example/t-dump.c
 * @brief	ramlog dump 使用例子

演示 ramlog 主程序随时保持日志

主进程向 ramlog 缓存写入日志，以 rl_logring 方式重复使用缓存的方式不会触发自动
保存。

使用 rl_log 方式在缓存使用完后自动触发保存日志。保存文件名序号依次递增

 *
 @section Platform
	-# 
 @section Library
	-# libramlog.so libpthread.so
- 2016-12-2,Menglong Woo,MenglongWoo@aliyun.com
 	- 

*/

#include <stdio.h>
#include "ramlog.h"

#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <stdbool.h>

#define _1K (1024)
#define _1M (1024*_1K)

int main()
{
	rl_resize(50, _1M * 16);
	rl_prefix("mylog");
	rl_path("./log/t-dump/");
	rl_clone();


	rl_logring("1111111111");
	rl_logring("2222222222");
	rl_logring("3333333333");
	rl_logring("4444444444");
	rl_logring("5555555555"); //  11111111112222222222333333333344444444445555555555. <--- 空间不足不能插入末尾的 '.'，缓存从头开始利用
				  //  5555555555.22222222233333333334444444444.555555555       处理后的结果
	rl_log("6666666666");
	rl_log("7777777777");
	rl_log("8888888888");	
	rl_log("9999999999");	  //  5555555555666666666677777777778888888888	<--- mylog-xxx.0.log
	rl_log("AAAAAAAAAA");
	rl_log("bbbbbbbbbb");
	rl_log("cccccccccc");	  //  9999999999AAAAAAAAAAbbbbbbbbbbcccccccccc  <--- mylog-xxx.1.log
	rl_log("dddddddddd");	  //  dddddddddd				<--- mylog-xxx.2.log
	printf("main end\n");
	return 0;
}