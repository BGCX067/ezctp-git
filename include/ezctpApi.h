#ifndef _EZCTP_API_H_
#define _EZCTP_API_H_

#include <stddef.h>
#include "ThostFtdcUserApiDataType.h"
#include "ThostFtdcUserApiStruct.h"

/* Based on CTP-API v5.5.1 */
#define EZCTP_API_VERSION_MAJOR	5
#define EZCTP_API_VERSION_MINOR	5
#define EZCTP_API_VERSION_MICRO	1

/* MdUserApi global accessible data structure */
struct ezctp_MdUserDataField {
	void *pMdUserApi;
	void *pMdUserSpi;
	TThostFtdcBrokerIDType BrokerID;
	TThostFtdcUserIDType UserID;
	TThostFtdcInvestorIDType InvestorID;
	TThostFtdcPasswordType Password;
	char** ppInstrumentID;
	int iInstrumentID;
	int iRequestID;
};

/* TraderApi global accessible data structure */
struct ezctp_TraderDataField {
	void *pTraderApi;
	void *pTraderSpi;
	TThostFtdcBrokerIDType BrokerID;
	TThostFtdcUserIDType UserID;
	TThostFtdcInvestorIDType InvestorID;
	TThostFtdcPasswordType Password;
	TThostFtdcFrontIDType FrontID;
	TThostFtdcSessionIDType SessionID;
	TThostFtdcOrderRefType OrderRef;
	TThostFtdcPriceType LimitPrice;
	TThostFtdcDirectionType Direction;
	TThostFtdcInstrumentIDType InstrumentID;
	char** ppInstrumentID;
	int iInstrumentID;
	int iRequestID;
};

/*******************
 * C++ extern
 *******************/
#ifdef __cplusplus
extern "C" {
#endif

/* CMdUserSpi class */
void *ezctp_md_CreateCMdUserSpi(struct ezctp_MdUserDataField *pMdUserData);
void ezctp_md_ReleaseCMdUserSpi(void *thisC);

/* CThostFtdcMdApi class */
void *ezctp_md_CreateFtdcMdApi(const char *pszFlowPath, const bool bIsUsingUdp);
void ezctp_md_Release(void *thisC);
void ezctp_md_Init(void *thisC);
int ezctp_md_Join(void *thisC);
const char *ezctp_md_GetTradingDay(void *thisC);
void ezctp_md_RegisterFront(void *thisC, char *pszFrontAddress);
void ezctp_md_RegisterSpi(void *thisC, void *pSpi);
int ezctp_md_SubscribeMarketData(void *thisC, char *ppInstrumentID[], int nCount);
int ezctp_md_UnSubscribeMarketData(void *thisC, char *ppInstrumentID[], int nCount);
int ezctp_md_ReqUserLogin(void *thisC, struct CThostFtdcReqUserLoginField *pReqUserLoginField, int nRequestID);
int ezctp_md_ReqUserLogout(void *thisC, struct CThostFtdcUserLogoutField *pUserLogout, int nRequestID);

/* CTraderSpi class */
void *ezctp_trader_CreateCTraderSpi(struct ezctp_TraderDataField *pTraderData);
void ezctp_trader_ReleaseCTraderSpi(void *thisC);

/* CThostFtdcTraderApi class */
void *ezctp_trader_CreateFtdcTraderApi(const char *pszFlowPath);
void ezctp_trader_Release(void *thisC);
void ezctp_trader_Init(void *thisC);
int ezctp_trader_Join(void *thisC);
const char *ezctp_trader_GetTradingDay(void *thisC);
void ezctp_trader_RegisterFront(void *thisC, char *pszFrontAddress);
void ezctp_trader_RegisterSpi(void *thisC, void *pSpi);
int ezctp_trader_ReqUserLogin(void *thisC, struct CThostFtdcReqUserLoginField *pReqUserLoginField, int nRequestID);
int ezctp_trader_ReqUserLogout(void *thisC, struct CThostFtdcUserLogoutField *pUserLogout, int nRequestID);
void ezctp_trader_SubscribePrivateTopic(void *thisC, enum THOST_TE_RESUME_TYPE nResumeType);
void ezctp_trader_SubscribePublicTopic(void *thisC, enum THOST_TE_RESUME_TYPE nResumeType);

#ifdef __cplusplus
}
#endif

#endif
