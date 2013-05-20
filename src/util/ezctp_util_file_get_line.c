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

bool ezctp_util_file_get_line(FILE *fp, char *buf, size_t size, char *comment, char *rm)
{
	if ((fp == NULL) || (buf == NULL) || (size < 1)) {
		return false;
	}

	while(fgets(buf, size, fp) != NULL)
	{
		if(strchr(comment, buf[0]) == NULL)
		{
			int len = strlen(buf);

			while((len > 0) && 
			      (buf[len] == '\0' || 
			       (strchr(rm, buf[len]) != NULL)))
			{
				buf[len] = '\0';
				len --;
			}

			if (len > 0)
				return true;
		}
	}
	return false;
}
