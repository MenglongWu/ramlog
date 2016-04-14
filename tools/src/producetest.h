#ifndef _PRODUCE_TEST_H_
#define _PRODUCE_TEST_H_

#include "autoconfig.h"

struct test_item
{
	int (*fun)(void *arg);
	char *description;
	char key;
};


#define RET_TI_BACK   (-10)

#define ECHO_PAUSE() 		\
{				\
	printf("\n按任意键继续\n");\
	getchar();		\
}
#define NOTE_START() printf("\n==============================================\n");
#define NOTE_END() printf("==============================================\n");
	
#define RESOULT_OK()	printf("\n\t\t\t\t\t\t\t\t\e[36m[正常]\e[0m\n\n");
#define RESOULT_NO()	printf("\n\t\t\t\t\t\t\t\e[31m[异常]\e[0m\n\n");
#define RESOULT_OK_EN()	printf("\n\t\t\t\t\t\t\t\t\e[36m[OK]\e[0m\n\n");
#define RESOULT_NO_EN()	printf("\n\t\t\t\t\t\t\t\e[31m[NO]\e[0m\n\n");

#define IO_START (1)
#define IO_END (19)

#if (CONFIG_BOARD == 11)
#define POS_WLAN0 "1-3.2:1.0"
#define POS_LAN0  "1-3.3.1:1.0"

#elif (CONFIG_BOARD == 12)
#define POS_WLAN0 "1-3.3.1:1.0"
#define POS_LAN0  "1-3.3.2:1.0"

#endif

#include <sys/types.h>
#include <dirent.h>
#include <stdbool.h>
struct check_dir
{
	char *dir;
	int result;//返回值0表示不存在，否则存在该目录
	char *todo;
};

struct ck_self
{
	char *notice;
	char *dir;
	char *cmp;
	int   condition;
#define COND_EQ		0
#define COND_NOTEQ	1
};


#define SMS_CAT "/dev/ttySAC3"
#endif
