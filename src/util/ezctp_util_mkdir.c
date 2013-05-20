/* ============================================================================
 * Project Name : ezctp Application Programming Interface
 * Module Name  : util/ezctp_util_mkdir.c
 *
 * Description  : ezctp API for CThostFtdcXXXApi class
 *
 * Copyright (C) 2012 by ezctp-project
 *
 * History      Rev       Description
 * 2012-03-08   0.1       Copy it from ezbox-project
 * ============================================================================
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <assert.h>
#include <errno.h>
#include <syslog.h>
#include <ctype.h>
#include <stdarg.h>

#include "ezctp-util.h"

#if 0
#define DBG(format, args...) do {\
	FILE *dbg_fp = fopen("/tmp/mydbg.ezctp_util_mkdir", "a"); \
	if (dbg_fp) { \
		fprintf(dbg_fp, format, ## args); \
		fclose(dbg_fp); \
	} \
} while(0)
#else
#define DBG(format, args...)
#endif

int ezctp_util_mkdir(const char *path, mode_t mode, bool is_dir)
{
	char buf[EZCTP_PATH_SIZE];
	char *s;

	if (path == NULL)
		return -1;
	if (strlen(path) > (sizeof(buf) - 1))
		return -1;

	snprintf(buf, sizeof(buf), "%s", path);
	s = buf;
	while (*s != '\0') {
		/* skip first '/' */
		while (*s == '/') s++;
		/* find next '/' */
		while ((*s != '/') && (*s != '\0')) s++;
		if (*s == '/') {
			*s = '\0';
			if (mkdir(buf, mode) == -1) {
				if (errno == EEXIST) {
					/* do nothing */
				}
				else {
					return -1;
				}
			}
			*s = '/';
			s++;
		}
	}

	if (is_dir == true) {
		if (mkdir(path, mode) == -1) {
			if (errno == EEXIST) {
				return 0;
			}
			else {
				return -1;
			}
		}
	}
	return 0;
}
