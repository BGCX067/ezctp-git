/* ============================================================================
 * Project Name : ezctp Application Programming Interface
 * Module Name  : ezctpMdUserSpi.cpp
 *
 * Description  : ezctp API for CMdUserApi class
 *
 * Copyright (C) 2012 by ezctp-project
 *
 * History      Rev       Description
 * 2012-02-24   0.1       Split it from ezctpThostFtdcMdApi.cpp
 * ============================================================================
 */

#include "ezctpApi.h"
#include "ThostFtdcMdApi.h"
#include "ezctpMdUserSpi.h"
#include "ezctp-util.h"

#include <iostream>
#include <stdio.h>
#include <string.h>

using namespace std;

/*****************
 * CMdUserSpi
 *****************/

/*****************
 * Private Methods
 *****************/
void CMdUserSpi::ReqUserLogin()
{
	int iResult;
	CThostFtdcMdApi *pMdUserApi;
	CThostFtdcReqUserLoginField req;

	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, pMdUserData->BrokerID);
	strcpy(req.UserID, pMdUserData->UserID);
	strcpy(req.Password, pMdUserData->Password);

	pMdUserApi = static_cast<CThostFtdcMdApi*>(pMdUserData->pMdUserApi);
	iResult = pMdUserApi->ReqUserLogin(&req, ++(pMdUserData->iRequestID));

	cerr << "--->>> ReqUserLogin: " << ((iResult == 0) ? "succ" : "fail") << endl;
}

void CMdUserSpi::SubscribeMarketData()
{
	int iResult;
	CThostFtdcMdApi *pMdUserApi;

	pMdUserApi = static_cast<CThostFtdcMdApi*>(pMdUserData->pMdUserApi);
	iResult = pMdUserApi->SubscribeMarketData(pMdUserData->ppInstrumentID, pMdUserData->iInstrumentID);
	cerr << "--->>> SubscribeMarketData: " << ((iResult == 0) ? "succ" : "fail") << endl;
}

bool CMdUserSpi::IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo)
{
	bool bResult = ((pRspInfo) && (pRspInfo->ErrorID != 0));
	if (bResult)
		cerr << "--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << endl;
	return bResult;
}


/*****************
 * Public Methods
 *****************/
/*****************
 * virtual functions
 *****************/
void CMdUserSpi::OnFrontConnected()
{
	cerr << "--->>> " << __FUNCTION__ << endl;
	ReqUserLogin();
}

void CMdUserSpi::OnFrontDisconnected(int nReason)
{
	cerr << "--->>> " << __FUNCTION__ << endl;
	cerr << "--->>> Reason = " << nReason << endl;
}
		
void CMdUserSpi::OnHeartBeatWarning(int nTimeLapse)
{
	cerr << "--->>> " << __FUNCTION__ << endl;
	cerr << "--->>> nTimerLapse = " << nTimeLapse << endl;
}

void CMdUserSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
		CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	CThostFtdcMdApi *pMdUserApi;

	pMdUserApi = static_cast<CThostFtdcMdApi*>(pMdUserData->pMdUserApi);
	cerr << "--->>> " << __FUNCTION__ << endl;
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		cerr << "--->>> GetTradingDay = " << pMdUserApi->GetTradingDay() << endl;
		SubscribeMarketData();	
	}
}

void CMdUserSpi::OnRspUserLogout(CThostFtdcUserLogoutField *pRspUserLogout,
		CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	CThostFtdcMdApi *pMdUserApi;

	pMdUserApi = static_cast<CThostFtdcMdApi*>(pMdUserData->pMdUserApi);
	cerr << "--->>> " << __FUNCTION__ << endl;
}

void CMdUserSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo,
		int nRequestID, bool bIsLast)
{
	cerr << "--->>> "<< __FUNCTION__ << endl;
	IsErrorRspInfo(pRspInfo);
}

void CMdUserSpi::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument,
		CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> "<< __FUNCTION__ << endl;
}

void CMdUserSpi::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument,
		CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> "<< __FUNCTION__ << endl;
}

void CMdUserSpi::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
	if (pDepthMarketData == NULL) {
		printf("pDepthMarketData = NULL");
		return;
	}

	if (ezctp_util_insert_market_data(pDepthMarketData, 1,
		sizeof(CThostFtdcDepthMarketDataField)) == false) {
		printf("insert data error\n");
	}
	else {
#if 0
        ///½»Ò×ÈÕ
        //TThostFtdcDateType      TradingDay;
		printf("TradingDay=[%s]\n", pDepthMarketData->TradingDay);
        ///ºÏÔ¼´úÂë
        //TThostFtdcInstrumentIDType      InstrumentID;
		printf("InstrumentID=[%s]\n", pDepthMarketData->InstrumentID);
        ///½»Ò×Ëù´úÂë
        //TThostFtdcExchangeIDType        ExchangeID;
		printf("ExchangeID=[%s]\n", pDepthMarketData->ExchangeID);
        ///ºÏÔ¼ÔÚ½»Ò×ËùµÄ´úÂë
        //TThostFtdcExchangeInstIDType    ExchangeInstID;
		printf("ExchangeInstID=[%s]\n", pDepthMarketData->ExchangeInstID);
        ///×îÐÂ¼Û
        //TThostFtdcPriceType     LastPrice;
		printf("LastPrice=[%f]\n", pDepthMarketData->LastPrice);
        ///ÉÏ´Î½áËã¼Û
        //TThostFtdcPriceType     PreSettlementPrice;
		printf("PreSettlementPrice=[%f]\n", pDepthMarketData->PreSettlementPrice);
        ///×òÊÕÅÌ
        //TThostFtdcPriceType     PreClosePrice;
		printf("PreClosePrice=[%f]\n", pDepthMarketData->PreClosePrice);
        ///×ò³Ö²ÖÁ¿
        //TThostFtdcLargeVolumeType       PreOpenInterest;
		printf("PreOpenInterest=[%f]\n", pDepthMarketData->PreOpenInterest);
        ///½ñ¿ªÅÌ
        //TThostFtdcPriceType     OpenPrice;
		printf("OpenPrice=[%f]\n", pDepthMarketData->OpenPrice);
        ///×î¸ß¼Û
        //TThostFtdcPriceType     HighestPrice;
		printf("HighestPrice=[%f]\n", pDepthMarketData->HighestPrice);
        ///×îµÍ¼Û
        //TThostFtdcPriceType     LowestPrice;
		printf("LowestPrice=[%f]\n", pDepthMarketData->LowestPrice);
        ///ÊýÁ¿
        //TThostFtdcVolumeType    Volume;
		printf("Volume=[%d]\n", pDepthMarketData->Volume);
        ///³É½»½ð¶î
        //TThostFtdcMoneyType     Turnover;
		printf("Turnover=[%f]\n", pDepthMarketData->Turnover);
        ///³Ö²ÖÁ¿
        //TThostFtdcLargeVolumeType       OpenInterest;
		printf("OpenInterest=[%f]\n", pDepthMarketData->OpenInterest);
        ///½ñÊÕÅÌ
        //TThostFtdcPriceType     ClosePrice;
		printf("ClosePrice=[%f]\n", pDepthMarketData->ClosePrice);
        ///±¾´Î½áËã¼Û
        //TThostFtdcPriceType     SettlementPrice;
		printf("SettlementPrice=[%f]\n", pDepthMarketData->SettlementPrice);
        ///ÕÇÍ£°å¼Û
        //TThostFtdcPriceType     UpperLimitPrice;
		printf("UpperLimitPrice=[%f]\n", pDepthMarketData->UpperLimitPrice);
        ///µøÍ£°å¼Û
        //TThostFtdcPriceType     LowerLimitPrice;
		printf("LowerLimitPrice=[%f]\n", pDepthMarketData->LowerLimitPrice);
        ///×òÐéÊµ¶È
        //TThostFtdcRatioType     PreDelta;
		printf("PreDelta=[%f]\n", pDepthMarketData->PreDelta);
        ///½ñÐéÊµ¶È
        //TThostFtdcRatioType     CurrDelta;
		printf("CurrDelta=[%f]\n", pDepthMarketData->CurrDelta);
        ///×îºóÐÞ¸ÄÊ±¼ä
        //TThostFtdcTimeType      UpdateTime;
		printf("UpdateTime=[%s]\n", pDepthMarketData->UpdateTime);
        ///×îºóÐÞ¸ÄºÁÃë
        //TThostFtdcMillisecType  UpdateMillisec;
		printf("UpdateMillisec=[%d]\n", pDepthMarketData->UpdateMillisec);
        ///ÉêÂò¼ÛÒ»
        //TThostFtdcPriceType     BidPrice1;
		printf("BidPrice1=[%f]\n", pDepthMarketData->BidPrice1);
        ///ÉêÂòÁ¿Ò»
        //TThostFtdcVolumeType    BidVolume1;
		printf("BidVolume1=[%d]\n", pDepthMarketData->BidVolume1);
        ///ÉêÂô¼ÛÒ»
        //TThostFtdcPriceType     AskPrice1;
		printf("AskPrice1=[%f]\n", pDepthMarketData->AskPrice1);
        ///ÉêÂôÁ¿Ò»
        //TThostFtdcVolumeType    AskVolume1;
		printf("AskVolume1=[%d]\n", pDepthMarketData->AskVolume1);
        ///ÉêÂò¼Û¶þ
        //TThostFtdcPriceType     BidPrice2;
		printf("BidPrice2=[%f]\n", pDepthMarketData->BidPrice2);
        ///ÉêÂòÁ¿¶þ
        //TThostFtdcVolumeType    BidVolume2;
		printf("BidVolume2=[%d]\n", pDepthMarketData->BidVolume2);
        ///ÉêÂô¼Û¶þ
        //TThostFtdcPriceType     AskPrice2;
		printf("AskPrice2=[%f]\n", pDepthMarketData->AskPrice2);
        ///ÉêÂôÁ¿¶þ
        //TThostFtdcVolumeType    AskVolume2;
		printf("AskVolume2=[%d]\n", pDepthMarketData->AskVolume2);
        ///ÉêÂò¼ÛÈý
        //TThostFtdcPriceType     BidPrice3;
		printf("BidPrice3=[%f]\n", pDepthMarketData->BidPrice3);
        ///ÉêÂòÁ¿Èý
        //TThostFtdcVolumeType    BidVolume3;
		printf("BidVolume3=[%d]\n", pDepthMarketData->BidVolume3);
        ///ÉêÂô¼ÛÈý
        //TThostFtdcPriceType     AskPrice3;
		printf("AskPrice3=[%f]\n", pDepthMarketData->AskPrice3);
        ///ÉêÂôÁ¿Èý
        //TThostFtdcVolumeType    AskVolume3;
		printf("AskVolume3=[%d]\n", pDepthMarketData->AskVolume3);
        ///ÉêÂò¼ÛËÄ
        //TThostFtdcPriceType     BidPrice4;
		printf("BidPrice4=[%f]\n", pDepthMarketData->BidPrice4);
        ///ÉêÂòÁ¿ËÄ
        //TThostFtdcVolumeType    BidVolume4;
		printf("BidVolume4=[%d]\n", pDepthMarketData->BidVolume4);
        ///ÉêÂô¼ÛËÄ
        //TThostFtdcPriceType     AskPrice4;
		printf("AskPrice4=[%f]\n", pDepthMarketData->AskPrice4);
        ///ÉêÂôÁ¿ËÄ
        //TThostFtdcVolumeType    AskVolume4;
		printf("AskVolume4=[%d]\n", pDepthMarketData->AskVolume4);
        ///ÉêÂò¼ÛÎå
        //TThostFtdcPriceType     BidPrice5;
		printf("BidPrice5=[%f]\n", pDepthMarketData->BidPrice5);
        ///ÉêÂòÁ¿Îå
        //TThostFtdcVolumeType    BidVolume5;
		printf("BidVolume5=[%d]\n", pDepthMarketData->BidVolume5);
        ///ÉêÂô¼ÛÎå
        //TThostFtdcPriceType     AskPrice5;
		printf("AskPrice5=[%f]\n", pDepthMarketData->AskPrice5);
        ///ÉêÂôÁ¿Îå
        //TThostFtdcVolumeType    AskVolume5;
		printf("AskVolume5=[%d]\n", pDepthMarketData->AskVolume5);
        ///µ±ÈÕ¾ù¼Û
        //TThostFtdcPriceType     AveragePrice;
		printf("AveragePrice=[%f]\n", pDepthMarketData->AveragePrice);
#endif
	}
}

/*****************
 * ezctp added functions
 *****************/
CMdUserSpi *CMdUserSpi::CreateCMdUserSpi(struct ezctp_MdUserDataField *pMdUserData)
{
	CMdUserSpi *pMyMdUserSpi;

	if (pMdUserData == NULL)
		return NULL;

	pMyMdUserSpi = new CMdUserSpi();
	if (pMyMdUserSpi == NULL)
		return NULL;

	pMyMdUserSpi->pMdUserData = pMdUserData;

	return pMyMdUserSpi;
}
