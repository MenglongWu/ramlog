#ifndef _PRINTL_H_
#define _PRINTL_H_


#define C_BLUE    "\e[36m"
#define C_NORMAL  "\e[0m"
#define C_RED     "\e[31m"
#define C_YELLOW  "\e[33m"
#define C_PURPLE  "\e[35m"

#define C_NULL  ""
#define C_INFO  C_NULL
#define C_TRACE C_BLUE
#define C_DEBUG C_BLUE
#define C_WARN  C_YELLOW
#define C_ERROR C_RED
#define C_ALARM C_PURPLE

#include <printl_common.h>
#include <sys/time.h>
#include <time.h>



#define  likely(x)        __builtin_expect(!!(x), 1)
#define  unlikely(x)      __builtin_expect(!!(x), 0)

#if 0
#define __printl(C1,C2,fmt, ...) printf(C1 fmt C2, ##__VA_ARGS__)
#define _printl(level,C1,C2,fmt,...) \
	if( unlikely(level) ) { \
		__printl(C1,C2,fmt, ##__VA_ARGS__); \
	}

#define PRINTL_I(fmt,...) _printl(1,""      ,       "", fmt, ##__VA_ARGS__)
#define PRINTL_T(fmt,...) _printl(1,C_BLUE  , C_NORMAL, fmt, ##__VA_ARGS__)


#define _cprint(C1,C2, fmt,...)  \
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


#ifdef NDEBUG
#   define PRINTL_D(fmt,...)
#else
// _cprint(C_BLUE, C_NORMAL, fmt, ##__VA_ARGS__)
#   define PRINTL_D(fmt,...)  \
	do {\
		RL_TIMESTAMP_V    \
		_cprint(C_BLUE, C_NORMAL, \
		        RL_TIMESTAMP\
		        RL_TRACE\
		        fmt  \
		        RL_TIMESTAMP_P(local) \
		        RL_TRACE_P, \
		        ##__VA_ARGS__)
};
#endif
#define PRINTL_W(fmt,...) _printl(1,C_YELLOW, C_NORMAL, fmt, ##__VA_ARGS__)
#define PRINTL_A(fmt,...) _printl(1,C_PURPLE, C_NORMAL, fmt, ##__VA_ARGS__)
#define PRINTL_E(fmt,...) _printl(1,C_RED   , C_NORMAL, fmt, ##__VA_ARGS__)
#endif

// ***************************************************************************
// 时间戳
#   define PL_TIMESTAMP_V \
	time_t t = time(NULL);\
	struct tm *local = localtime(&t);
#   define PL_TIMESTAMP "%d%d%d %d:%d:%d "
#   define PL_TIMESTAMP_P(v) \
	(1900 + v->tm_year), (1 + v->tm_mon), (v->tm_mday),\
	(v->tm_hour), (v->tm_min), (v->tm_sec)

// **************************
// 跟踪
#   define PL_TRACE   "%s() %d: "
#   define PL_TRACE_P __FUNCTION__, __LINE__



// **************************
//
// 带色彩打印
// Hello world!
#define ___cprint(C1,C2,fmt, ...) printf(C1 fmt C2, ##__VA_ARGS__)



// 色彩时间戳
// 2016126 17:23:28 main() 12:  Hello world!\r\n
#define _ctstprint(C1,C2,fmt,...) \
	do { \
		PL_TIMESTAMP_V    \
		___cprint(C1,C2,    \
		          PL_TIMESTAMP  \
		          PL_TRACE      \
		          fmt "\r\n",   \
		          PL_TIMESTAMP_P(local), \
		          PL_TRACE_P,	      \
		          ##__VA_ARGS__);       \
	}while(0)

// 无时间错带色彩
// main() 12:  Hello world!\r\n
#define _ctprint(C1,C2,fmt,...) \
	do { \
		___cprint(C1,C2,    \
		          PL_TRACE      \
		          fmt "\r\n",   \
		          PL_TRACE_P,	      \
		          ##__VA_ARGS__);       \
	}while(0)





// 划分等级打印
// #define __printl(C1,C2,fmt, ...) printf(C1 fmt C2, ##__VA_ARGS__)
#define _printl(level,fmt,...) \
	if( unlikely(level) ) { \
		printf(fmt, ##__VA_ARGS__); \
	}


// ***************************************************************************
// 打印级别划分

// **************************
// 不可动态关闭打印内容

// 代码跟踪
#define PRINTT(fmt,...) _ctprint(C_BLUE  , C_NORMAL, fmt, ##__VA_ARGS__)

// 已调试模式打印
#ifndef NDEBUG
	#define PRINTD(fmt,...)
#else
	#define PRINTD(fmt,...) _ctstprint(C_BLUE  , C_NORMAL, fmt, ##__VA_ARGS__)
#endif

// 特殊高亮显示
#define PRINTW(fmt,...) _ctstprint(C_YELLOW, C_NORMAL, fmt, ##__VA_ARGS__)
#define PRINTA(fmt,...) _ctstprint(C_PURPLE, C_NORMAL, fmt, ##__VA_ARGS__)
#define PRINTE(fmt,...) _ctstprint(C_RED   , C_NORMAL, fmt, ##__VA_ARGS__)


// **************************
// 可动态关闭打印内容， 

#define LEVEL_ID(n,m)	( g_level[(n) * SUB_GROUP + (m)] )
/* 
  level 可选 g_level[0] ~ g_level[LEVEL_GROUP * SUB_GROUP - 1]
  g_level 是全局变量，pl_enable、LEVEL_ID() 控制其内容是否使能

  如下内容可以打印
	LEVEL_ID(0,1) = 1;
  	PRINTL(LEVEL_ID(0,1),"Hello Word");
  关闭打印
  	LEVEL_ID(0,1) = 0;
  	PRINTL(LEVEL_ID(0,1),"Hello Word");
 */
#define PRINTL(level,fmt,...) _printl(level, fmt, ##__VA_ARGS__)

#endif
