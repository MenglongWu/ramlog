#define MAX_LAST_LOG (3)

struct ramlog {
	char name[256];
	char tm[20];
	char *rampath;					///< 日志内存路径
	char *diskpath;					///< 日志磁盘路径
	char *prefix;				///< 日志前缀
	int curid;					///< 当前文件id
	int last_log;					///< MAX_LAST_LOG
	int last_id[MAX_LAST_LOG]; ///< 最新的若干文件

	unsigned long s_log;				///< 单个日志大小，以byte为单位，为提高文件多次写入速度，建议大小设置成扇区大小
	unsigned long s_total;				///< 日志总大小，以byte为单位
	unsigned long s_cur;

};

#define _1K (1024)
#define _1M (1024*1024)