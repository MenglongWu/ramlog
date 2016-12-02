/**
 ******************************************************************************
 * @file	t-dump.c
 * @brief	TODO:brief 

TODO:introduce 
 *
 @section Platform
	-# 
 @section Library
	-# 
- 20xx-xx-xx,author,email
 	- brief

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

	printf("ppid %d pid %d tid %d\n", getppid(), getpid(), syscall(SYS_gettid));

	rl_resize(_1K * 4, _1M * 16);
	rl_prefix("mylog");
	rl_path("./log/t-dump/");
	rl_clone();


	rl_logring("1111111111");
	rl_logring("2222222222");
	rl_logring("3333333333");
	rl_logring("4444444444");
	rl_logring("5555555555");
	rl_logring("6666666666");
	rl_logring("7777777777");
	rl_logring("8888888888");
	rl_logring("9999999999");
	rl_logring("AAAAAAAAAA");
	rl_logring("bbbbbbbbbb");
	rl_logring("cccccccccc");
	rl_logring("dddddddddd");
	rl_logring("eeeeeeeeee");
	rl_logring("ffffffffff");
	rl_logring("gggggggggg");
	rl_logring("hhhhhhhhhh");
	rl_logring("iiiiiiiiii");
	rl_logring("jjjjjjjjjj");
	rl_logring("kkkkkkkkkk");
	rl_logring("LLLLLLLLLL");
	rl_logring("MMMMMMMMMM");
	rl_logring("NNNNNNNNNN");

	printf("main end\n");
	return 0;
}