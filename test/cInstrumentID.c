/* ============================================================================
 * Project Name : ezctp Application Programming Interface
 * Module Name  : cSize.c
 *
 * Description  : ezctp API for CThostFtdcXXXApi class
 *
 * Copyright (C) 2012 by ezctp-project
 *
 * History      Rev       Description
 * 2012-03-09   0.1       Write it from scratch
 * ============================================================================
 */

#include <stddef.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
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
#include <sys/poll.h>

#include "ezctpApi.h"
#include "ezcfg-api.h"
#include "ezctp-util.h"

int main(int argc, char **argv)
{
	int rc = EXIT_SUCCESS;
	char buf[256];

	if (ezctp_util_gen_instrument_id_shfe_cu(buf, sizeof(buf), 0) > 0) {
		printf("shfe-cu(0):=[%s]\n", buf);
	}
	
	if (ezctp_util_gen_instrument_id_shfe_al(buf, sizeof(buf), 0) > 0) {
		printf("shfe-al(0):=[%s]\n", buf);
	}
	
	if (ezctp_util_gen_instrument_id_shfe_zn(buf, sizeof(buf), 0) > 0) {
		printf("shfe-zn(0):=[%s]\n", buf);
	}
	
	if (ezctp_util_gen_instrument_id_shfe_pb(buf, sizeof(buf), 0) > 0) {
		printf("shfe-pb(0):=[%s]\n", buf);
	}
	
	if (ezctp_util_gen_instrument_id_shfe_au(buf, sizeof(buf), 0) > 0) {
		printf("shfe-au(0):=[%s]\n", buf);
	}
	
	if (ezctp_util_gen_instrument_id_shfe_rb(buf, sizeof(buf), 0) > 0) {
		printf("shfe-rb(0):=[%s]\n", buf);
	}
	
	if (ezctp_util_gen_instrument_id_shfe_wr(buf, sizeof(buf), 0) > 0) {
		printf("shfe-wr(0):=[%s]\n", buf);
	}
	
	if (ezctp_util_gen_instrument_id_shfe_fu(buf, sizeof(buf), 0) > 0) {
		printf("shfe-fu(0):=[%s]\n", buf);
	}
	
	if (ezctp_util_gen_instrument_id_shfe_ru(buf, sizeof(buf), 0) > 0) {
		printf("shfe-ru(0):=[%s]\n", buf);
	}
	
	if (ezctp_util_gen_instrument_id_czce_ws(buf, sizeof(buf), 0) > 0) {
		printf("czce-ws(0):=[%s]\n", buf);
	}
	
	if (ezctp_util_gen_instrument_id_czce_wt(buf, sizeof(buf), 0) > 0) {
		printf("czce-wt(0):=[%s]\n", buf);
	}
	
	if (ezctp_util_gen_instrument_id_czce_cf(buf, sizeof(buf), 0) > 0) {
		printf("czce-cf(0):=[%s]\n", buf);
	}
	
	if (ezctp_util_gen_instrument_id_czce_sr(buf, sizeof(buf), 0) > 0) {
		printf("czce-sr(0):=[%s]\n", buf);
	}
	
	if (ezctp_util_gen_instrument_id_czce_ta(buf, sizeof(buf), 0) > 0) {
		printf("czce-ta(0):=[%s]\n", buf);
	}
	
	if (ezctp_util_gen_instrument_id_czce_ro(buf, sizeof(buf), 0) > 0) {
		printf("czce-ro(0):=[%s]\n", buf);
	}
	
	if (ezctp_util_gen_instrument_id_czce_er(buf, sizeof(buf), 0) > 0) {
		printf("czce-er(0):=[%s]\n", buf);
	}
	
	if (ezctp_util_gen_instrument_id_czce_me(buf, sizeof(buf), 0) > 0) {
		printf("czce-me(0):=[%s]\n", buf);
	}
	
	if (ezctp_util_gen_instrument_id_dce_c(buf, sizeof(buf), 0) > 0) {
		printf("dce-c(0):=[%s]\n", buf);
	}
	
	if (ezctp_util_gen_instrument_id_dce_a(buf, sizeof(buf), 0) > 0) {
		printf("dce-a(0):=[%s]\n", buf);
	}
	
	if (ezctp_util_gen_instrument_id_dce_b(buf, sizeof(buf), 0) > 0) {
		printf("dce-b(0):=[%s]\n", buf);
	}
	
	if (ezctp_util_gen_instrument_id_dce_m(buf, sizeof(buf), 0) > 0) {
		printf("dce-m(0):=[%s]\n", buf);
	}
	
	if (ezctp_util_gen_instrument_id_dce_y(buf, sizeof(buf), 0) > 0) {
		printf("dce-y(0):=[%s]\n", buf);
	}

	if (ezctp_util_gen_instrument_id_dce_p(buf, sizeof(buf), 0) > 0) {
		printf("dce-p(0):=[%s]\n", buf);
	}

	if (ezctp_util_gen_instrument_id_dce_l(buf, sizeof(buf), 0) > 0) {
		printf("dce-l(0):=[%s]\n", buf);
	}

	if (ezctp_util_gen_instrument_id_dce_v(buf, sizeof(buf), 0) > 0) {
		printf("dce-v(0):=[%s]\n", buf);
	}

	if (ezctp_util_gen_instrument_id_dce_j(buf, sizeof(buf), 0) > 0) {
		printf("dce-j(0):=[%s]\n", buf);
	}

	return (rc);
}
