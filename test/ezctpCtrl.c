/* ============================================================================
 * Project Name : ezctp Application Programming Interface
 * Module Name  : ezctpCtrl.c
 *
 * Description  : ezctp API for CThostFtdcXXXApi class
 *
 * Copyright (C) 2012 by ezctp-project
 *
 * History      Rev       Description
 * 2012-03-08   0.1       Copy it from ezbox project
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
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/un.h>
#include <fcntl.h>
#include <assert.h>
#include <errno.h>
#include <syslog.h>
#include <ctype.h>
#include <stdarg.h>

#include "ezctpApi.h"
#include "ezcfg-api.h"
#include "ezctp-util.h"


#define MDUSER_CONFIG_FILE_PATH       "/etc/ezctp_mduser.conf"

static void ezctpCtrl_show_usage(void)
{
	printf("Usage: ezctpCtrl [-q] [-c config_file_path] <command> [args]\n");
	printf("\n");
	printf("  [-q]--\n");
	printf("    run in quiet mode\n");
	printf("  [-c config_file_path]--\n");
	printf("    set config file\n");
	printf("  <command>--\n");
	printf("    ezcfg control supported commands\n");
	printf("  [args]--\n");
	printf("    ezcfg control command arguments\n");
	printf("\n");
}

int main(int argc, char **argv)
{
	int rc = 0;
	int i;
	bool quiet_mode = false;
	char conf_path[EZCTP_PATH_SIZE] = MDUSER_CONFIG_FILE_PATH;
	char buf[EZCTP_CTRL_MAX_MESSAGE_SIZE];
	int buf_len;

	if (argc < 2) {
		printf("need more arguments.\n");
		ezctpCtrl_show_usage();
		return -1;
	}

	i = 1;
	if (strcmp(argv[i], "-q") == 0) {
		quiet_mode = true;
		i++;
	}

	/* check -c only if argc > 3 */
	if (argc > 3) {
		if (strcmp(argv[i], "-c") == 0) {
			i++;
			snprintf(conf_path, sizeof(conf_path), "%s", argv[i]);
			i++;
		}
	}

	/* init ezcfg_api */
	if (ezctp_util_init_ezcfg_api(conf_path) == false) {
		printf("ERROR\n");
		return -1;
	}

	buf_len = sizeof(buf);
	rc = ezcfg_api_ctrl_exec(&(argv[i]), buf, buf_len);
	if (quiet_mode == false) {
		if (rc < 0) {
			printf("ERROR\n");
		}
		else {
			printf("%s=%s\n", argv[i], buf);
		}
	}
        return rc;
}
