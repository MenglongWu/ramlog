#ifndef _RAMLOG_H_

#define _RAMLOG_H_

#define MAX_LAST_LOG (4)
#define MAX_PATH (256)
struct ramlog {
	char name[256];
	char tm[20];
	
	int curid;					///< 当前文件id
	int last_log;					///< MAX_LAST_LOG
	char last_id[MAX_LAST_LOG][32]; ///< 最新的若干文件

	unsigned long s_log;				///< 单个日志大小，以byte为单位，为提高文件多次写入速度，建议大小设置成扇区大小
	unsigned long s_total;				///< 日志总大小，以byte为单位
	unsigned long s_cur;


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
	unsigned int offset;
	int dir_limit_size;
};
extern struct ramlog g_rl;
#define _1K (1024)
#define _1M (1024*1024)


int rl_dirlimitsize(struct ramlog *val, char *dir);
int rl_init(struct ramlog *val,
            char *rampath, char *diskpath,
            int s_log, int s_total);
int rl_onefile(struct ramlog *val, char *str, int n);
int rl_multifile(struct ramlog *val, char *str, int n);


int rl_snprintf (struct ramlog *val, char *s, size_t maxlen, const char *format, ...);
int rl_prefix(struct ramlog *rl, char *newprefix);
int rl_path(struct ramlog *rl, char *newdiskpath);
#endif