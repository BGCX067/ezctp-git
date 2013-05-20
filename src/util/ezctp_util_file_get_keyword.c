/* ============================================================================
 * Project Name : ezctp Application Programming Interface
 * Module Name  : util/ezctp_util_file_get_keyword.c
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

#if 0
#define DBG(format, args...) do {\
	FILE *fp = fopen("/dev/kmsg", "a"); \
	if (fp) { \
		fprintf(fp, format, ## args); \
		fclose(fp); \
	} \
} while(0)
#else
#define DBG(format, args...)
#endif

char *ezctp_util_file_get_keyword_by_index(char *filename, char *keyword, int idx)
{
        FILE *file;
	char *p = NULL;
	char *q = NULL;
	char *v = NULL;
	int i;
	char buf[1024];

	/* open file */
	file = fopen(filename, "r");
	if (file == NULL)
		return NULL;

	memset(buf, 0, sizeof(buf));

	while (ezctp_util_file_get_line(file, buf, sizeof(buf), "#", LINE_TAIL_STRING) == true) {

		q = strstr(buf, keyword);
		if (q == NULL)
			continue;

		/* skip key word length */
		p = q + strlen(keyword);
		q = strchr(p, ' ');
		if (q != NULL)
			*q = '\0';

		if (idx > 0) {
			/* get index part string */
			for (i = 1; (i < idx) && (p != NULL); i++) {
				p = strchr(p, ',');
				if (p != NULL)
					p++;
			}

			/* split string */
			if (p != NULL) {
				q = strchr(p, ',');
				if (q != NULL)
					*q = '\0';
			}
		}

		/* found the keyword string, stop searching */
		break;
	}

	fclose(file);
	if (p != NULL) {
		v = strdup(p);
	}
	return (v);
}

char *ezctp_util_file_get_keyword(char *filename, char *keyword)
{
	return ezctp_util_file_get_keyword_by_index(filename, keyword, 0);
}
