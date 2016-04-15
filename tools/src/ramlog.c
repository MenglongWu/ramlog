#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ramlog.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
const char def_rampath  [] = "/dev/shm/log";
const char def_diskpath  [] = "./log";
//const char def_rampath  [] = "./log";
const char def_prefix[] =  "log";



#ifdef _DEBUG
	#define dbg(fmt, ...) printf(fmt, ##__VA_ARGS__)
#else
	#define dbg(fmt, ...)
#endif

/**
 * @brief	找到文件最新的“几个”文件
 * @param	null
 * @retval	找到的文件数，最多 MAX_LOG
 * @remarks	仅将最新文件的ID写入 last_id

ls log -l
-rwxr-xr-x 1 root root 3 2016-04-15 09:11 log1
-rwxr-xr-x 1 root root 3 2016-04-15 09:14 log2
-rwxr-xr-x 1 root root 3 2016-04-15 09:16 log3
-rwxr-xr-x 1 root root 3 2016-04-15 09:18 log4
-rwxr-xr-x 1 root root 3 2016-04-15 09:20 log5

ls log -tr | grep log
log1
log2
log3
log4
log5

ls log -tr | grep log | tail -n 3
log3
log4
log5
 */
int _rl_findlast(struct ramlog *val)
{

	char cmd[1024];
	char filter[32];
	FILE *stream;
	int i;

	// 在日志目录下搜索所有日志文件，并以时间排序
	snprintf(cmd, 1024,
	         "cd %s;"
	         "ls  | grep '%s'|"
	         "tail -n %d", val->diskpath, val->prefix, MAX_LAST_LOG);
	printf("cmd %s\n", cmd);

	stream = popen(cmd, "r");
	if (stream == NULL) {
		perror("_rl_findlast() popen()");
		return 0;
	}
	// fread(cmd , 1, 1024, stream);
	// printf("out %s\n", cmd);
	// return 0;
	// 遍历所有收到的文件名，并记录ID
	// snprintf(filter, 32, "%s%%d", val->prefix);
	snprintf(filter, 32, "%s%s-%%d", val->prefix, val->tm);
	for (i = 0; i < MAX_LAST_LOG; i++) {

		fgets(val->last_id[i], 1024, stream);
		if (feof(stream)) {
			break;
		}
		// strcpy(cmd, "log20160415-14-49-41-3");
		// sscanf(cmd, filter, &val->last_id[i]);
		// dbg("[%s] [%s] id = %d\n", cmd, filter, val->last_id[i]);
		int len;
		len = strlen(val->last_id[i]);
		val->last_id[i][len-1] = '\0';
		dbg("[%s]\n", val->last_id[i]);
	}
	printf("i = %d\n", i);
	// if (i == 0) {
	// 	val->curid = val->last_id[i];
	// }
	// else {
	// 	val->curid = val->last_id[i - 1];
	// }
	pclose(stream);


	return 0;
}

/**
 * @brief	清除文件夹，删除大多数日志文件，仅保留最新文件
 * @param	null
 * @retval	null
 * @remarks
 */
int _rl_rm_except_last(struct ramlog *val)
{
	char cmd[1024];
	char filter[32];
	FILE *stream;
	int i;
	char filelist[256] = "";

	for (i = 0; i < MAX_LAST_LOG; i++) {
		if (val->last_id[i]) {
			if (i != 0) {
				strcat(filelist, "\\|");
			}
			strcat(filelist, val->last_id[i]);
		}
		else {
			break;	
		}
		
	}
	// 在日志目录下搜索所有日志文件，并以时间排序
	snprintf(cmd, 1024,
	         "cd %s;"
	         "ls  | grep -v '%s'|"
	         "xargs rm -f", val->diskpath, filelist);
	printf("--------cmd %s\n", cmd);
	system(cmd);
	// exit(0);

	return 0;
}

void _rl_tm(struct ramlog *val)
{
	time_t t;
	struct tm *local; //本地时间

	t = time(NULL);
	local = localtime(&t); //转为本地时间


	// char buf[20];
	strftime(val->tm, 20, "%Y%m%d-%H-%M-%S", local);
}
void _rl_rm_ram(struct ramlog *val)
{
	char cmd[1024];
	snprintf(cmd, 1024, "rm %s/*", val->rampath);
	system(cmd);	
}
void _rl_ram2disk(struct ramlog *val)
{
	// return ;
	char cmd[1024];
	snprintf(cmd, 1024, "cp %s/* %s", val->rampath, val->diskpath);
	system(cmd);
	printf("val->curid %d\n", val->curid);
}

int _rl_ckdirsize(struct ramlog *val)
{
	char cmd[1024];
	char str[32];
	FILE *stream;
	int dirsize;

	snprintf(cmd, 1024, "du -b %s", val->diskpath);
	stream = popen(cmd, "r");
	if (stream == NULL) {
		return 0;
	}
	fgets(str,  32, stream);
	pclose(stream);



	dirsize = atoi(str);
	printf("dirsize = %d s_total %ld\n", dirsize , val->s_total);
	if (dirsize > val->s_total) {
		_rl_rm_except_last(val);
	}
	

	return 0;
}
int rl_init(struct ramlog *val,
            char *rampath, char *diskpath,
            int s_log, int s_total)
{
	bzero(val, sizeof(struct ramlog));


	if (NULL == rampath) {
		val->rampath     = def_rampath;
	}
	else {
		val->rampath     = rampath;
	}
	if (NULL == diskpath) {
		val->diskpath    = def_diskpath;
	}
	else {
		val->diskpath    = diskpath;
	}
	val->prefix          = def_prefix;

	if (s_log < 4 * _1K ) {
		val->s_log        = 4 * _1K;
	}
	else {
		val->s_log	= s_log;
	}
	if (s_total < val->s_log * MAX_LAST_LOG) {
		val->s_total = 4 * _1K * MAX_LAST_LOG;
	}
	else {
		val->s_total = s_total;
	}


	mkdir(val->rampath, 777);
	mkdir(val->diskpath, 777);

	_rl_tm(val);
	_rl_findlast(val);
	_rl_ckdirsize(val);
	
	// printf("logid %d\n", val->curid);
	
	val->curid = 0;
	snprintf(val->name, 256, "%s/%s%s-%d", val->rampath, val->prefix, val->tm, val->curid);
	printf("val->name %s\n" , val->name);

	_rl_rm_ram(val);
	pid_t fpid = fork();
	if (fpid < 0) {
		perror("fork");
	}
	else if (fpid == 0) {
		printf("child %d %d\n" , getpid(), getppid());
		while (getppid() != 1) {
			sleep(10);
		}
		printf("get sig\n");
		// TODO copy ramfs to disk
		_rl_ram2disk(val);

		exit(0);
	}
	return 0;
}


/**
 * @brief	写日志
 * @param	null
 * @retval	null
 * @remarks
 * @see
 */
int rl_onefile(struct ramlog *val, char *str, int n)
{
	FILE *fd;
	dbg("\nopen file %s size %d\n", val->name, n);
	fd = fopen(val->name, "a+");
	if (fd == NULL) {
		dbg("err");
		return 0;
	}
	int len, byte;
	byte = fwrite(str,  1, n, fd);

	dbg("byte = %d\n", byte);
	len = ftell(fd);
	fclose(fd);
	// sleep(1);
	return len;
}

int rl_multifile(struct ramlog *val, char *str, int n)
{
	FILE *fd;
	int  len;
	int byte, offset = 0;


	// if (val->s_cur >= val->s_log) {
	// 	val->curid++;
	// 	snprintf(val->name, 256, "%s/%s%s-%d", val->rampath, val->prefix, val->tm, val->curid);
	// 	val->s_cur = rl_onefile(val, str + offset, n);
	// }
	// else
	if (val->s_cur + n > val->s_log) {
		byte = val->s_log - val->s_cur;
		val->s_cur = rl_onefile(val, str, byte);
		offset += byte;
		n -= byte;
		while(n > 0) {
			if (n > val->s_log) {
				byte = val->s_log;
			}
			else {
				byte = n;
			}
			val->curid++;
			snprintf(val->name, 256, "%s/%s%s-%d", val->rampath, val->prefix, val->tm, val->curid);

			val->s_cur = rl_onefile(val, str + offset, byte);
			offset += byte;
			n -= byte;
		}
	}
	else {
		val->s_cur = rl_onefile(val, str, n);
	}
}

#include <stdio.h>
#include <sys/time.h>
#include <stdio.h>
#include <math.h>

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

#if 1
	rl_multifile(&rl, (char *)&data[0], 10240);
	rl_multifile(&rl, (char *)&data[0], 10240);
	rl_multifile(&rl, (char *)&data[0], 10240);
	rl_multifile(&rl, (char *)&data[0], 10240);
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
	// sleep(4);
	printf("father end %d \n", rl.curid);

	return 0;
}