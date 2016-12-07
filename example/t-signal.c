/**
 ******************************************************************************
 * @file	example/t-signal.c
 * @brief	ramlog 信号触发保存

外部信号触发ramlog保存日志
主进程底频度使用缓存，长时间不让缓存填满，日志保存时间后延。
外部进程向ramlog发送 SIGUSR1 信号触发保存。
保存位置 ./log/t-signal

$ ./release-x86/t-signal.elf 

查看PID，第二个是ramlog的PID
$ ps -aux | grep elf
Warning: bad ps syntax, perhaps a bogus '-'? See http://procps.sf.net/faq.html
root     22261  0.0  0.0   2096   580 pts/1    S+   17:38   0:00 ./release-x86/t-signal.elf
root     22262  0.0  0.0   2096   580 pts/1    S+   17:38   0:00 ./release-x86/t-signal.elf
root     22264  0.0  0.0   4012   768 pts/2    S+   17:38   0:00 grep --color=auto elf

向ramlog发送 USR1 信号
 kill -USR1 22262

查看日志内容
 $ cat log/t-signal/mylog-20161206-17-38-58.0.log   
new line 0
new line 1
new line 2
new line 3
new line 4

 *
 @section Platform
	-#
 @section Library
	-# libramlog.so libpthread.so
- 2016-12-6,Menglong Woo,MenglongWoo@aliyun.com
 	-

*/

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <unistd.h>
#include "ramlog.h"



#define _1K (1024)
#define _1M (1024*_1K)

int main()
{
	int      len      = _1K * 128;
	char    *data     = (char *)malloc(len);
	int      index    = 0;



	rl_resize(len, _1M * 4);	// 缓存大小 128KB，限制日志目录容量 4MB
	rl_prefix("mylog");
	rl_path("./log/t-signal/");
	// todo 检查时间配置
	rl_clone();


	bzero(data, len);
	int i = 0;
	while(1) {
		index = i % 10;
		snprintf(data, len,  "new line %d\r\n", i);
		printf("%s", data);
		rl_log(data);
		sleep(1);
		i++;
	}
	return 0;
}