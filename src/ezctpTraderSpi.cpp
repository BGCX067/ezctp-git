/* ============================================================================
 * Project Name : ezctp Application Programming Interface
 * Module Name  : ezctpTraderSpi.cpp
 *
 * Description  : ezctp API for CTraderSpi class
 *
 * Copyright (C) 2012 by ezctp-project
 *
 * History      Rev       Description
 * 2012-02-25   0.1       Write it from scratch
 * ============================================================================
 */

#include "ezctpApi.h"
#include "ThostFtdcTraderApi.h"
#include "ezctpTraderSpi.h"

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

using namespace std;

/*****************
 * CTraderSpi
 *****************/

/*****************
 * Private Methods
 *****************/
void CTraderSpi::ReqUserLogin()
{
	int iResult;
	CThostFtdcTraderApi *pTraderApi;
	CThostFtdcReqUserLoginField req;

	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, pTraderData->BrokerID);
	strcpy(req.UserID, pTraderData->UserID);
	strcpy(req.Password, pTraderData->Password);

	pTraderApi = static_cast<CThostFtdcTraderApi*>(pTraderData->pTraderApi);
	iResult = pTraderApi->ReqUserLogin(&req, ++(pTraderData->iRequestID));

	cerr << "--->>> ReqUserLogin: " << ((iResult == 0) ? "succ" : "fail") << endl;
}

bool CTraderSpi::IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo)
{
	bool bResult = ((pRspInfo) && (pRspInfo->ErrorID != 0));
	if (bResult)
		cerr << "--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << endl;
	return bResult;
}

void CTraderSpi::ReqSettlementInfoConfirm()
{
	int iResult;
	CThostFtdcTraderApi *pTraderApi;
	CThostFtdcSettlementInfoConfirmField req;

	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, pTraderData->BrokerID);
	strcpy(req.InvestorID, pTraderData->InvestorID);

	pTraderApi = static_cast<CThostFtdcTraderApi*>(pTraderData->pTraderApi);
	iResult = pTraderApi->ReqSettlementInfoConfirm(&req, ++(pTraderData->iRequestID));

	cerr << "--->>> ReqSettlementInfoConfirm: " << ((iResult == 0) ? "succ" : "fail") << endl;
}

void CTraderSpi::ReqQryInstrument()
{
	int iResult;
	CThostFtdcTraderApi *pTraderApi;
	CThostFtdcQryInstrumentField req;

	memset(&req, 0, sizeof(req));
	strcpy(req.InstrumentID, pTraderData->InstrumentID);

	pTraderApi = static_cast<CThostFtdcTraderApi*>(pTraderData->pTraderApi);
	iResult = pTraderApi->ReqQryInstrument(&req, ++(pTraderData->iRequestID));

	cerr << "--->>> ReqQryInstrument: " << ((iResult == 0) ? "succ" : "fail") << endl;
}

void CTraderSpi::ReqQryTradingAccount()
{
	int iResult;
	CThostFtdcTraderApi *pTraderApi;
	CThostFtdcQryTradingAccountField req;

	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, pTraderData->BrokerID);
	strcpy(req.InvestorID, pTraderData->InvestorID);

	pTraderApi = static_cast<CThostFtdcTraderApi*>(pTraderData->pTraderApi);
	iResult = pTraderApi->ReqQryTradingAccount(&req, ++(pTraderData->iRequestID));

	cerr << "--->>> ReqQryTradingAccount: " << ((iResult == 0) ? "succ" : "fail") << endl;
}

void CTraderSpi::ReqQryInvestorPosition()
{
	int iResult;
	CThostFtdcTraderApi *pTraderApi;
	CThostFtdcQryInvestorPositionField req;

	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, pTraderData->BrokerID);
	strcpy(req.InvestorID, pTraderData->InvestorID);
	strcpy(req.InstrumentID, pTraderData->InstrumentID);

	pTraderApi = static_cast<CThostFtdcTraderApi*>(pTraderData->pTraderApi);
	iResult = pTraderApi->ReqQryInvestorPosition(&req, ++(pTraderData->iRequestID));

	cerr << "--->>> ReqQryInvestorPosition: " << ((iResult == 0) ? "succ" : "fail") << endl;
}

void CTraderSpi::ReqOrderInsert()
{
	int iResult;
	CThostFtdcTraderApi *pTraderApi;
	CThostFtdcInputOrderField req;

	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, pTraderData->BrokerID);
	strcpy(req.InvestorID, pTraderData->InvestorID);
	strcpy(req.InstrumentID, pTraderData->InstrumentID);
	strcpy(req.OrderRef, pTraderData->OrderRef);
	req.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
	req.Direction = pTraderData->Direction;
	req.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
	req.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
	req.LimitPrice = pTraderData->LimitPrice;
	req.VolumeTotalOriginal = 1;
	req.TimeCondition = THOST_FTDC_TC_GFD;
	req.VolumeCondition = THOST_FTDC_VC_AV;
	req.MinVolume = 1;
	req.ContingentCondition = THOST_FTDC_CC_Immediately;
	req.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
	req.IsAutoSuspend = 0;
	req.UserForceClose = 0;

	pTraderApi = static_cast<CThostFtdcTraderApi*>(pTraderData->pTraderApi);
	iResult = pTraderApi->ReqOrderInsert(&req, ++(pTraderData->iRequestID));

	cerr << "--->>> ReqOrderInsert: " << ((iResult == 0) ? "succ" : "fail") << endl;
}

void CTraderSpi::ReqOrderAction(CThostFtdcOrderField *pOrder)
{
	static bool ORDER_ACTION_SENT = false;		//是否发送了报单
	if (ORDER_ACTION_SENT)
		return;

	int iResult;
	CThostFtdcTraderApi *pTraderApi;
	CThostFtdcInputOrderActionField req;

	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, pOrder->BrokerID);
	strcpy(req.InvestorID, pOrder->InvestorID);
	strcpy(req.OrderRef, pOrder->OrderRef);
	req.FrontID = pTraderData->FrontID;
	req.SessionID = pTraderData->SessionID;
	req.ActionFlag = THOST_FTDC_AF_Delete;
	strcpy(req.InstrumentID, pOrder->InstrumentID);

	pTraderApi = static_cast<CThostFtdcTraderApi*>(pTraderData->pTraderApi);
	iResult = pTraderApi->ReqOrderAction(&req, ++(pTraderData->iRequestID));

	cerr << "--->>> ReqOrderAction: " << ((iResult == 0) ? "succ" : "fail") << endl;
	ORDER_ACTION_SENT = true;
}

bool CTraderSpi::IsMyOrder(CThostFtdcOrderField *pOrder)
{
	return ((pOrder->FrontID == pTraderData->FrontID) &&
			(pOrder->SessionID == pTraderData->SessionID) &&
			(strcmp(pOrder->OrderRef, pTraderData->OrderRef) == 0));
}

bool CTraderSpi::IsTradingOrder(CThostFtdcOrderField *pOrder)
{
	return ((pOrder->OrderStatus != THOST_FTDC_OST_PartTradedNotQueueing) &&
			(pOrder->OrderStatus != THOST_FTDC_OST_Canceled) &&
			(pOrder->OrderStatus != THOST_FTDC_OST_AllTraded));
}


/*****************
 * Public Methods
 *****************/
/*****************
 * virtual functions
 *****************/
void CTraderSpi::OnFrontConnected()
{
	cerr << "--->>> " << __FUNCTION__ << endl;
	ReqUserLogin();
}

void CTraderSpi::OnFrontDisconnected(int nReason)
{
	cerr << "--->>> " << __FUNCTION__ << endl;
	cerr << "--->>> Reason = " << nReason << endl;
}

void CTraderSpi::OnHeartBeatWarning(int nTimeLapse)
{
	cerr << "--->>> " << __FUNCTION__ << endl;
	cerr << "--->>> nTimerLapse = " << nTimeLapse << endl;
}

void CTraderSpi::OnRspAuthenticate(CThostFtdcRspAuthenticateField *pRspAuthenticateField, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> " << __FUNCTION__ << endl;
}

void CTraderSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
		CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> " << "OnRspUserLogin" << endl;
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		int iNextOrderRef;
		CThostFtdcTraderApi *pTraderApi;

		pTraderApi = static_cast<CThostFtdcTraderApi*>(pTraderData->pTraderApi);
		pTraderData->FrontID = pRspUserLogin->FrontID;
		pTraderData->SessionID = pRspUserLogin->SessionID;
		iNextOrderRef = atoi(pRspUserLogin->MaxOrderRef);
		iNextOrderRef++;
		sprintf(pTraderData->OrderRef, "%d", iNextOrderRef);
		cerr << "--->>> GetTradingDay = " << pTraderApi->GetTradingDay() << endl;
		ReqSettlementInfoConfirm();
	}
}

void CTraderSpi::OnRspUserLogout(CThostFtdcUserLogoutField *pRspUserLogout,
		CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> "<< __FUNCTION__ << endl;
}

void CTraderSpi::OnRspUserPasswordUpdate(CThostFtdcUserPasswordUpdateField *pUserPasswordUpdate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> "<< __FUNCTION__ << endl;
}

void CTraderSpi::OnRspTradingAccountPasswordUpdate(CThostFtdcTradingAccountPasswordUpdateField *pTradingAccountPasswordUpdate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> "<< __FUNCTION__ << endl;
}

void CTraderSpi::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> "<< __FUNCTION__ << endl;
	IsErrorRspInfo(pRspInfo);
}

void CTraderSpi::OnRspParkedOrderInsert(CThostFtdcParkedOrderField *pParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> "<< __FUNCTION__ << endl;
}

void CTraderSpi::OnRspParkedOrderAction(CThostFtdcParkedOrderActionField *pParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> "<< __FUNCTION__ << endl;
}

void CTraderSpi::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> "<< __FUNCTION__ << endl;
	IsErrorRspInfo(pRspInfo);
}

void CTraderSpi::OnRspQueryMaxOrderVolume(CThostFtdcQueryMaxOrderVolumeField *pQueryMaxOrderVolume, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> "<< __FUNCTION__ << endl;
}

void CTraderSpi::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> "<< __FUNCTION__ << endl;
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		ReqQryInstrument();
	}
}

void CTraderSpi::OnRspRemoveParkedOrder(CThostFtdcRemoveParkedOrderField *pRemoveParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> "<< __FUNCTION__ << endl;
}

void CTraderSpi::OnRspRemoveParkedOrderAction(CThostFtdcRemoveParkedOrderActionField *pRemoveParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> "<< __FUNCTION__ << endl;
}

void CTraderSpi::OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> "<< __FUNCTION__ << endl;
}

void CTraderSpi::OnRspQryTrade(CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> "<< __FUNCTION__ << endl;
}

void CTraderSpi::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> "<< __FUNCTION__ << endl;
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		ReqOrderInsert();
	}
}

void CTraderSpi::OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> "<< __FUNCTION__ << endl;
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		ReqQryInvestorPosition();
	}
}

void CTraderSpi::OnRspQryInvestor(CThostFtdcInvestorField *pInvestor, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> "<< __FUNCTION__ << endl;
}

void CTraderSpi::OnRspQryTradingCode(CThostFtdcTradingCodeField *pTradingCode, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> "<< __FUNCTION__ << endl;
}

void CTraderSpi::OnRspQryInstrumentMarginRate(CThostFtdcInstrumentMarginRateField *pInstrumentMarginRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> "<< __FUNCTION__ << endl;
}

void CTraderSpi::OnRspQryInstrumentCommissionRate(CThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> "<< __FUNCTION__ << endl;
}

void CTraderSpi::OnRspQryExchange(CThostFtdcExchangeField *pExchange, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> "<< __FUNCTION__ << endl;
}

void CTraderSpi::OnRspQryDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> "<< __FUNCTION__ << endl;
}

void CTraderSpi::OnRspQrySettlementInfo(CThostFtdcSettlementInfoField *pSettlementInfo, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> "<< __FUNCTION__ << endl;
}

void CTraderSpi::OnRspQryTransferBank(CThostFtdcTransferBankField *pTransferBank, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> "<< __FUNCTION__ << endl;
}

void CTraderSpi::OnRspQryInvestorPositionDetail(CThostFtdcInvestorPositionDetailField *pInvestorPositionDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> "<< __FUNCTION__ << endl;
}

void CTraderSpi::OnRspQryNotice(CThostFtdcNoticeField *pNotice, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> "<< __FUNCTION__ << endl;
}

void CTraderSpi::OnRspQrySettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> "<< __FUNCTION__ << endl;
}

void CTraderSpi::OnRspQryInvestorPositionCombineDetail(CThostFtdcInvestorPositionCombineDetailField *pInvestorPositionCombineDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> "<< __FUNCTION__ << endl;
}

void CTraderSpi::OnRspQryCFMMCTradingAccountKey(CThostFtdcCFMMCTradingAccountKeyField *pCFMMCTradingAccountKey, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> "<< __FUNCTION__ << endl;
}

void CTraderSpi::OnRspQryEWarrantOffset(CThostFtdcEWarrantOffsetField *pEWarrantOffset, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> "<< __FUNCTION__ << endl;
}

void CTraderSpi::OnRspQryTransferSerial(CThostFtdcTransferSerialField *pTransferSerial, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> "<< __FUNCTION__ << endl;
}

void CTraderSpi::OnRspQryAccountregister(CThostFtdcAccountregisterField *pAccountregister, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> "<< __FUNCTION__ << endl;
}

void CTraderSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo,
		int nRequestID, bool bIsLast)
{
	cerr << "--->>> "<< __FUNCTION__ << endl;
	IsErrorRspInfo(pRspInfo);
}


void CTraderSpi::OnRtnOrder(CThostFtdcOrderField *pOrder)
{
	cerr << "--->>> " << "OnRtnOrder"  << endl;
	if (IsMyOrder(pOrder))
	{
		if (IsTradingOrder(pOrder))
			ReqOrderAction(pOrder);
		else if (pOrder->OrderStatus == THOST_FTDC_OST_Canceled)
			cout << "--->>> 撤单成功" << endl;
	}
}


void CTraderSpi::OnRtnTrade(CThostFtdcTradeField *pTrade)
{
	cerr << "--->>> "<< __FUNCTION__ << endl;
}

void CTraderSpi::OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo)
{
	cerr << "--->>> "<< __FUNCTION__ << endl;
}

void CTraderSpi::OnErrRtnOrderAction(CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo)
{
	cerr << "--->>> "<< __FUNCTION__ << endl;
}

void CTraderSpi::OnRtnInstrumentStatus(CThostFtdcInstrumentStatusField *pInstrumentStatus)
{
	cerr << "--->>> "<< __FUNCTION__ << endl;
}

void CTraderSpi::OnRtnTradingNotice(CThostFtdcTradingNoticeInfoField *pTradingNoticeInfo)
{
	cerr << "--->>> "<< __FUNCTION__ << endl;
}

void CTraderSpi::OnRtnErrorConditionalOrder(CThostFtdcErrorConditionalOrderField *pErrorConditionalOrder)
{
	cerr << "--->>> "<< __FUNCTION__ << endl;
}

void CTraderSpi::OnRspQryContractBank(CThostFtdcContractBankField *pContractBank, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> "<< __FUNCTION__ << endl;
}

void CTraderSpi::OnRspQryParkedOrder(CThostFtdcParkedOrderField *pParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> "<< __FUNCTION__ << endl;
}

void CTraderSpi::OnRspQryParkedOrderAction(CThostFtdcParkedOrderActionField *pParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> "<< __FUNCTION__ << endl;
}

void CTraderSpi::OnRspQryTradingNotice(CThostFtdcTradingNoticeField *pTradingNotice, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> "<< __FUNCTION__ << endl;
}

void CTraderSpi::OnRspQryBrokerTradingParams(CThostFtdcBrokerTradingParamsField *pBrokerTradingParams, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> "<< __FUNCTION__ << endl;
}

void CTraderSpi::OnRspQryBrokerTradingAlgos(CThostFtdcBrokerTradingAlgosField *pBrokerTradingAlgos, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> "<< __FUNCTION__ << endl;
}

void CTraderSpi::OnRtnFromBankToFutureByBank(CThostFtdcRspTransferField *pRspTransfer)
{
	cerr << "--->>> "<< __FUNCTION__ << endl;
}

void CTraderSpi::OnRtnFromFutureToBankByBank(CThostFtdcRspTransferField *pRspTransfer)
{
	cerr << "--->>> "<< __FUNCTION__ << endl;
}

void CTraderSpi::OnRtnRepealFromBankToFutureByBank(CThostFtdcRspRepealField *pRspRepeal)
{
	cerr << "--->>> "<< __FUNCTION__ << endl;
}

void CTraderSpi::OnRtnRepealFromFutureToBankByBank(CThostFtdcRspRepealField *pRspRepeal)
{
	cerr << "--->>> "<< __FUNCTION__ << endl;
}

void CTraderSpi::OnRtnFromBankToFutureByFuture(CThostFtdcRspTransferField *pRspTransfer)
{
	cerr << "--->>> "<< __FUNCTION__ << endl;
}

void CTraderSpi::OnRtnFromFutureToBankByFuture(CThostFtdcRspTransferField *pRspTransfer)
{
	cerr << "--->>> "<< __FUNCTION__ << endl;
}

void CTraderSpi::OnRtnRepealFromBankToFutureByFutureManual(CThostFtdcRspRepealField *pRspRepeal)
{
	cerr << "--->>> "<< __FUNCTION__ << endl;
}

void CTraderSpi::OnRtnRepealFromFutureToBankByFutureManual(CThostFtdcRspRepealField *pRspRepeal)
{
	cerr << "--->>> "<< __FUNCTION__ << endl;
}

void CTraderSpi::OnRtnQueryBankBalanceByFuture(CThostFtdcNotifyQueryAccountField *pNotifyQueryAccount)
{
	cerr << "--->>> "<< __FUNCTION__ << endl;
}

void CTraderSpi::OnErrRtnBankToFutureByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo)
{
	cerr << "--->>> "<< __FUNCTION__ << endl;
}

void CTraderSpi::OnErrRtnFutureToBankByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo)
{
	cerr << "--->>> "<< __FUNCTION__ << endl;
}

void CTraderSpi::OnErrRtnRepealBankToFutureByFutureManual(CThostFtdcReqRepealField *pReqRepeal, CThostFtdcRspInfoField *pRspInfo)
{
	cerr << "--->>> "<< __FUNCTION__ << endl;
}

void CTraderSpi::OnErrRtnRepealFutureToBankByFutureManual(CThostFtdcReqRepealField *pReqRepeal, CThostFtdcRspInfoField *pRspInfo)
{
	cerr << "--->>> "<< __FUNCTION__ << endl;
}

void CTraderSpi::OnErrRtnQueryBankBalanceByFuture(CThostFtdcReqQueryAccountField *pReqQueryAccount, CThostFtdcRspInfoField *pRspInfo)
{
	cerr << "--->>> "<< __FUNCTION__ << endl;
}

void CTraderSpi::OnRtnRepealFromBankToFutureByFuture(CThostFtdcRspRepealField *pRspRepeal)
{
	cerr << "--->>> "<< __FUNCTION__ << endl;
}

void CTraderSpi::OnRtnRepealFromFutureToBankByFuture(CThostFtdcRspRepealField *pRspRepeal)
{
	cerr << "--->>> "<< __FUNCTION__ << endl;
}

void CTraderSpi::OnRspFromBankToFutureByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> "<< __FUNCTION__ << endl;
}

void CTraderSpi::OnRspFromFutureToBankByFuture(CThostFtdcReqTransferField *pReqTransfer, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> "<< __FUNCTION__ << endl;
}

void CTraderSpi::OnRspQueryBankAccountMoneyByFuture(CThostFtdcReqQueryAccountField *pReqQueryAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> "<< __FUNCTION__ << endl;
}

void CTraderSpi::OnRtnOpenAccountByBank(CThostFtdcOpenAccountField *pOpenAccount)
{
	cerr << "--->>> "<< __FUNCTION__ << endl;
}

void CTraderSpi::OnRtnCancelAccountByBank(CThostFtdcCancelAccountField *pCancelAccount)
{
	cerr << "--->>> "<< __FUNCTION__ << endl;
}

void CTraderSpi::OnRtnChangeAccountByBank(CThostFtdcChangeAccountField *pChangeAccount)
{
	cerr << "--->>> "<< __FUNCTION__ << endl;
}

/* FIXME: */
void CTraderSpi::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> " << "OnRspQryInstrument" << endl;
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		///请求查询合约
		ReqQryTradingAccount();
	}
}

/*****************
 * ezctp added functions
 *****************/
CTraderSpi *CTraderSpi::CreateCTraderSpi(struct ezctp_TraderDataField *pTraderData)
{
	CTraderSpi *pMyTraderSpi;

	if (pTraderData == NULL)
		return NULL;

	pMyTraderSpi = new CTraderSpi();
	if (pMyTraderSpi == NULL)
		return NULL;

	pMyTraderSpi->pTraderData = pTraderData;

	return pMyTraderSpi;
}

