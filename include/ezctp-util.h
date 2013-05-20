#ifndef _EZCTP_UTIL_H_
#define _EZCTP_UTIL_H_

#include <stdio.h>
#include <sys/types.h>

#define EZCTP_PATH_SIZE         256
#define EZCTP_CTRL_MAX_MESSAGE_SIZE	4096
#define LINE_TAIL_STRING        " \t\r\n"

/*******************
 * C++ extern
 *******************/
#ifdef __cplusplus
extern "C" {
#endif

int ezctp_util_get_conf_string(const char *path, const char *keyword, char *buf, size_t len);
char *ezctp_util_file_get_keyword_by_index(char *filename, char *keyword, int idx);
char *ezctp_util_file_get_keyword(char *filename, char *keyword);
bool ezctp_util_file_get_line(FILE *fp, char *buf, size_t size, char *comment, char *rm);
bool ezctp_util_file_print_line(FILE *fp, char *buf, size_t size, const char *fmt, ...);
int ezctp_util_mkdir(const char *path, mode_t mode, bool is_dir);
bool ezctp_util_init_ezcfg_api(const char *path);
bool ezctp_util_release_ezcfg_api(void);
bool ezctp_util_insert_market_data(const void *data, size_t n, size_t size);
bool ezctp_util_remove_market_data(void **data, size_t *n, size_t *size);
bool ezctp_util_save_market_data(FILE *fp, size_t size, int flag);

int ezctp_util_gen_instrument_id_shfe_cu(char *buf, size_t len, int idx);
int ezctp_util_gen_instrument_id_shfe_al(char *buf, size_t len, int idx);
int ezctp_util_gen_instrument_id_shfe_zn(char *buf, size_t len, int idx);
int ezctp_util_gen_instrument_id_shfe_pb(char *buf, size_t len, int idx);
int ezctp_util_gen_instrument_id_shfe_au(char *buf, size_t len, int idx);
int ezctp_util_gen_instrument_id_shfe_rb(char *buf, size_t len, int idx);
int ezctp_util_gen_instrument_id_shfe_wr(char *buf, size_t len, int idx);
int ezctp_util_gen_instrument_id_shfe_fu(char *buf, size_t len, int idx);
int ezctp_util_gen_instrument_id_shfe_ru(char *buf, size_t len, int idx);

int ezctp_util_gen_instrument_id_czce_ws(char *buf, size_t len, int idx);
int ezctp_util_gen_instrument_id_czce_wt(char *buf, size_t len, int idx);
int ezctp_util_gen_instrument_id_czce_cf(char *buf, size_t len, int idx);
int ezctp_util_gen_instrument_id_czce_sr(char *buf, size_t len, int idx);
int ezctp_util_gen_instrument_id_czce_ta(char *buf, size_t len, int idx);
int ezctp_util_gen_instrument_id_czce_ro(char *buf, size_t len, int idx);
int ezctp_util_gen_instrument_id_czce_er(char *buf, size_t len, int idx);
int ezctp_util_gen_instrument_id_czce_me(char *buf, size_t len, int idx);

int ezctp_util_gen_instrument_id_dce_c(char *buf, size_t len, int idx);
int ezctp_util_gen_instrument_id_dce_a(char *buf, size_t len, int idx);
int ezctp_util_gen_instrument_id_dce_b(char *buf, size_t len, int idx);
int ezctp_util_gen_instrument_id_dce_m(char *buf, size_t len, int idx);
int ezctp_util_gen_instrument_id_dce_y(char *buf, size_t len, int idx);
int ezctp_util_gen_instrument_id_dce_p(char *buf, size_t len, int idx);
int ezctp_util_gen_instrument_id_dce_l(char *buf, size_t len, int idx);
int ezctp_util_gen_instrument_id_dce_v(char *buf, size_t len, int idx);
int ezctp_util_gen_instrument_id_dce_j(char *buf, size_t len, int idx);

#ifdef __cplusplus
}
#endif

#endif
