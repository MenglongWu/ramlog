/**
 ******************************************************************************
 * @file	bb.h
 * @brief	busybox精简头文件


*/

#ifndef _BB_H_
#define _BB_H_

#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

/* DO NOT CHANGE THESE VALUES!  cp.c, mv.c, install.c depend on them. */
#define FILEUTILS_PRESERVE_STATUS (1 << 0) /* -p */
#define FILEUTILS_DEREFERENCE     (1 << 1) /* !-d */
#define FILEUTILS_RECUR           (1 << 2) /* -R */
#define FILEUTILS_FORCE           (1 << 3) /* -f */
#define FILEUTILS_INTERACTIVE     (1 << 4) /* -i */
#define FILEUTILS_MAKE_HARDLINK   (1 << 5) /* -l */
#define FILEUTILS_MAKE_SOFTLINK   (1 << 6) /* -s */
#define FILEUTILS_DEREF_SOFTLINK  (1 << 7) /* -L */
#define FILEUTILS_DEREFERENCE_L0  (1 << 8) /* -H */
#if ENABLE_SELINUX
	#define FILEUTILS_PRESERVE_SECURITY_CONTEXT (1 << 9) /* -c */
	#define FILEUTILS_SET_SECURITY_CONTEXT (1 << 10)
#endif
#define FILEUTILS_IGNORE_CHMOD_ERR (1 << 11)
/* -v */
#define FILEUTILS_VERBOSE         ((1 << 12) * ENABLE_FEATURE_VERBOSE,



#define LONE_CHAR(s,c)     ((s)[0] == (c) && !(s)[1])
#define NOT_LONE_CHAR(s,c) ((s)[0] != (c) || (s)[1])

#endif