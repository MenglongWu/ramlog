#ifndef _RAMLOG_H_

#define _RAMLOG_H_

#define MAX_LAST_LOG (4)
#define MAX_PATH (256)
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
#define _1K (1024)
#define _1M (1024*1024)

void rl_mmap(struct ramlog *rl);
void rl_unmap(struct ramlog *rl);
int rl_resize(int ramsize, int disk_limit);
int rl_prefix(struct ramlog *rl, char *newprefix);
int rl_path(struct ramlog *rl, char *newdiskpath);
int rl_clone();
void rl_writefile(struct ramlog *rl);
int rl_log(const char *format, ...);
int rl_log2(struct ramlog *rl, const char *format, ...);
#endif
