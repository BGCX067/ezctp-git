TOPDIR := $(shell pwd)

CC := gcc
CXX := g++

CFLAGS := -Wall -fPIC -pthread
CFLAGS += -I./include
CFLAGS += -DHAVE_EZBOX_SERVICE_EZCTP=1
LDFLAGS := -L. -L./lib

CTP_MDUSER_LIBS := -lthostmduserapi
CTP_TRADER_LIBS := -lthosttraderapi
EZCTP_LIBS := -lezctp -lezcfg
EZCTP_MDUSER_LIBS := -lezctpmduser
EZCTP_TRADER_LIBS := -lezctptrader
SQLITE3_LIBS := -lsqlite3

CXX_CFLAGS := -Wall -fPIC -pthread
CXX_CFLAGS += -I./include
CXX_CFLAGS += -DHAVE_EZBOX_SERVICE_EZCTP=1

EZCTP_MDUSER_CXX_SRCS := ezctpMdUserApi.cpp
EZCTP_MDUSER_CXX_SRCS += ezctpMdUserSpi.cpp
EZCTP_MDUSER_CXX_OBJS := $(subst .cpp,.oo,$(EZCTP_MDUSER_CXX_SRCS))

EZCTP_TRADER_CXX_SRCS := ezctpTraderApi.cpp
EZCTP_TRADER_CXX_SRCS += ezctpTraderSpi.cpp
EZCTP_TRADER_CXX_OBJS := $(subst .cpp,.oo,$(EZCTP_TRADER_CXX_SRCS))

C_SRCS := ezctp_util_common.c
C_SRCS += ezctp_util_get_conf_string.c
C_SRCS += ezctp_util_file_get_keyword.c
C_SRCS += ezctp_util_file_get_line.c
C_SRCS += ezctp_util_file_print_line.c
C_SRCS += ezctp_util_mkdir.c
C_SRCS += ezctp_util_init_ezcfg_api.c
C_SRCS += ezctp_util_release_ezcfg_api.c
C_SRCS += ezctp_util_insert_market_data.c
C_SRCS += ezctp_util_remove_market_data.c
C_SRCS += ezctp_util_save_market_data.c
C_SRCS += ezctp_util_gen_instrument_id_shfe_cu.c
C_SRCS += ezctp_util_gen_instrument_id_shfe_al.c
C_SRCS += ezctp_util_gen_instrument_id_shfe_zn.c
C_SRCS += ezctp_util_gen_instrument_id_shfe_pb.c
C_SRCS += ezctp_util_gen_instrument_id_shfe_au.c
C_SRCS += ezctp_util_gen_instrument_id_shfe_rb.c
C_SRCS += ezctp_util_gen_instrument_id_shfe_wr.c
C_SRCS += ezctp_util_gen_instrument_id_shfe_fu.c
C_SRCS += ezctp_util_gen_instrument_id_shfe_ru.c
C_SRCS += ezctp_util_gen_instrument_id_czce_ws.c
C_SRCS += ezctp_util_gen_instrument_id_czce_wt.c
C_SRCS += ezctp_util_gen_instrument_id_czce_cf.c
C_SRCS += ezctp_util_gen_instrument_id_czce_sr.c
C_SRCS += ezctp_util_gen_instrument_id_czce_ta.c
C_SRCS += ezctp_util_gen_instrument_id_czce_ro.c
C_SRCS += ezctp_util_gen_instrument_id_czce_er.c
C_SRCS += ezctp_util_gen_instrument_id_czce_me.c
C_SRCS += ezctp_util_gen_instrument_id_dce_c.c
C_SRCS += ezctp_util_gen_instrument_id_dce_a.c
C_SRCS += ezctp_util_gen_instrument_id_dce_b.c
C_SRCS += ezctp_util_gen_instrument_id_dce_m.c
C_SRCS += ezctp_util_gen_instrument_id_dce_y.c
C_SRCS += ezctp_util_gen_instrument_id_dce_p.c
C_SRCS += ezctp_util_gen_instrument_id_dce_l.c
C_SRCS += ezctp_util_gen_instrument_id_dce_v.c
C_SRCS += ezctp_util_gen_instrument_id_dce_j.c

CXX_OBJS := $(subst .cpp,.oo,$(CXX_SRCS))
C_OBJS := $(subst .c,.o,$(C_SRCS))

EXES := cTestMdUser cTestTrader cReceiveMarketData
EXES += ezctpDaemon ezctpCtrl
EXES += ezctpGetMarketData ezctpSaveMarketData ezctpStoreMarketData
EXES += ezctpGetInstrumentID
EXES += cSize cInstrumentID

all: clean libezctp.so libezctpmduser.so libezctptrader.so $(EXES)

%.oo : $(TOPDIR)/src/%.cpp
	$(CXX) $(CXX_CFLAGS) -c $< -o $@

%.o : $(TOPDIR)/src/util/%.c
	$(CC) $(CFLAGS) -c $< -o $@

libezctpmduser.so: $(EZCTP_MDUSER_CXX_OBJS)
	$(CXX) $(CXX_CFLAGS) --shared $(EZCTP_MDUSER_CXX_OBJS) -o $@

libezctptrader.so: $(EZCTP_TRADER_CXX_OBJS)
	$(CXX) $(CXX_CFLAGS) --shared $(EZCTP_TRADER_CXX_OBJS) -o $@

libezctp.so: $(C_OBJS)
	$(CC) $(CFLAGS) --shared $(C_OBJS) -o $@

cTestMdUser: libezctp.so libezctpmduser.so
	$(CC) $(CFLAGS) $(LDFLAGS) $(EZCTP_LIBS) $(EZCTP_MDUSER_LIBS) $(CTP_MDUSER_LIBS) test/cTestMdUser.c -o $@

cTestTrader: libezctp.so libezctptrader.so
	$(CC) $(CFLAGS) $(LDFLAGS) $(EZCTP_LIBS) $(EZCTP_TRADER_LIBS) $(CTP_TRADER_LIBS) test/cTestTrader.c -o $@

cReceiveMarketData: libezctp.so libezctpmduser.so
	$(CC) $(CFLAGS) $(LDFLAGS) $(EZCTP_LIBS) $(EZCTP_MDUSER_LIBS) $(CTP_MDUSER_LIBS) test/cReceiveMarketData.c -o $@

ezctpDaemon: libezctp.so
	$(CC) $(CFLAGS) $(LDFLAGS) $(EZCTP_LIBS) test/ezctpDaemon.c -o $@

ezctpCtrl: libezctp.so
	$(CC) $(CFLAGS) $(LDFLAGS) $(EZCTP_LIBS) test/ezctpCtrl.c -o $@

ezctpGetMarketData: libezctp.so libezctpmduser.so
	$(CC) $(CFLAGS) $(LDFLAGS) $(EZCTP_LIBS) $(EZCTP_MDUSER_LIBS) $(CTP_MDUSER_LIBS) test/ezctpGetMarketData.c -o $@

ezctpSaveMarketData: libezctp.so libezctpmduser.so
	$(CC) $(CFLAGS) $(LDFLAGS) $(EZCTP_LIBS) $(EZCTP_MDUSER_LIBS) $(CTP_MDUSER_LIBS) test/ezctpSaveMarketData.c -o $@

ezctpStoreMarketData: libezctp.so libezctpmduser.so
	$(CC) $(CFLAGS) $(LDFLAGS) $(EZCTP_LIBS) $(EZCTP_MDUSER_LIBS) $(CTP_MDUSER_LIBS) $(SQLITE3_LIBS) test/ezctpStoreMarketData.c -o $@

ezctpGetInstrumentID: libezctp.so libezctptrader.so
	$(CC) $(CFLAGS) $(LDFLAGS) $(EZCTP_LIBS) $(EZCTP_TRADER_LIBS) $(CTP_TRADER_LIBS) test/ezctpGetInstrumentID.c -o $@

cSize: libezctp.so libezctpmduser.so
	$(CC) $(CFLAGS) $(LDFLAGS) $(EZCTP_LIBS) $(EZCTP_MDUSER_LIBS) $(CTP_MDUSER_LIBS) test/cSize.c -o $@

cInstrumentID: libezctp.so libezctpmduser.so
	$(CC) $(CFLAGS) $(LDFLAGS) $(EZCTP_LIBS) $(EZCTP_MDUSER_LIBS) $(CTP_MDUSER_LIBS) test/cInstrumentID.c -o $@

clean:
	rm -rf *.oo *.o *.so $(EXES)
