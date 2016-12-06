/**
 ******************************************************************************
 * @file	example/t-type.c
 * @brief	ramlog 日志消息分类

演示各种消息类型的输出在什么情况下使用

 *
 @section Platform
	-# 
 @section Library
	-# libramlog.so libpthread.so
- 2016-12-4,Menglong Woo,MenglongWoo@aliyun.com
 	- 

*/

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <unistd.h>

#define CONFIG_LOG_TIMESTAMP 1
#define CONFIG_LOG_TRACE 1

#include "ramlog.h"




#define _1K (1024)
#define _1M (1024*_1K)

int main(int argc,char **argv)
{	
	int input = 0;
	int divisor = 0;

	printf("Hello !!!\n");
	LOGI("INFOR run start");

	rl_prefix("mylog");
	rl_path("./log/t-type/");
	rl_clone();
	LOGD("DEBUG call rl_clone()");
	
	
	printf("Are you Man, Woman or ga... (M/W/G): ");
	scanf("%c", &input);
	LOGT("TRACE input %c", input);
	switch(input) {
	case 'm':
	case 'M':
		printf("Welcome gentleman\n");
		LOGD("DEBUG case m:");
		divisor = 1;
		break;
	case 'w':
	case 'W':
		printf("Welcome lady\n");
		LOGD("DEBUG case w:");
		divisor = 2;
		break;
	case 'g':
	case 'G':
		printf("Are you kid me?\n");
		LOGD("DEBUG case g:");
		LOGW("WARN gay");
		divisor = 3;
		break;
	default:
		printf("input error!!!\n");
		LOGD("DEBUG case default:");
		LOGE("ERROR unknow");
		divisor = 0;
		break;
	}

	printf("program end\n");
	LOGI("INFOR program end\r\n");	
	
	return 0;
}
