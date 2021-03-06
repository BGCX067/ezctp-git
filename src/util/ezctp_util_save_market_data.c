/* ============================================================================
 * Project Name : ezctp Application Programming Interface
 * Module Name  : util/ezctp_util_remove_market_data.c
 *
 * Description  : ezctp API for CThostFtdcXXXApi class
 *
 * Copyright (C) 2012 by ezctp-project
 *
 * History      Rev       Description
 * 2012-03-30   0.1       Write it from scratch
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
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mount.h>
#include <sys/un.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <pthread.h>
#include <errno.h>
#include <syslog.h>
#include <ctype.h>
#include <stdarg.h>

#include "ezcfg-api.h"
#include "ezctp-util.h"

extern void *ezctp_common_shm_ezcfg_addr;

/* save market data which is [size] bytes per entry from shared memory to file,
 * to file pointed by [fp] */
bool ezctp_util_save_market_data(FILE *fp, size_t size, int flag)
{
	if ((fp == NULL) || (size < 1)) {
		return false;
	}

	if ((ezctp_common_shm_ezcfg_addr == NULL) || (ezctp_common_shm_ezcfg_addr == (void *)-1)) {
		return false;
	}

	/* ezcfg_api_ezctp */
	if (ezcfg_api_ezctp_save_market_data(ezctp_common_shm_ezcfg_addr, fp, size, flag) == false) {
		return false;
	}

	return true;
}
