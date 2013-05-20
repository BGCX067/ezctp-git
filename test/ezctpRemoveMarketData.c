/* ============================================================================
 * Project Name : ezctp Application Programming Interface
 * Module Name  : ezctpRemoveMarketData.c
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

//#include <sqlite3.h>

#include "ezctpApi.h"
#include "ezcfg-api.h"
#include "ezctp-util.h"


#define MDUSER_PRIORITY               0
#define VERSION                       "0.1"

#define DB_SYNC_INTERVAL              60
#define DB_ROOT_PATH                  "/var/tmpfs/lib/ezctp"

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
/* 0: stopped, 1:running, 2:stopping */
static int run_state = 0;

static char ezctp_ConfPath[EZCTP_PATH_SIZE];
static char ezctp_RawFilePath[EZCTP_PATH_SIZE];

static void ezctp_mduser_show_usage(void)
{
	printf("Usage: ezctpSaveMarketData -c config_file_path -r raw_file_path [-d] [-D] [-t max_worker_threads]\n");
	printf("\n");
	printf("  -c\tcofig file path\n");
	printf("  -d\tdaemonize\n");
	printf("  -D\tdebug mode\n");
	printf("  -r\traw data file path\n");
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

static void *save_thread_routine(void *arg)
{
	void *data;
	size_t n, size;
	size_t i;
	struct CThostFtdcDepthMarketDataField *pDepthMarketData;
	FILE *raw_fp = NULL;
#if 0
	char db_path[EZCTP_PATH_SIZE];
	char sql[8192];
	sqlite3 *db = NULL;
	int rc;
	struct stat stat_buf;
	bool need_create_db_table = false;
	char *zErr;
#endif

	/* tag save thread being running */
	run_state = 1;
	raw_fp = fopen(ezctp_RawFilePath, "a+b");
	if (raw_fp == NULL) {
		//printf("%s(%d) can't open [%s]\n", __func__, __LINE__, ezctp_RawFilePath);
	}

	/* do the save task */
	while (run_state == 1) {
		//printf("%s(%d)\n", __func__, __LINE__);
		if (ezctp_util_remove_market_data(&data, &n, &size) == true) {
			//printf("%s(%d) n=[%lu], size=[%lu]\n", __func__, __LINE__, n, size);
			for(i = 0; i < n; i++) {
				pDepthMarketData = (struct CThostFtdcDepthMarketDataField *)(data + (i * size));
				/* first save to raw file */
				if (raw_fp != NULL) {
					fwrite(pDepthMarketData, sizeof(struct CThostFtdcDepthMarketDataField), 1, raw_fp);
				}

#if 0
				/* then save to SQLite DB */
				snprintf(db_path, sizeof(db_path), "%s/%s.db", DB_ROOT_PATH, pDepthMarketData->InstrumentID);
				//printf("%s(%d) db_path=[%s]\n", __func__, __LINE__, db_path);
				/* check if it's the first time create database */
				if (stat(db_path, &stat_buf) != 0) {
					need_create_db_table = true;
					//printf("%s(%d) stat(db_path, &stat_buf) != 0 need_create_db_table=[%d]\n", __func__, __LINE__, need_create_db_table);
				}
				else if (S_ISREG(stat_buf.st_mode)) {
					need_create_db_table = false;
					//printf("%s(%d) S_ISREG(stat_buf.st_mode) need_create_db_table=[%d]\n", __func__, __LINE__, need_create_db_table);
				}
				else {
					need_create_db_table = true;
					//printf("%s(%d) !S_ISREG(stat_buf.st_mode) need_create_db_table=[%d]\n", __func__, __LINE__, need_create_db_table);
				}
				
				rc = sqlite3_open_v2(db_path, &db, SQLITE_OPEN_READWRITE|SQLITE_OPEN_CREATE, NULL);
				if (rc != SQLITE_OK) {
					fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
					goto exit_save_db;
				}

				/* first time create the table */
				if (need_create_db_table == true) {
#if 0
					rc = snprintf(sql, sizeof(sql), "create table DepthMarketData(TradingDay text, InstrumentID text, ExchangeID text, ExchangeInstID text, LastPrice real, PreSettlementPrice real, PreClosePrice real, PreOpenInterest real, OpenPrice real, HighestPrice real, LowestPrice real, Volume integer, Turnover real, OpenInterest real, ClosePrice real, SettlementPrice real, UpperLimitPrice real, LowerLimitPrice real, PreDelta real, CurrDelta real, UpdateTime text, UpdateMillisec integer, BidPrice1 real, BidVolume1 integer, AskPrice1 real, AskVolume1 integer, BidPrice2 real, BidVolume2 integer, AskPrice2 real, AskVolume2 integer, BidPrice3 real, BidVolume3 integer, AskPrice3 real, AskVolume3 integer, BidPrice4 real, BidVolume4 integer, AskPrice4 real, AskVolume4 integer, BidPrice5 real, BidVolume5 integer, AskPrice5 real, AskVolume5 integer, AveragePrice real, unique(TradingDay, UpdateTime, UpdateMillisec))");
#else
					rc = snprintf(sql, sizeof(sql), "create table DepthMarketData(TradingDay text, InstrumentID text, ExchangeID text, ExchangeInstID text, LastPrice real, PreSettlementPrice real, PreClosePrice real, PreOpenInterest real, OpenPrice real, HighestPrice real, LowestPrice real, Volume integer, Turnover real, OpenInterest real, ClosePrice real, SettlementPrice real, UpperLimitPrice real, LowerLimitPrice real, PreDelta real, CurrDelta real, UpdateTime text, UpdateMillisec integer, BidPrice1 real, BidVolume1 integer, AskPrice1 real, AskVolume1 integer, BidPrice2 real, BidVolume2 integer, AskPrice2 real, AskVolume2 integer, BidPrice3 real, BidVolume3 integer, AskPrice3 real, AskVolume3 integer, BidPrice4 real, BidVolume4 integer, AskPrice4 real, AskVolume4 integer, BidPrice5 real, BidVolume5 integer, AskPrice5 real, AskVolume5 integer, AveragePrice real)");
#endif
					if (rc >= sizeof(sql)) {
						fprintf(stderr, "sql buffer is too small!\n");
						goto exit_save_db;
					}

					rc = sqlite3_exec(db, sql, NULL, NULL, &zErr);
					if(rc != SQLITE_OK) {
						if (zErr != NULL) {
							fprintf(stderr, "SQL error: %s\n", zErr);
							sqlite3_free(zErr);
						}
					}
				}

				/* insert depth market data */
				rc = snprintf(sql, sizeof(sql), "insert into DepthMarketData(TradingDay, InstrumentID, ExchangeID, ExchangeInstID, LastPrice, PreSettlementPrice, PreClosePrice, PreOpenInterest, OpenPrice, HighestPrice, LowestPrice, Volume, Turnover, OpenInterest, ClosePrice, SettlementPrice, UpperLimitPrice, LowerLimitPrice, PreDelta, CurrDelta, UpdateTime, UpdateMillisec, BidPrice1, BidVolume1, AskPrice1, AskVolume1, BidPrice2, BidVolume2, AskPrice2, AskVolume2, BidPrice3, BidVolume3, AskPrice3, AskVolume3, BidPrice4, BidVolume4, AskPrice4, AskVolume4, BidPrice5, BidVolume5, AskPrice5, AskVolume5, AveragePrice) values ('%s', '%s', '%s', '%s', %f, %f, %f, %f, %f, %f, %f, %d, %f, %f, %f, %f, %f, %f, %f, %f, '%s', %d, %f, %d, %f, %d, %f, %d, %f, %d, %f, %d, %f, %d, %f, %d, %f, %d, %f, %d, %f, %d, %f)",
					pDepthMarketData->TradingDay,
					pDepthMarketData->InstrumentID,
					pDepthMarketData->ExchangeID,
					pDepthMarketData->ExchangeInstID,
					pDepthMarketData->LastPrice,
					pDepthMarketData->PreSettlementPrice,
					pDepthMarketData->PreClosePrice,
					pDepthMarketData->PreOpenInterest,
					pDepthMarketData->OpenPrice,
					pDepthMarketData->HighestPrice,
					pDepthMarketData->LowestPrice,
					pDepthMarketData->Volume,
					pDepthMarketData->Turnover,
					pDepthMarketData->OpenInterest,
					pDepthMarketData->ClosePrice,
					pDepthMarketData->SettlementPrice,
					pDepthMarketData->UpperLimitPrice,
					pDepthMarketData->LowerLimitPrice,
					pDepthMarketData->PreDelta,
					pDepthMarketData->CurrDelta,
					pDepthMarketData->UpdateTime,
					pDepthMarketData->UpdateMillisec,
					pDepthMarketData->BidPrice1,
					pDepthMarketData->BidVolume1,
					pDepthMarketData->AskPrice1,
					pDepthMarketData->AskVolume1,
					pDepthMarketData->BidPrice2,
					pDepthMarketData->BidVolume2,
					pDepthMarketData->AskPrice2,
					pDepthMarketData->AskVolume2,
					pDepthMarketData->BidPrice3,
					pDepthMarketData->BidVolume3,
					pDepthMarketData->AskPrice3,
					pDepthMarketData->AskVolume3,
					pDepthMarketData->BidPrice4,
					pDepthMarketData->BidVolume4,
					pDepthMarketData->AskPrice4,
					pDepthMarketData->AskVolume4,
					pDepthMarketData->BidPrice5,
					pDepthMarketData->BidVolume5,
					pDepthMarketData->AskPrice5,
					pDepthMarketData->AskVolume5,
					pDepthMarketData->AveragePrice
				);
				if (rc >= sizeof(sql)) {
					fprintf(stderr, "sql buffer is too small for insert!\n");
					goto exit_save_db;
				}

				rc = sqlite3_exec(db, sql, NULL, NULL, &zErr);
				if(rc != SQLITE_OK) {
					if (zErr != NULL) {
						fprintf(stderr, "SQL error: %s\n", zErr);
						sqlite3_free(zErr);
					}
				}

			exit_save_db:
				sqlite3_close(db);
#endif

#if 0
				printf("-----------------------\n");
				printf("InstrumentID=[%s]\n", pDepthMarketData->InstrumentID);
				printf("ExchangeID=[%s]\n", pDepthMarketData->ExchangeID);
				printf("ExchangeInstID=[%s]\n", pDepthMarketData->ExchangeInstID);
				
				printf("TradingDay=[%s]\n", pDepthMarketData->TradingDay);
				printf("UpdateTime=[%s]\n", pDepthMarketData->UpdateTime);
				printf("UpdateMillisec=[%d]\n", pDepthMarketData->UpdateMillisec);

				printf("LastPrice=[%f]\n", pDepthMarketData->LastPrice);
				printf("Volume=[%d]\n", pDepthMarketData->Volume);
				printf("Turnover=[%f]\n", pDepthMarketData->Turnover);
				printf("OpenInterest=[%f]\n", pDepthMarketData->OpenInterest);

				//printf("OpenPrice=[%f]\n", pDepthMarketData->OpenPrice);
				//printf("ClosePrice=[%f]\n", pDepthMarketData->ClosePrice);
				//printf("SettlementPrice=[%f]\n", pDepthMarketData->SettlementPrice);

				printf("BidPrice1=[%f]\n", pDepthMarketData->BidPrice1);
				printf("BidVolume1=[%d]\n", pDepthMarketData->BidVolume1);
				printf("AskPrice1=[%f]\n", pDepthMarketData->AskPrice1);
				printf("AskVolume1=[%d]\n", pDepthMarketData->AskVolume1);
				printf("====================\n");
#endif
			}
			free(data);
			sleep(DB_SYNC_INTERVAL);
		}
		else {
			sleep(DB_SYNC_INTERVAL);
		}
	}

	/* save thread stopped */
	run_state = 0;
	if (raw_fp != NULL) {
		fclose(raw_fp);
	}

	return NULL;
}

static void *sig_thread_routine(void *arg)
{
	sigset_t *set = (sigset_t *) arg;
	int s, sig;

	for (;;) {
		s = sigwait(set, &sig);
		if (s != 0) {
			DBG("<6>ezctpSaveMarketData: sigwait errno = [%d]\n", s);
			continue;
		}
		DBG("<6>ezctpSaveMarketData: Signal handling thread got signal %d\n", sig);
		switch(sig) {
		case SIGTERM :
			/* tag save thread to be stopped */
			if (run_state == 1) {
				run_state = 2;
			}
			else {
				run_state = 0;
			}

			while (run_state != 0) {
				sleep(1);
			}
			/* now release resources */
			ezctp_util_release_ezcfg_api();
			rc = EXIT_SUCCESS;
			return NULL;
		case SIGUSR1 :
			DBG("<6>ezctpSaveMarketData: [%d]\n", sig);
			break;
		case SIGCHLD :
			/* do nothing for child exit */
			break;
		default :
			DBG("<6>ezctpSaveMarketData: unknown signal [%d]\n", sig);
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
	pthread_t sig_thread, save_thread;
	sigset_t sigset;
	struct ezcfg *ezcfg = NULL;
	char *p;

	/* clean config file path */
	ezctp_ConfPath[0] = '\0';
	ezctp_RawFilePath[0] = '\0';

	for (;;) {
		int c;
		c = getopt( argc, argv, "c:dDhr:t:");
		if (c == EOF) break;
		switch (c) {
			case 'c':
				snprintf(ezctp_ConfPath, sizeof(ezctp_ConfPath), "%s", optarg);
				break;
			case 'r':
				snprintf(ezctp_RawFilePath, sizeof(ezctp_RawFilePath), "%s", optarg);
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
				ezctp_mduser_show_usage();
				return (EXIT_SUCCESS);
			default:
				ezctp_mduser_show_usage();
				return (EXIT_FAILURE);
		}
        }

	if (ezctp_ConfPath[0] == '\0') {
		ezctp_mduser_show_usage();
		return (EXIT_FAILURE);
	}

	if (ezctp_RawFilePath[0] == '\0') {
		ezctp_mduser_show_usage();
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
	setpriority(PRIO_PROCESS, 0, MDUSER_PRIORITY);

	setsid();

	/* main process */
	INFO("<6>ezctpSaveMarketData: booting...\n");
	/* prepare signal handling thread */
	sigemptyset(&sigset);
	sigaddset(&sigset, SIGCHLD);
	sigaddset(&sigset, SIGUSR1);
	sigaddset(&sigset, SIGTERM);
	s = pthread_sigmask(SIG_BLOCK, &sigset, NULL);
	if (s != 0) {
		DBG("<6>ezctpSaveMarketData: pthread_sigmask\n");
		handle_error_en(s, "pthread_sigmask");
	}

	/* get root thread id */
	root_thread = pthread_self();
	//printf("root_thread=[%ld]\n", (long int)&root_thread);

	s = pthread_create(&sig_thread, NULL, &sig_thread_routine, (void *) &sigset);
	if (s != 0) {
		DBG("<6>ezctpSaveMarketData: pthread_create\n");
		handle_error_en(s, "pthread_create");
	}
	//printf("sig_thread=[%ld]\n", (long int)&sig_thread);

	s = pthread_create(&save_thread, NULL, &save_thread_routine, (void *) &sigset);
	if (s != 0) {
		DBG("<6>ezctpSaveMarketData: pthread_create save\n");
		handle_error_en(s, "pthread_create save");
	}
	//printf("save_thread=[%ld]\n", (long int)&save_thread);

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

	s = pthread_join(sig_thread, NULL);
	if (s != 0) {
		ezctp_util_release_ezcfg_api();
		DBG("<6>ezctpSaveMarketData: pthread_join\n");
		handle_error_en(s, "pthread_join");
	}

	/* wait for exit signal */
	return (rc);
}
