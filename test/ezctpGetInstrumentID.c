/* ============================================================================
 * Project Name : ezctp Application Programming Interface
 * Module Name  : ezctpGetMarketData.c
 *
 * Description  : ezctp API for CThostFtdcXXXApi class
 *
 * Copyright (C) 2012 by ezctp-project
 *
 * History      Rev       Description
 * 2012-03-16   0.1       Write it from scratch
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

#include <sys/ipc.h>
#include <sys/shm.h>

#include "ezctpApi.h"
#include "ezcfg-api.h"
#include "ezctp-util.h"


#define TRADER_PRIORITY               0
#define VERSION                       "0.1"

/* minimum number of worker threads */
/* ctrl socket, nvram socket, uevent socket */
#define EZCFG_THREAD_MIN_NUM    3

#define handle_error_en(en, msg) \
	do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

#if 1
#define DBG(format, args...) do {\
	char path[256]; \
	FILE *dbg_fp; \
	snprintf(path, 256, "/tmp/%d-debug.txt", getpid()); \
        dbg_fp = fopen(path, "a"); \
	if (dbg_fp) { \
		fprintf(dbg_fp, format, ## args); \
		fclose(dbg_fp); \
	} \
} while(0)
#else
#define DBG(format, args...)
#endif

#define INFO(format, args...) do {\
	char path[256]; \
	FILE *info_fp; \
	snprintf(path, 256, "/tmp/%d-debug.txt", getpid()); \
        info_fp = fopen(path, "a"); \
	if (info_fp) { \
		fprintf(info_fp, format, ## args); \
		fclose(info_fp); \
	} \
} while(0)

static bool debug = false;
static int rc = EXIT_FAILURE;
static pthread_t root_thread;

static struct ezctp_TraderDataField myTraderData;
static char ezctp_ConfPath[EZCTP_PATH_SIZE];
static char *ezctp_InstrumentIDArray[128];
static char ezctp_InstrumentIDString[1024];

static int ctp_start(char *pConfPath, struct ezctp_TraderDataField *pData)
{
	char buf[1024];
	int i, rc;
	char *p;

	if ((pConfPath == NULL) || (pData == NULL)) {
		printf("pConfPath or pData = NULL!\n");
		exit(EXIT_FAILURE);
	}

	memset(pData, 0, sizeof(struct ezctp_TraderDataField));

	/* first get pszFlowPath */
	rc = ezctp_util_get_conf_string(pConfPath, "flow_path", buf, sizeof(buf));
	if (rc > 0) {
		if (buf[0] == '/') {
			if (ezctp_util_mkdir(buf, 0777, false) == -1) {
				fprintf(stderr, "%s ezctp_util_mkdir.\n", buf);
				exit(EXIT_FAILURE);
			}
		}
		pData->pTraderApi = ezctp_trader_CreateFtdcTraderApi(buf);
	}
	else {
		printf("flow_path error rc=[%d]!\n", rc);
		pData->pTraderApi = ezctp_trader_CreateFtdcTraderApi("");
	}
	if (pData->pTraderApi == NULL) {
		printf("pTraderApi = NULL!\n");
		exit(EXIT_FAILURE);
	}

	/* first clean SPI for API */
	ezctp_trader_RegisterSpi(pData->pTraderApi, NULL);

	/* then prepare pData for SPI */
	/* Broker ID */
	rc = ezctp_util_get_conf_string(pConfPath, "broker_id", buf, sizeof(buf));
	if (rc < 1) {
		printf("broker_id error rc=[%d]!\n", rc);
		exit(EXIT_FAILURE);
	}
	snprintf(pData->BrokerID, sizeof(TThostFtdcBrokerIDType), "%s", buf);

	/* User ID */
	rc = ezctp_util_get_conf_string(pConfPath, "user_id", buf, sizeof(buf));
	if (rc < 1) {
		printf("user_id error rc=[%d]!\n", rc);
		exit(EXIT_FAILURE);
	}
	snprintf(pData->UserID, sizeof(TThostFtdcUserIDType), "%s", buf);

	/* Investor ID */
	rc = ezctp_util_get_conf_string(pConfPath, "investor_id", buf, sizeof(buf));
	if (rc < 1) {
		printf("user_id error rc=[%d]!\n", rc);
		exit(EXIT_FAILURE);
	}
	snprintf(pData->InvestorID, sizeof(TThostFtdcInvestorIDType), "%s", buf);

	/* Password */
	rc = ezctp_util_get_conf_string(pConfPath, "password", buf, sizeof(buf));
	if (rc < 1) {
		printf("user_id error rc=[%d]!\n", rc);
		exit(EXIT_FAILURE);
	}
	snprintf(pData->Password, sizeof(TThostFtdcPasswordType), "%s", buf);

	/* InstrumentID */
	rc = ezctp_util_get_conf_string(pConfPath, "instrument_id", ezctp_InstrumentIDString, sizeof(ezctp_InstrumentIDString));
	if (rc < 1) {
		printf("instrument_id error rc=[%d]!\n", rc);
		exit(EXIT_FAILURE);
	}
	p = ezctp_InstrumentIDString;
	for (i = 0; (p != NULL) && (i < 128); i++) {
		ezctp_InstrumentIDArray[i] = p;
		p = strchr(p, ',');
		if (p != NULL) {
			*p = '\0';
			p++;
		}
	}
	if (p != NULL) {
		printf("instrument_id too large!\n");
		exit(EXIT_FAILURE);
	}
	pData->iInstrumentID = i;
	pData->ppInstrumentID = ezctp_InstrumentIDArray;

	/* RequestID */
	rc = ezctp_util_get_conf_string(pConfPath, "request_id", buf, sizeof(buf));
	if (rc < 1) {
		printf("request_id error rc=[%d]!\n", rc);
		exit(EXIT_FAILURE);
	}
	pData->iRequestID = atoi(buf);

	/* Front Address */
	rc = ezctp_util_get_conf_string(pConfPath, "front_address", buf, sizeof(buf));
	if (rc < 1) {
		printf("front_address error rc=[%d]!\n", rc);
		exit(EXIT_FAILURE);
	}
	ezctp_trader_RegisterFront(pData->pTraderApi, buf);

	/* register SPI */
	pData->pTraderSpi = ezctp_trader_CreateCTraderSpi(pData);
	if (pData->pTraderSpi == NULL) {
		printf("pTraderSpi = NULL!\n");
		/* release Trader API */
		ezctp_trader_Release(pData->pTraderApi);
		pData->pTraderApi = NULL;
		return (EXIT_FAILURE);
	}
	ezctp_trader_RegisterSpi(pData->pTraderApi, pData->pTraderSpi);

	ezctp_trader_Init(pData->pTraderApi);
	return (EXIT_SUCCESS);
}

static int ctp_stop(struct ezctp_TraderDataField *pData)
{
	if (pData->pTraderApi != NULL) {
		ezctp_trader_RegisterSpi(pData->pTraderApi, NULL);
	}

	if (pData->pTraderSpi != NULL) {
		ezctp_trader_ReleaseCTraderSpi(pData->pTraderSpi);
		pData->pTraderSpi = NULL;
	}

	if (pData->pTraderApi != NULL) {
		ezctp_trader_Release(pData->pTraderApi);
		pData->pTraderApi = NULL;
	}

	return (EXIT_SUCCESS);
}

static void ezctp_trader_show_usage(void)
{
	printf("Usage: ezctpGetInstrumentID -c config_file_path [-d] [-D] [-t max_worker_threads]\n");
	printf("\n");
	printf("  -c\tcofig file path\n");
	printf("  -d\tdaemonize\n");
	printf("  -D\tdebug mode\n");
	printf("  -t\tmax worker threads\n");
	printf("  -h\thelp\n");
	printf("\n");
}

static int mem_size_mb(void)
{
	FILE *fp;
	char buf[1024];
	long int memsize = -1;

	fp = fopen("/proc/meminfo", "r");
	if (fp == NULL)
		return -1;

	while (fgets(buf, sizeof(buf), fp) != NULL) {
		long int value;

		if (sscanf(buf, "MemTotal: %ld kB", &value) == 1) {
			memsize = value / 1024;
			break;
		}
	}

	fclose(fp);
	return memsize;
}

static void *sig_thread_routine(void *arg)
{
	sigset_t *set = (sigset_t *) arg;
	int s, sig;

	for (;;) {
		s = sigwait(set, &sig);
		if (s != 0) {
			DBG("<6>ezctpGetInstrumentID: sigwait errno = [%d]\n", s);
			continue;
		}
		DBG("<6>ezctpGetInstrumentID: Signal handling thread got signal %d\n", sig);
		switch(sig) {
		case SIGTERM :
			ctp_stop(&myTraderData);
			ezctp_util_release_ezcfg_api();
			rc = EXIT_SUCCESS;
			break;
		case SIGUSR1 :
			DBG("<6>ezctpGetInstrumentID: [%d]\n", sig);
			break;
		case SIGCHLD :
			/* do nothing for child exit */
			break;
		default :
			DBG("<6>ezctpGetInstrumentID: unknown signal [%d]\n", sig);
			break;
		}
	}

	return NULL;
}

int main(int argc, char **argv)
{
	bool daemonize = false;
	int fd = -1;
	int threads_max = 0;
	int s;
	pthread_t thread;
	sigset_t sigset;
	struct ezcfg *ezcfg = NULL;
	char *p;

	/* clean config file path */
	ezctp_ConfPath[0] = '\0';

	for (;;) {
		int c;
		c = getopt( argc, argv, "c:dDht:");
		if (c == EOF) break;
		switch (c) {
			case 'c':
				snprintf(ezctp_ConfPath, sizeof(ezctp_ConfPath), "%s", optarg);
				break;
			case 'd':
				daemonize = true;
				break;
			case 'D':
				debug = true;
				break;
			case 't':
				threads_max = atoi(optarg);
				break;
			case 'h':
				ezctp_trader_show_usage();
				return (EXIT_SUCCESS);
			default:
				ezctp_trader_show_usage();
				return (EXIT_FAILURE);
		}
        }

	if (ezctp_ConfPath[0] == '\0') {
		ezctp_trader_show_usage();
		return (EXIT_FAILURE);
	}

	ezcfg = ezcfg_api_common_new(ezctp_ConfPath);
	if (ezcfg == NULL) {
		fprintf(stderr, "%s format error.\n", ezctp_ConfPath);
		exit(EXIT_FAILURE);
	}

	p = ezcfg_api_common_get_root_path(ezcfg);
	if ((p != NULL) && (*p == '/')) {
		if (ezctp_util_mkdir(p, 0777, true) == -1) {
			fprintf(stderr, "%s ezctp_util_mkdir.\n", p);
			ezcfg_api_common_delete(ezcfg);
			exit(EXIT_FAILURE);
		}
	}

	p = ezcfg_api_common_get_sem_ezcfg_path(ezcfg);
	if ((p == NULL) || (*p == '\0')) {
		fprintf(stderr, "%s no sem_ezcfg_path.\n", ezctp_ConfPath);
		ezcfg_api_common_delete(ezcfg);
		exit(EXIT_FAILURE);
	}
	if ((p != NULL) && (*p == '/')) {
		if (ezctp_util_mkdir(p, 0777, false) == -1) {
			fprintf(stderr, "%s ezctp_util_mkdir.\n", p);
			ezcfg_api_common_delete(ezcfg);
			exit(EXIT_FAILURE);
		}
		fd = open(p, O_CREAT|O_RDWR, S_IRWXU);
		if (fd < 0) {
			fprintf(stderr, "cannot open %s\n", p);
			ezcfg_api_common_delete(ezcfg);
			exit(EXIT_FAILURE);
		}
		close(fd);
	}

	p = ezcfg_api_common_get_shm_ezctp_path(ezcfg);
	if ((p == NULL) || (*p == '\0')) {
		fprintf(stderr, "%s no shm_ezctp_path.\n", ezctp_ConfPath);
		ezcfg_api_common_delete(ezcfg);
		exit(EXIT_FAILURE);
	}
	if ((p != NULL) && (*p == '/')) {
		if (ezctp_util_mkdir(p, 0777, false) == -1) {
			fprintf(stderr, "%s ezctp_util_mkdir.\n", p);
			ezcfg_api_common_delete(ezcfg);
			exit(EXIT_FAILURE);
		}
		fd = open(p, O_CREAT|O_RDWR, S_IRWXU);
		if (fd < 0) {
			fprintf(stderr, "cannot open %s\n", p);
			ezcfg_api_common_delete(ezcfg);
			exit(EXIT_FAILURE);
		}
		close(fd);
	}

	ezcfg_api_common_delete(ezcfg);
	ezcfg = NULL;

	/* before opening new files, make sure std{in,out,err} fds are in a sane state */
	fd = open("/dev/null", O_RDWR);
	if (fd < 0) {
		fprintf(stderr, "cannot open /dev/null\n");
		exit(EXIT_FAILURE);
	}
	if (write(STDOUT_FILENO, 0, 0) < 0)
		dup2(fd, STDOUT_FILENO);
	if (write(STDERR_FILENO, 0, 0) < 0)
		dup2(fd, STDERR_FILENO);

	if (!debug) {
		dup2(fd, STDIN_FILENO);
		dup2(fd, STDOUT_FILENO);
		dup2(fd, STDERR_FILENO);
	}
	if (fd > STDERR_FILENO)
		close(fd);

	if (daemonize)
	{
		pid_t pid = fork();
		switch (pid) {
		case 0:
			/* child process */
			break;

		case -1:
			/* error */
			return (EXIT_FAILURE);

		default:
			/* parant process */
			return (EXIT_SUCCESS);
		}
	}

        /* set scheduling priority for the main daemon process */
	setpriority(PRIO_PROCESS, 0, TRADER_PRIORITY);

	setsid();

	/* main process */
	INFO("<6>ezctpGetInstrumentID: booting...\n");
	/* prepare signal handling thread */
	sigemptyset(&sigset);
	sigaddset(&sigset, SIGCHLD);
	sigaddset(&sigset, SIGUSR1);
	sigaddset(&sigset, SIGTERM);
	s = pthread_sigmask(SIG_BLOCK, &sigset, NULL);
	if (s != 0) {
		DBG("<6>ezctpGetMarketData: pthread_sigmask\n");
		handle_error_en(s, "pthread_sigmask");
	}

	/* get root thread id */
	root_thread = pthread_self();

	s = pthread_create(&thread, NULL, &sig_thread_routine, (void *) &sigset);
	if (s != 0) {
		DBG("<6>ezctpGetMarketData: pthread_create\n");
		handle_error_en(s, "pthread_create");
	}

	if (threads_max < EZCFG_THREAD_MIN_NUM) {
		int memsize = mem_size_mb();

		/* set value depending on the amount of RAM */
		if (memsize > 0)
			threads_max = EZCFG_THREAD_MIN_NUM + (memsize / 8);
		else
			threads_max = EZCFG_THREAD_MIN_NUM;
	}

	if (ezctp_util_init_ezcfg_api(ezctp_ConfPath) == false) {
		DBG("<6>ezctp: init ezcfg_api failed\n");
		return (EXIT_FAILURE);
	};

	if (ctp_start(ezctp_ConfPath, &myTraderData) == EXIT_SUCCESS) {
		INFO("<6>ezctpGetInstrumentID: starting version " VERSION "\n");

		/* wait SPI thread exit */
		ezctp_trader_Join(myTraderData.pTraderApi);

		while ((myTraderData.pTraderApi != NULL) ||
		       (myTraderData.pTraderSpi != NULL)) {
			printf("myTraderData.pTraderApi=[%ld]\n", (long int)myTraderData.pTraderApi);
			printf("myTraderData.pTraderSpi=[%ld]\n", (long int)myTraderData.pTraderSpi);
			sleep(1);
		}
	}
	else {
		INFO("<6>ezctpGetInstrumentID: fail to start" VERSION "\n");
		ezctp_util_release_ezcfg_api();
		rc = EXIT_FAILURE;
	}

	/* wait for exit signal */
	return (rc);
}
