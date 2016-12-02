/**
 ******************************************************************************
 * @file	example/t-logring.c
 * @brief	ramlog rl_logring 使用例子

演示 ramlog 环形缓存，

限制日志缓存只有 50 Byte，当线性缓存被使用完毕后从头循环使用，在主程序结束后
只将最后在缓存内容写入日志。

ramlog 最初的设计目的：
	1. 减小存储器的写入频度，以内存换取效率
	2. 只在乎程序崩溃附加的执行逻辑
	3. 延长存储器的使用寿命

本测试代码遵循目的 2.

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
#include <stdbool.h>

#define _1K (1024)
#define _1M (1024*_1K)

int main()
{
	rl_resize(50, _1M * 16); // 日志缓存限制成 50 Byte
	rl_prefix("mylog");
	rl_path("./log/t-logring/");
	rl_clone();

	/*
	 每次输入字符串长度为10的字符，字符末尾为 '\0'，
	 下一次插入位置在上一次结束符 '\0' 开始处，
	 连续插入 2 个长度为 10 的字符串，实际占用空间 21 Byte
	*/
				  // 内存结构，如图描述50 Byte 缓存使用情况，其中末尾的 '.'
				  // 表示字符串的结束符 '\0'
				  //  0--------1---------2---------3---------4---------5
	rl_logring("1111111111"); //  1111111111.                                       
	rl_logring("2222222222"); //  11111111112222222222.                             
	rl_logring("3333333333"); //  111111111122222222223333333333.                   
	rl_logring("4444444444"); //  1111111111222222222233333333334444444444.           <--- 此时使用空间 40 byte
	rl_logring("5555555555"); //  11111111112222222222333333333344444444445555555555. <--- 空间不足不能插入末尾的 '.'，缓存从头开始利用
				  //  5555555555.22222222233333333334444444444.555555555       处理后的结果
	rl_logring("6666666666"); //  55555555556666666666.3333333334444444444.555555555 
	rl_logring("7777777777"); //  555555555566666666667777777777.444444444.555555555 
	rl_logring("8888888888"); //  5555555555666666666677777777778888888888.555555555 
	rl_logring("9999999999"); //  55555555556666666666777777777788888888889999999999. <--- 空间不足不能插入末尾的 '.'，缓存从头开始利用
				  //  9999999999.66666666677777777778888888888.999999999       处理后的结果
	rl_logring("AAAAAAAAAA"); //  9999999999AAAAAAAAAA.7777777778888888888.999999999
				  //                       <---     1     --->	               此时日志的正确阅读顺序 
				  //  <---     2       --->
				  //  77777777788888888889999999999AAAAAAAAAA.            <--- 最后在写入日志的内容
	printf("main end\n");
	return 0;
}