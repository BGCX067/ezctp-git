/* ============================================================================
 * Project Name : ezctp Application Programming Interface
 * Module Name  : util/ezctp_util_gen_instrument_id_shfe_ru.c
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
#include <time.h>

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

#define EZCTP_INSTRUMENT_ID_SHFE_RU_STRING	"ru"

/* month 1, 3, 4, 5, 6, 7, 8, 9, 10, 11 */
int ezctp_util_gen_instrument_id_shfe_ru(char *buf, size_t len, int idx)
{
	size_t count = 0;
	int i;
	time_t t;
	struct tm tm;
	int ret;

	if (idx > 0) {
		if (((idx % 100) > 11) || (idx == 2)) {
			return -1;
		}
		/* for single instrument id */
		count = snprintf(buf, len, "%s%d", EZCTP_INSTRUMENT_ID_SHFE_RU_STRING, idx);
		if (count >= len) {
			return -1;
		}
	}
	else {
		/* for full instrument id */
		if (time(&t) == ((time_t) -1)) {
			return -1;
		}
		if (gmtime_r(&t, &tm) == NULL) {
			return -1;
		}
		count = 0;
		for(i = 0; i < 13; i++) {
			idx = tm.tm_year + (tm.tm_mon + i)/12 - (2000 - 1900);
			idx = idx * 100; /* year part */
			idx = idx + (tm.tm_mon + i)%12 + 1; /* month part */
			/* skip month 2 and month 12 */
			if ((idx % 10) == 2) {
				continue;
			}
			ret = snprintf(buf+count, len-count, "%s%d", EZCTP_INSTRUMENT_ID_SHFE_RU_STRING, idx);
			if (ret < 0) {
				return -1;
			}
			count += ret;
			if (count >= len) {
				return -1;
			}
			/* skip comma with next month is 2 or 12 */
			idx++;
			if ((i == 11) && ((idx % 10) == 2)) {
				continue;
			}
			if (i < 12) {
				ret = snprintf(buf+count, len-count, ",");
				if (ret < 0) {
					return -1;
				}
				count += ret;
				if (count >= len) {
					return -1;
				}
			}
		}
	}
	return count;
}
