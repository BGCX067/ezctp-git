/* ============================================================================
 * Project Name : ezctp Application Programming Interface
 * Module Name  : util/ezctp_util_init_ezcfg_api.c
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
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/shm.h>
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

#if 0
#define DBG(format, args...) do { \
	FILE *dbg_fp = fopen("/dev/kmsg", "a"); \
	if (dbg_fp) { \
		fprintf(dbg_fp, format, ## args); \
		fclose(dbg_fp); \
	} \
} while(0)
#else
#define DBG(format, args...)
#endif


extern void *ezctp_common_shm_ezcfg_addr;

bool ezctp_util_init_ezcfg_api(const char *path)
{
	int shm_id = -1;
	int rc;

	if (path == NULL) {
		return false;
	}

	/* ezcfg_api_common */
	if (ezcfg_api_common_set_config_file(path) < 0) {
		return false;
	}

	/* init ezctp_common_shm_ezcfg_addr */
	rc = ezcfg_api_ipc_get_ezcfg_shm_id(&shm_id);
	if (rc < 0) {
		return false;
	}

	if (shm_id == -1) {
		return false;
	}


	ezctp_common_shm_ezcfg_addr = shmat(shm_id, NULL, 0);
	if ((ezctp_common_shm_ezcfg_addr == NULL) || (ezctp_common_shm_ezcfg_addr == (void *)-1)) {
		return false;
	}

	return true;
}
