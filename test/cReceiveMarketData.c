/* ============================================================================
 * Project Name : ezctp Application Programming Interface
 * Module Name  : cReceiveMarketData.c
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


/* front address */
char FRONT_ADDR[] = "tcp://asp-sim2-md1.financial-trading-platform.com:26213";
TThostFtdcBrokerIDType BROKER_ID = "2030";
TThostFtdcUserIDType USER_ID = "00092";
TThostFtdcInvestorIDType INVESTOR_ID = "00092";
TThostFtdcPasswordType PASSWORD = "888888";
char *ppInstrumentID[] = {
	"if1203",
#if 1
	"cu1203",
	"cu1204",
	"cu1205",
	"cu1206",
	"cu1207",
	"cu1208",
	"cu1209",
	"cu1210",
	"cu1211",
	"cu1212",
	"cu1301",
	"cu1302",
	"cu1303",
	"au1203",
	"au1204",
	"au1205",
	"au1206",
	"au1207",
	"au1208",
	"au1209",
	"au1210",
	"au1211",
	"au1212",
	"au1301",
	"au1302",
	"au1303",
#endif
	NULL
};

int iInstrumentID = 0;
int iRequestID = 0;

#define MDUSER_PRIORITY               -4
#define MDUSER_CONFIG_FILE_PATH       "/etc/ezctp_mduser.conf"
#define VERSION                       "0.1"

/* minimum number of worker threads */
/* ctrl socket, nvram socket, uevent socket */
#define EZCFG_THREAD_MIN_NUM    3

#define handle_error_en(en, msg) \
	do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

#if 1
#define DBG(format, args...) do {\
	FILE *dbg_fp = fopen("/dev/kmsg", "a"); \
	if (dbg_fp) { \
		fprintf(dbg_fp, format, ## args); \
		fclose(dbg_fp); \
	} \
} while(0)
#else
#define DBG(format, args...)
#endif

#define INFO(format, args...) do {\
	FILE *info_fp = fopen("/dev/kmsg", "a"); \
	if (info_fp) { \
		fprintf(info_fp, format, ## args); \
		fclose(info_fp); \
	} \
} while(0)

static bool debug = false;
static int rc = EXIT_FAILURE;
static pthread_t root_thread;

#if 0
void *pMdUserApi = NULL;
void *pMdUserSpi = NULL;
#endif

static struct ezctp_MdUserDataField myMdUserData;

static int ctp_start(struct ezctp_MdUserDataField *pData)
{
	memset(pData, 0, sizeof(struct ezctp_MdUserDataField));

	pData->pMdUserApi = ezctp_md_CreateFtdcMdApi("", false, false);
	if (pData->pMdUserApi == NULL) {
		printf("pMdUserApi = NULL!\n");
		exit(EXIT_FAILURE);
	}

	/* first clean SPI for API */
	ezctp_md_RegisterSpi(pData->pMdUserApi, NULL);

	/* then prepare pData for SPI */
	strcpy(pData->BrokerID, BROKER_ID);
	strcpy(pData->UserID, USER_ID);
	strcpy(pData->InvestorID, INVESTOR_ID);
	strcpy(pData->Password, PASSWORD);
	pData->ppInstrumentID = ppInstrumentID;
	for (iInstrumentID = 0; ppInstrumentID[iInstrumentID] != NULL; iInstrumentID++) { ; }
	pData->iInstrumentID = iInstrumentID;
	pData->iRequestID = iRequestID;

	pData->pMdUserSpi = ezctp_md_CreateCMdUserSpi(pData);
	if (pData->pMdUserSpi == NULL) {
		printf("pMdUserSpi = NULL!\n");
		goto func_fail;
	}

	ezctp_md_RegisterSpi(pData->pMdUserApi, pData->pMdUserSpi);
	ezctp_md_RegisterFront(pData->pMdUserApi, FRONT_ADDR);
	ezctp_md_Init(pData->pMdUserApi);
	return (EXIT_SUCCESS);

func_fail:
	/* release MdUser API */
	ezctp_md_Release(pData->pMdUserApi);
	pData->pMdUserApi = NULL;
	return (EXIT_FAILURE);
}

static int ctp_stop(struct ezctp_MdUserDataField *pData)
{
	ezctp_md_RegisterSpi(pData->pMdUserApi, NULL);
	sleep(1);
	ezctp_md_Release(pData->pMdUserApi);
	pData->pMdUserApi = NULL;

	ezctp_md_ReleaseCMdUserSpi(pData->pMdUserSpi);
	pData->pMdUserSpi = NULL;
	sleep(1);

	return (EXIT_SUCCESS);
}

static void ezctp_mduser_show_usage(void)
{
	printf("Usage: ezctp_mduser [-d] [-D] [-c config_file_path] [-t max_worker_threads]\n");
	printf("\n");
	printf("  -d\tdaemonize\n");
	printf("  -D\tdebug mode\n");
	printf("  -c\tcofig file path\n");
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

static void *sig_thread(void *arg)
{
	sigset_t *set = (sigset_t *) arg;
	int s, sig;

	for (;;) {
		s = sigwait(set, &sig);
		if (s != 0) {
			DBG("<6>ezctp_mduser: sigwait errno = [%d]\n", s);
			continue;
		}
		DBG("<6>ezctp_mduser: Signal handling thread got signal %d\n", sig);
		switch(sig) {
		case SIGTERM :
			ctp_stop(&myMdUserData);
			sleep(1);
			ezctp_util_release_ezcfg_api();
			sleep(1);
			rc = EXIT_SUCCESS;
			s = pthread_kill(root_thread, SIGHUP);
			break;
		case SIGUSR1 :
			DBG("<6>ezctp_mduser: [%d]\n", sig);
			break;
		case SIGCHLD :
			/* do nothing for child exit */
			break;
		default :
			DBG("<6>ezctp_mduser: unknown signal [%d]\n", sig);
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
	char conf_path[EZCTP_PATH_SIZE] = MDUSER_CONFIG_FILE_PATH;

	for (;;) {
		int c;
		c = getopt( argc, argv, "c:dDht:");
		if (c == EOF) break;
		switch (c) {
			case 'c':
				snprintf(conf_path, sizeof(conf_path), "%s", optarg);
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
			default:
				ezctp_mduser_show_usage();
				return (EXIT_FAILURE);
		}
        }

	ezcfg = ezcfg_api_common_new(conf_path);
	if (ezcfg == NULL) {
		fprintf(stderr, "%s format error.\n", conf_path);
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
		fprintf(stderr, "%s no sem_ezcfg_path.\n", conf_path);
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
		fprintf(stderr, "%s no shm_ezctp_path.\n", conf_path);
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
	setpriority(PRIO_PROCESS, 0, MDUSER_PRIORITY);

	setsid();

	/* main process */
	INFO("<6>ezctp_mduser: booting...\n");
	/* prepare signal handling thread */
	sigemptyset(&sigset);
	sigaddset(&sigset, SIGCHLD);
	sigaddset(&sigset, SIGUSR1);
	sigaddset(&sigset, SIGTERM);
	s = pthread_sigmask(SIG_BLOCK, &sigset, NULL);
	if (s != 0) {
		DBG("<6>ezctp_mduser: pthread_sigmask\n");
		handle_error_en(s, "pthread_sigmask");
	}

	/* get root thread id */
	root_thread = pthread_self();

	s = pthread_create(&thread, NULL, &sig_thread, (void *) &sigset);
	if (s != 0) {
		DBG("<6>ezctp_mduser: pthread_create\n");
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

	if (ezctp_util_init_ezcfg_api(conf_path) == false) {
		DBG("<6>ezctp: init ezcfg_api failed\n");
		return (EXIT_FAILURE);
	};

	if (ctp_start(&myMdUserData) == EXIT_SUCCESS) {
		INFO("<6>ezctp_mduser: starting version " VERSION "\n");

		/* wait SPI thread exit */
		ezctp_md_Join(myMdUserData.pMdUserApi);
	}
	else {
		INFO("<6>ezctp_mduser: fail to start" VERSION "\n");
		rc = EXIT_FAILURE;
		sleep(3);
	}

	/* wait for exit signal, should never run to this place */
	return (rc);
}
