#include <stdio.h>
#include <ramlog.h>

#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#define DBG_S(f,...) \
	printf("%s: %s(): %d:\n\t" f, __FILE__,__FUNCTION__,__LINE__,##__VA_ARGS__)

extern void fun1(char *a);
int main()
{
	printf("ppid %d pid %d tid %d\n",getppid(), getpid(),syscall(SYS_gettid));
	rl_clone();
#if 1
	rl_log2(&g_rl, "1111111111");
	rl_log2(&g_rl, "2222222222");
	rl_log2(&g_rl, "3333333333");
	rl_log2(&g_rl, "4444444444");
	rl_log2(&g_rl, "5555555555");
	rl_log2(&g_rl, "6666666666");
	rl_log2(&g_rl, "7777777777");
	rl_log2(&g_rl, "8888888888");
	rl_log2(&g_rl, "9999999999");
	rl_log2(&g_rl, "AAAAAAAAAA");
	rl_log2(&g_rl, "cccccccccc");
	rl_log2(&g_rl, "dddddddddd");
	rl_log2(&g_rl, "eeeeeeeeee");
	rl_log2(&g_rl, "ffffffffff");
	rl_log2(&g_rl, "gggggggggg");
	rl_log2(&g_rl, "hhhhhhhhhh");
	rl_log2(&g_rl, "iiiiiiiiii");
	rl_log2(&g_rl, "jjjjjjjjjj");
	rl_log2(&g_rl, "kkkkkkkkkk");
	rl_log2(&g_rl, "LLLLLLLLLL");
	rl_log2(&g_rl, "MMMMMMMMMM");
	rl_log2(&g_rl, "NNNNNNNNNN");
#endif
#if 0
	char *p = malloc(200);
	p[199] = '\0';
	memset(p,'A', 199);
	rl_log2(&g_rl, p);
	memset(p,'B', 199);
	rl_log2(&g_rl, p);
#endif
	// while(1) {
	// 	sleep(1);
	// }
	sleep(1);
	printf("main end\n");
	return 0;
}