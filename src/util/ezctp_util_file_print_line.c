/* ============================================================================
 * Project Name : ezctp Application Programming Interface
 * Module Name  : util/ezctp_util_file_get_line.c
 *
 * Description  : ezctp API for CThostFtdcXXXApi class
 *
 * Copyright (C) 2012 by ezctp-project
 *
 * History      Rev       Description
 * 2012-02-29   0.1       Copy it from ezbox-project
 * ============================================================================
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <syslog.h>
#include <ctype.h>
#include <stdarg.h>

#include "ezctp-util.h"

bool ezctp_util_file_print_line(FILE *fp, char *buf, size_t size, const char *fmt, ...)
{
	va_list args;

	if ((fp == NULL) || (buf == NULL) || (size < 1)) {
		return false;
	}

	va_start(args, fmt);
	vsnprintf(buf, size, fmt, args);
	va_end(args);
	va_start(args, fmt);
	fprintf(fp, "%s", buf);
	va_end(args);

	return true;
}
