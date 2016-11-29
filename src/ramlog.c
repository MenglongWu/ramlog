#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ramlog.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <autoconfig.h>
#define DEF_RAMPATH  "/dev/shm/log"
#define DEF_DISKPATH  "./log/a/b"
#define DEF_PREFIX "log-"



#ifdef CONFIG_RAMLOG_DBG
	#define dbg(fmt, ...) printf("\e[36m" fmt "\e[0m", ##__VA_ARGS__)
#else
	#define dbg(fmt, ...)
#endif
#define TRRAC_TAG() \
	dbg("%s: %s(): %d\n", __FILE__,__FUNCTION__,__LINE__)
#define TRRAC_TAG_S(fmt,...) \
	dbg("%s: %s(): %d: " fmt, __FILE__,__FUNCTION__,__LINE__,##__VA_ARGS__)

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
int _rl_findlast(struct ramlog *val, char *dir)
{

	char cmd[1024];
	char filter[32];
	FILE *stream;
	int i;

	// 在日志目录下搜索所有日志文件，并以时间排序
	snprintf(cmd, 1024,
	         "cd %s;"
	         "ls  | grep '%s'|"
	         "tail -n %d", dir, val->prefix, MAX_LAST_LOG);
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
		val->last_id[i][len - 1] = '\0';
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
int _rl_rm_except_last(struct ramlog *val, char *dir)
{
	char cmd[1024];
	int i;
	char filelist[256] = "";

	for (i = 0; i < MAX_LAST_LOG; i++) {
		if (val->last_id[i]) {
			if (i != 0) {
				strcat(filelist, "\\|");
			}
			strcat(filelist, val->last_id[i]);
		} else {
			break;
		}

	}
	// 在日志目录下搜索所有日志文件，并以时间排序
	snprintf(cmd, 1024,
	         "cd %s;"
	         "ls  | grep -v '%s'|"
	         "xargs rm -f", dir, filelist);
	printf("--------cmd %s\n", cmd);
	system(cmd);
	// exit(0);

	return 0;
}
#include <time.h>
void _rl_tm(struct ramlog *val)
{
	time_t t;
	struct tm *local; //本地时间

	t = time(NULL);
	local = localtime(&t); //转为本地时间
	strftime(val->tm, 20, "%Y%m%d-%H-%M", local);
}
/**
 * @brief	清除所有ram文件
 * @param	null
 * @retval	null
 * @remarks
 * @see
 */

void _rl_rm_ram(struct ramlog *val)
{
	char cmd[1024];
	snprintf(cmd, 1024, "rm %s/*", val->rampath);
	system(cmd);
}

/**
 * @brief	将ram文件拷贝到disk（当主进程崩溃时子进程自动完成）
 * @param	null
 * @retval	null
 * @remarks
 * @see
 */

void _rl_ram2disk(struct ramlog *val)
{
	// return ;
	char cmd[1024];
	snprintf(cmd, 1024, "cp %s/* %s", val->rampath, val->diskpath);
	system(cmd);
	printf("val->curid %d\n", val->curid);
}

/**
 * @brief	如果日志目录过大则清除部分
 * @param	null
 * @retval	null
 * @remarks
 * @see
 */

int rl_dirlimitsize(struct ramlog *val, char *dir)
{
	char cmd[1024];
	char str[32];
	FILE *stream;
	int dirsize;

	snprintf(cmd, 1024, "du -b %s", dir);
	printf("check dir %s\n", cmd);
	stream = popen(cmd, "r");
	if (stream == NULL) {
		return 0;
	}
	fgets(str,  32, stream);
	pclose(stream);



	dirsize = atoi(str);
	printf("dirsize = %d s_total %ld %s\n", dirsize , val->s_total, dir);
	if (dirsize > val->s_total) {
		_rl_rm_except_last(val, dir);
	}

	return 0;
}

/**
 * @brief
 * @param	null
 * @retval	null
 * @remarks
 * @see
 */

int rl_init(struct ramlog *val,
            char *rampath, char *diskpath,
            int s_log, int s_total)
{
	bzero(val, sizeof(struct ramlog));


	if (NULL == rampath) {
		val->rampath     = (char *)DEF_RAMPATH;
	} else {
		val->rampath     = (char *)rampath;
	}
	if (NULL == diskpath) {
		val->diskpath    = (char *)DEF_DISKPATH;
	} else {
		val->diskpath    = (char *)diskpath;
	}
	val->prefix          = (char *)DEF_PREFIX;

	if (s_log < 4 * _1K ) {
		val->s_log        = 4 * _1K;
	} else {
		val->s_log	= s_log;
	}
	if (s_total < val->s_log * MAX_LAST_LOG) {
		val->s_total = 4 * _1K * MAX_LAST_LOG;
	} else {
		val->s_total = s_total;
	}


	mkdir(val->rampath, 777);
	mkdir(val->diskpath, 777);

	_rl_tm(val);
	_rl_rm_ram(val);
	_rl_findlast(val, val->diskpath);
	rl_dirlimitsize(val, val->diskpath);

	// printf("logid %d\n", val->curid);

	val->curid = 0;
	snprintf(val->name, 256, "%s/%s%s-%d", val->rampath, val->prefix, val->tm, val->curid);
	printf("val->name %s\n" , val->name);


	pid_t fpid = fork();
	if (fpid < 0) {
		perror("fork");
	} else if (fpid == 0) {
		printf("child %d %d\n" , getpid(), getppid());
		while (getppid() != 1) {
			sleep(1);
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
	int byte, offset = 0;

	if (val->s_cur + n > val->s_log) {
		byte = val->s_log - val->s_cur;
		val->s_cur = rl_onefile(val, str, byte);
		offset += byte;
		n -= byte;
		while(n > 0) {
			if (n > val->s_log) {
				byte = val->s_log;
			} else {
				byte = n;
			}
			val->curid++;
			snprintf(val->name, 256, "%s/%s%s-%d", val->rampath, val->prefix, val->tm, val->curid);

			val->s_cur = rl_onefile(val, str + offset, byte);
			offset += byte;
			n -= byte;
		}
	} else {
		val->s_cur = rl_onefile(val, str, n);
	}
	return val->s_cur;
}



#include <stdarg.h>
int
rl_snprintf (struct ramlog *val, char *s, size_t maxlen, const char *format, ...)
{
	va_list arg;
	int done;

	va_start (arg, format);
	done = vsnprintf (s, maxlen, format, arg);
	va_end (arg);

	rl_multifile(val, s, done);
	return done;
}


// ***************************************
// 默认进程内存大小8K
// 管道代替内存文件系统
// 默认
#define  likely(x)        __builtin_expect(!!(x), 1)
#define  unlikely(x)      __builtin_expect(!!(x), 0)

#include <sched.h>
#include <sys/mman.h>
#include <errno.h>
#include <assert.h>
#include <stdbool.h>
#include "bb.h"
struct ramlog g_rl;

// 分配共享内存
void rl_mmap(struct ramlog *rl)
{

}

void rl_unmap(struct ramlog *rl)
{

}

static void _rl_reset(struct ramlog *rl)
{
	rl->head  = rl->data;
	rl->tail  = rl->data + rl->size;
	rl->read  = rl->data;
	rl->write = rl->data;
	rl->dirty = rl->tail;
	rl->offset = 0;
	rl->curid  = 0;
}
#define _1K (1024)
#define _1M (1024*_1K)


// 在main函数之前分配内存环境
void __attribute__((constructor)) _rl_init()
{
	TRRAC_TAG();
#ifdef CONFIG_RAMLOG_100BYTE_CACHE
	g_rl.size = 100;
#else
	g_rl.size = _1K * 64;
#endif
	TRRAC_TAG_S("cache size %d\n", g_rl.size);
	// 分配切填充内存
	g_rl.data = (char *)malloc(g_rl.size + 1); // 多出的一个字节为安全
	if (g_rl.data == NULL) {
		printf("%s: %s(): %d %s\n", __FILE__, __FUNCTION__, __LINE__, strerror(errno));
		exit(1);
	}
	bzero(g_rl.data, g_rl.size + 1);

	g_rl.diskpath = (char *)malloc(strlen(DEF_DISKPATH) + 1);
	g_rl.prefix   = (char *)malloc(strlen(DEF_PREFIX) + 1);
	g_rl.filename = (char *)malloc(MAX_PATH);
	if (g_rl.diskpath == NULL || g_rl.prefix == NULL  || g_rl.filename == NULL) {
		goto err;
	}
	strcpy(g_rl.diskpath, DEF_DISKPATH);
	bzero(g_rl.filename, MAX_PATH);
	_rl_reset(&g_rl);
	return ;
err:
	if (g_rl.diskpath) {
		free(g_rl.diskpath);
	}
	if (g_rl.prefix) {
		free(g_rl.prefix);
	}
	if (g_rl.filename	) {
		free(g_rl.filename);
	}
	g_rl.diskpath = NULL;
	g_rl.prefix   = NULL;
	g_rl.filename = NULL;
}

int _rl_repath(char **oldstr, char *newstr)
{
	free(*oldstr);
	*oldstr = NULL;

	*oldstr = (char *)malloc(strlen(newstr) + 1);
	if (*oldstr == NULL) {
		printf("%s: %s(): %d %s\n", __FILE__, __FUNCTION__, __LINE__, strerror(errno));
		exit(1);
	}
	strcpy(*oldstr, newstr);
	return 0;
}
// 重命名前缀
int rl_prefix(struct ramlog *rl, char *newprefix)
{
	assert(rl != NULL);
	assert(rl->prefix != NULL);
	assert(newprefix != NULL);
	return _rl_repath(&rl->prefix, newprefix);
}

// 重命名路径
int rl_path(struct ramlog *rl, char *newdiskpath)
{
	assert(rl != NULL);
	assert(rl->diskpath != NULL);
	assert(newdiskpath != NULL);
	return _rl_repath(&rl->diskpath, newdiskpath);
}
int _rl_sub_process(void *ptr);
#define STACK_SIZE (_1K*12)

// 创建克隆子进程
int rl_clone()
{
	void *pstack = (void *)mmap(NULL,
	                            STACK_SIZE,
	                            PROT_READ | PROT_WRITE ,
	                            MAP_PRIVATE | MAP_ANONYMOUS | MAP_ANON ,//| MAP_GROWSDOWN ,
	                            -1,
	                            0);
	pid_t pid;

	if (MAP_FAILED != pstack) {

		// 以共享内存方式创建子进程
		pid = clone(
		          _rl_sub_process,
		          (void *)((unsigned char *)pstack + STACK_SIZE),
		          CLONE_VM,
		          &g_rl);
		if (-1 != pid) {
			TRRAC_TAG_S("clone pid %d\n", pid);
		} else {
			TRRAC_TAG_S("clone fail: %s\n", strerror(errno));
		}
	}
	return -1;
}

// 检查日志目录下的容量是否到极限
static bool _rl_diskpoor(struct ramlog *rl)
{
	return false;
}

static int _rl_compress(struct ramlog *rl)
{
	// 压缩所有log
	return 0;
}

void _rl_mkdir(struct ramlog *rl)
{
	char strout[256];
	snprintf(strout, 256, "%s", rl->diskpath);
	printf("mkdir %s", strout);
	bb_make_directory(strout, 0777,  FILEUTILS_RECUR);
}
/*
1. 缓存末端dirty（大都数情况）
UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUDDDDDDDDDDD
^                               ^     ^         ^
|                               |     |         |
head                                           tail
                               read  dirty
                               write
1) 写入read -- dirty内容
2) 写入head -- read 内容


2. 缓存内部没有dirty
UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUFFFFFFF
^                                         ^     ^
|                                         |     |
head                                           tail
                                         read  dirty
                                         write

1) 写入read -- write内容
2) 写入head -- read 内容


写入后内存复位
FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
^                                               ^
|                                               |
head                                           tail
read                                           dirty
write
*/
void rl_writefile(struct ramlog *rl)
{
	assert(rl != NULL);
	assert(rl->size != NULL);
	assert(rl->data != NULL);
	assert(rl->head == rl->data);
	assert(rl->tail == rl->data + rl->size);
	assert(rl->dirty <= rl->tail);

	TRRAC_TAG_S("head %x tail %x size %d\n", rl->head, rl->tail, rl->size);
	TRRAC_TAG_S("read %x(%d) write %x(%d) dirty %x(%d)\n",
	            rl->read, rl->read - rl->head,
	            rl->write, rl->write - rl->head,
	            rl->dirty, rl->dirty - rl->head);
	_rl_tm(rl);
	_rl_mkdir(&g_rl);


	static int index = 0;
	snprintf(rl->filename, MAX_PATH, "%s/%s%s-%d", rl->diskpath, rl->prefix, rl->tm, index);

	FILE *fp = fopen(rl->filename, "wb");
	int ret;
	if (fp == NULL) {
		return ;
	}
	if (likely(rl->dirty != rl->tail)) {
		dbg("read -- dirty  %0.200s\n", rl->read + 1);
		dbg("head -- read  %0.200s\n", rl->head);
		fprintf(fp, "%s", rl->read + 1);
		fprintf(fp, "%s", rl->head);

	} else {
		dbg("head -- read  %0.200s\n", rl->head);
		fprintf(fp, "%s", rl->head);
	}
	fclose(fp);
	_rl_reset(rl);
	index++;
}
#ifdef CONFIG_RAMLOG_100BYTE_CACHE
void _dbg_disp(struct ramlog *rl)
{
	assert(rl != NULL);
	assert(rl->size != NULL);
	assert(rl->data != NULL);
	assert(rl->head == rl->data);
	assert(rl->tail == rl->data + rl->size);
	assert(rl->dirty <= rl->tail);

	TRRAC_TAG_S("head %x tail %x size %d\n", rl->head, rl->tail, rl->size);
	TRRAC_TAG_S("read %x(%d) write %x(%d) dirty %x(%d)\n",
	            rl->read, rl->read - rl->head,
	            rl->write, rl->write - rl->head,
	            rl->dirty, rl->dirty - rl->head);
	if (likely(rl->dirty != rl->tail)) {
		dbg("read -- dirty  %0.200s\n", rl->read + 1);
		dbg("head -- read  %0.200s\n", rl->head);
	} else {
		dbg("head -- read  %0.200s\n", rl->head);
	}
}
#else
#define _dbg_disp(v)
#endif
void sig_user(int v)
{
	// 根据信号参数执行
	// 1. 拷贝日志到内存
	// 2. 保存日志到文件系统
}

#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>

int _rl_sub_process(void *ptr)
{
	struct ramlog *p = (struct ramlog *)ptr;
	TRRAC_TAG_S("ppid %d pid %d\n", getppid(), getpid());
	while(getppid() != 1) {
		// 死循环等待子进程退出，循环间隔1s
		// 当主进程退出后本进程同时退出
		sleep(1);
	}

	// 控制日志文件系统的大小
	// 检查是否需要多当前日志目录进行压缩
	if (_rl_diskpoor(&g_rl)) {
		if (_rl_compress(&g_rl)) {

		}
	}
	dbg("\n");
	TRRAC_TAG_S("parent exit\n");
	// todo释放所有共享资源，
	TRRAC_TAG_S("compress log\n");

	// 日志保存到文件系统
	TRRAC_TAG_S("save log\n");
	rl_writefile(&g_rl);
	// 删除日志文件系统的内容（暂时不能删除，调试要用）


	puts("\n");
	return 0;
}

// 写入到默认日志位置
int rl_log(const char *format, ...)
{

}

// 写入到自定日志位置
/*
标记
U 已经使用的内存
F 空闲内存
u 刚刚写入的新内容
N 超出部分
n 超出部分被重新写入
D 脏内存

1. 连续内存充足情况写入新内容
------------------------------
初始状态内存
FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
^                                               ^
|                                               |
head                                           tail
read                                           dirty
write

写入后
uuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuFFFFFFFFFFF
^                                     ^         ^
|                                     |         |
head                                           tail
read                                 write     dirty


2. 写入新内容超过剩余连续容量
-----------------------------
写入前
UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUFFFFFFFFFFF
^                                     ^         ^
|                                     |         |
head                                           tail
read                                 write     dirty

空闲内存不足，此时wirte后的内容已经被修改不再是之前的内容'F'，
超出部分是'N'的长度，'N'并没有真正写入，不会造成地址越界
第一次写入后
UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUuuuuuuuuuuuNNNNNNNNNNN
^                                     ^         ^
|                                     |         |
head                                           tail
read                                 write

标记dirty后的内容无效
写入位置换成head处重新写入
第二次写入后
uuuuuuuuuuunnnnnnnnnnnUUUUUUUUUUUUUUUUuuuuuuuuuuu
^                     ^               ^         ^
|                     |               |         |
head                                           tail
                      read           dirty
                      write
值得注意的是，第二次就算再不够用也不会重新分配内存，write指向
tail，日志缓存都是nK以上，而每次写入内容最多也就几百byte，这
种能将缓存全部填满的情况是不应该出现的


3. 存在dirty情况下写入新内容
----------------------------
写入前
UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUDDDDDDDDDDD
^                     ^               ^         ^
|                     |               |         |
head                                           tail
                      read           dirty
                      write

写入少量内容，切不足以与dirty区域叠加
UUUUUUUUUUUUUUUUUUUUUUUuuuuuuuuuUUUUUUDDDDDDDDDDD
^                               ^     ^         ^
|                               |     |         |
head                                           tail
                               read  dirty
                               write
写入内容与dirty区域叠加，dirty无效
UUUUUUUUUUUUUUUUUUUUUUUuuuuuuuuuuuuuuuuuuuFFFFFFF
^                                         ^     ^
|                                         |     |
head                                           tail
                                         read  dirty
                                         write

*/



int rl_log2(struct ramlog *rl, const char *format, ...)
{
	// todo LOCK
	assert(rl != NULL);
	assert(rl->size != NULL);
	assert(rl->data != NULL);
	assert(rl->head == rl->data);
	assert(rl->tail == rl->data + rl->size);
	assert(rl->dirty <= rl->tail);




	// 写入新内容到内存
	va_list arg;
	int done;

	if (rl->dirty < rl->tail) {

	} else if (rl->dirty > rl->tail) { // 不可能
		_rl_reset(rl);
	}
	va_start (arg, format);
	done = vsnprintf (
	           rl->write,
	           rl->tail - rl->write,
	           format,
	           arg);
	va_end (arg);
	// printf("done rl->data %x %d\n", rl->data, done);




	// 连续空闲内存是否足够填充新的日志
	// printf("done %d free %d\n", done, rl->tail - rl->write);
	if ( likely(rl->dirty <= rl->tail)) {
		if ( likely(done < rl->tail - rl->write)) {
			rl->write += done;
			rl->read = rl->write;
			if (rl->write >= rl->dirty) {
				rl->dirty = rl->tail;
			}
		} else {
			*rl->write = '\0';// 设置字符串结束
			rl->dirty = rl->write;
			va_start (arg, format);
			done = vsnprintf (
			           rl->data,
			           rl->size,
			           format,
			           arg);
			va_end (arg);
			if ( likely(done < rl->size) ) {
				rl->write = rl->data + done;
				rl->read = rl->write;
			} else {
				printf("warning: ramlog cache too small\n");
				_rl_reset(rl);
			}
		}
	} else {
		if ( likely(done < rl->tail - rl->write) ) {
			rl->write += done;
		} else {
			// 不够则将日志写入点标记为 “脏” 从头开始填写
			rl->dirty = rl->write;
			*rl->dirty = '\0';
			va_start (arg, format);
			done = vsnprintf (
			           rl->data,
			           rl->size,
			           format,
			           arg);
			va_end (arg);
			if ( likely(done < rl->size) ) {
				rl->write = rl->data + done;
				rl->read = rl->write;
			} else {
				printf("warning: ramlog cache too small\n");
				_rl_reset(rl);
			}
		}
	}
	_dbg_disp(rl);
	// TODO unlock
}


