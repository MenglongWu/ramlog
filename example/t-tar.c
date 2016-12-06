/**
 ******************************************************************************
 * @file	example/t-tar.c
 * @brief	ramlog 压缩日志

演示 ramlog 压缩日志功能
目前该功能需要外部调用shell，系统需要支持如下命令，以及命令选项

cd		进入目录
ls -1rt *.tar 	以时间顺序排序文件
tail -n 1	显示末端1行
xargs		传递管道参数
rm		删除文件
tar -czf	压缩日志
grep -v		查询过滤
du -b		查询文件夹下消耗空间


ramlog 缓存大小 128KB，限制日志目录容量 4MB，可容纳 32 个日志文件

应用程序设置ramlog进程每 5s （演示需要） 检查一次日志目录使用情况
每秒产生 128KB 的日志并写入存储器（实际使用不可能如此频繁），
每创建 12 个日志促使 ramlog 进程对目录进行压缩操作

值得注意的是，需要保证ramlog定期检查日志容量时间（deta）间隔小于目录容量被填满时间(tfull)。
例子里分别是 deta = 5s、tfull = 12s。
deta 过小影响系统性能，默认 deta = 30 min



自动压缩机制：
	1. 当前日志目录容量 + 20 * 缓存 >= 限制日志目录容量，触发压缩
	2. 将目录下所有本应用程序产生的日志进行打包处理，生成mylog.1.tar，tar序号依次递增
	3. 当tar文件容量也将目录容量使用如同 1. 一样，执行删除 老旧的 tar
	4. 按时间顺序排列保留最新的两个 tar 

 *
 @section Platform
	-# 
 @section Library
	-# libramlog.so libpthread.so
- 2016-12-2,Menglong Woo,MenglongWoo@aliyun.com
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
	char    *data     = (char*)malloc(len);
	int      index    = 0;



	rl_resize(len, _1M * 4);	// 缓存大小 128KB，限制日志目录容量 4MB
	rl_prefix("mylog");
	rl_path("./log/t-tar/");
	// todo 检查时间配置
	rl_clone();

	
	bzero(data, len);
	for (int i = 0;i < 1000; i++) {
		index = i % 10;
		memset(data, '1' + index, len  - 1);
		rl_log(data);
		sleep(1);
	}
	return 0;
}