#ifndef _RAMLOG_H_

#define _RAMLOG_H_

#define MAX_LAST_LOG (4)
#define MAX_PATH (256)
#include <pthread.h>
#include <stdbool.h>
struct ramlog {
#if 0
	char name[256];
	

	int curid;					///< 当前文件id
	int last_log;					///< MAX_LAST_LOG
	char last_id[MAX_LAST_LOG][32]; ///< 最新的若干文件

	unsigned long s_log;				///< 单个日志大小，以byte为单位，为提高文件多次写入速度，建议大小设置成扇区大小
	unsigned long s_total;				///< 日志总大小，以byte为单位
	unsigned long s_cur;
#endif
	pthread_mutex_t mutex;

	pthread_cond_t cond_dump;
	pthread_mutex_t mutex_dump;
	
	char tm[20];
	int size;
	char *data;
	char *head;
	char *tail;
	char *read;
	char *write;
	char *dirty;

	char *rampath;					///< 日志内存路径
	char *diskpath;					///< 日志磁盘路径
	char *prefix;				///< 日志前缀
	char *filename;
	// unsigned int free;
	// unsigned int offset;
	int dir_limit_size;
};
extern struct ramlog g_rl;

void rl_mmap();
void rl_unmap();

/**
 * @brief	重新规划容量
 * @param	ramsize 日志内存缓存大小
 * @param	disk_limit 日志目录下最大容量
 * @retval	null
 * @remarks	disk_limit大小必须是ramsize 的100倍以上
 * @see
 */
int rl_resize(int ramsize, int disk_limit);

/**
 * @brief	定义日志文件前缀，未定义前采用默认前缀 DEF_PREFIX
 * @see DEF_PREFIX
 */
int rl_prefix(char *newprefix);

/**
 * @brief	定义日志保存路径，未定义前采用默认路径 DEF_DISKPATH
 * @see DEF_DISKPATH
 */
int rl_path(char *newdiskpath);

/**
 * @brief	创建子进程，子进程与主进程之间以共享内存方式通信
 * @param	null
 * @retval	>0 创建子进程 pid
 * @retval	<0 发生错误，错误码查阅 errno
 * @see	_rl_sub_process
 */
int rl_clone();

/**
 * @brief	强行将缓存内容写入文件
 	每次调用都会创建一个新的日志文件
 * @remarks	建议不要频繁调用
 * @see	
 */

void rl_writefile();


/**
 * @brief	日志内容是否是空的
 * @retval	true 空
 * @retval	false 有内容
 * @remarks	目的是避免不必要的向磁盘写入空内容文件
 * @see	_rl_reset
 * @see	_rl_sub_process
 */
int rl_log(const char *format, ...);

/**
 * @brief	以环形缓存方式向写日志，反复覆盖
 * @param	format 内容格式化
 * @param	... 格式化参数
 * @retval	null
 * @remarks	
 * @see	rl_log
 * @see	rl_writefile
 */
int rl_logring(const char *format, ...);



#define C_BLUE    "\e[36m"
#define C_NORMAL  "\e[0m"
#define C_RED     "\e[31m"
#define C_YELLOW  "\e[33m"
#define C_PURPLE  "\e[35m"

#define DEBUG C_BLUE
#define WARN  C_YELLOW
#define ERROR C_RED


#if CONFIG_LOG_TIMESTAMP
#   define RL_TIMESTAMP_V \
	time_t t = time(NULL);\
	struct tm *local = localtime(&t);
#   define RL_TIMESTAMP "%d%d%d %d:%d:%d "
#   define RL_TIMESTAMP_P(v) \
	(1900 + v->tm_year), (1 + v->tm_mon), (v->tm_mday),\
	(v->tm_hour), (v->tm_min), (v->tm_sec)
#else
#   define RL_TIMESTAMP_V
#   define RL_TIMESTAMP
#   define RL_TIMESTAMP_P(v)
#endif

#if CONFIG_LOG_TRACE
#   define RL_TRACE   "%s() %d: "
#   define RL_TRACE_P __FUNCTION__, __LINE__
#endif

#define crl_log(C1,C2,fmt, ...) rl_log(C1 fmt C2, ##__VA_ARGS__)
#if CONFIG_LOG_TRACE
#   define trl_log(C1,C2,fmt,...) \
	do { \
		RL_TIMESTAMP_V    \
		crl_log(C1,C2,    \
			RL_TIMESTAMP  \
			RL_TRACE      \
			fmt "\r\n",   \
			RL_TIMESTAMP_P(local), \
			RL_TRACE_P,	      \
			##__VA_ARGS__);       \
	}while(0)
#else
#   define trl_log(C1,C2,fmt,...) \
	do { \
		RL_TIMESTAMP_V   \
		crl_log(C1,C2,   \
			RL_TIMESTAMP \
			fmt "\r\n",  \
			RL_TIMESTAMP_P(local), \
			##__VA_ARGS__);       \
	}while(0)
#endif

#define LOGI(fmt,...) trl_log(""      ,       "", fmt, ##__VA_ARGS__)
#define LOGT(fmt,...) trl_log(C_BLUE  , C_NORMAL, fmt, ##__VA_ARGS__)
#define LOGD(fmt,...) trl_log(C_BLUE  , C_NORMAL, fmt, ##__VA_ARGS__)
#define LOGW(fmt,...) trl_log(C_YELLOW, C_NORMAL, fmt, ##__VA_ARGS__)
#define LOGA(fmt,...) trl_log(C_PURPLE, C_NORMAL, fmt, ##__VA_ARGS__)
#define LOGE(fmt,...) trl_log(C_RED   , C_NORMAL, fmt, ##__VA_ARGS__)

#endif
