
#include <stdio.h>
#include <sys/time.h>
#include <stdio.h>
#include <math.h>

#include <ramlog.h>
struct timeval tpstart, tpend;
float timeuse;
// 记录开始时间
void ResetConst()
{
	gettimeofday(&tpstart, NULL);
}
// 打印从调用ResetConst到调用PrintCost之间代码运行时间
float PrintCost()
{

	gettimeofday(&tpend, NULL);
	timeuse = 1000000 * (tpend.tv_sec - tpstart.tv_sec) +
	          tpend.tv_usec - tpstart.tv_usec;
	timeuse /= 1000000;
	printf("Used Time:%f\r\n", timeuse);
	return timeuse;

}


int main()
{
	struct ramlog rl;
	char data[10240];

	memset(data + 0 , '1', 1024);
	memset(data + 1024 * 1, '2', 1024);
	memset(data + 1024 * 2, '3', 1024);
	memset(data + 1024 * 3, '4', 1024);
	memset(data + 1024 * 4, '5', 1024);
	memset(data + 1024 * 5, '6', 1024);
	memset(data + 1024 * 6, '7', 1024);
	memset(data + 1024 * 7, '8', 1024);
	memset(data + 1024 * 8, '9', 1024);
	memset(data + 1024 * 9, 'a', 1024);


	rl_init(&rl, NULL, NULL, 0, 200* _1K);
	ResetConst();

	char strout[1024];
	rl_snprintf(&rl, strout, 1024,"%s %d", "asdfsf", 3);
#if 1
	rl_multifile(&rl, (char *)&data[0], 10240);
	rl_multifile(&rl, (char *)&data[0], 10240);
	rl_multifile(&rl, (char *)&data[0], 10240);
	rl_multifile(&rl, (char *)&data[0], 10240);

	// rl_multifile(&rl, (char *)&data[0], 10240);
	// rl_multifile(&rl, (char *)&data[0], 10240);
	// rl_multifile(&rl, (char *)&data[0], 10240);
	// rl_multifile(&rl, (char *)&data[0], 10240);

	// rl_multifile(&rl, (char *)&data[0], 10240);
	// rl_multifile(&rl, (char *)&data[0], 10240);
	// rl_multifile(&rl, (char *)&data[0], 10240);
	// rl_multifile(&rl, (char *)&data[0], 10240);

	// rl_multifile(&rl, (char *)&data[0], 10240);
	// rl_multifile(&rl, (char *)&data[0], 10240);
	// rl_multifile(&rl, (char *)&data[0], 10240);
	// rl_multifile(&rl, (char *)&data[0], 10240);

	// rl_multifile(&rl, (char *)&data[0], 10240);
	// rl_multifile(&rl, (char *)&data[0], 10240);
	// rl_multifile(&rl, (char *)&data[0], 10240);
	// rl_multifile(&rl, (char *)&data[0], 10240);
#else
	rl_multifile(&rl, (char *)&data[0], 1024);
	rl_multifile(&rl, (char *)&data[0], 1024);
	rl_multifile(&rl, (char *)&data[0], 1024);
	rl_multifile(&rl, (char *)&data[0], 1024);
	rl_multifile(&rl, (char *)&data[0], 1024);
	rl_multifile(&rl, (char *)&data[0], 1024);
	rl_multifile(&rl, (char *)&data[0], 1024);
	rl_multifile(&rl, (char *)&data[0], 1024);
	rl_multifile(&rl, (char *)&data[0], 1024);
	rl_multifile(&rl, (char *)&data[0], 1024);

	rl_multifile(&rl, (char *)&data[0], 1024);
	rl_multifile(&rl, (char *)&data[0], 1024);
	rl_multifile(&rl, (char *)&data[0], 1024);
	rl_multifile(&rl, (char *)&data[0], 1024);
	rl_multifile(&rl, (char *)&data[0], 1024);
	rl_multifile(&rl, (char *)&data[0], 1024);
	rl_multifile(&rl, (char *)&data[0], 1024);
	rl_multifile(&rl, (char *)&data[0], 1024);
	rl_multifile(&rl, (char *)&data[0], 1024);
	rl_multifile(&rl, (char *)&data[0], 1024);

	rl_multifile(&rl, (char *)&data[0], 1024);
	rl_multifile(&rl, (char *)&data[0], 1024);
	rl_multifile(&rl, (char *)&data[0], 1024);
	rl_multifile(&rl, (char *)&data[0], 1024);
	rl_multifile(&rl, (char *)&data[0], 1024);
	rl_multifile(&rl, (char *)&data[0], 1024);
	rl_multifile(&rl, (char *)&data[0], 1024);
	rl_multifile(&rl, (char *)&data[0], 1024);
	rl_multifile(&rl, (char *)&data[0], 1024);
	rl_multifile(&rl, (char *)&data[0], 1024);


#endif
	PrintCost();
	printf("father ready end\n");

	rl_dirlimitsize(&rl, rl.rampath);
	// sleep(4);
	printf("father end %d \n", rl.curid);


	return 0;
}