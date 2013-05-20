/* ============================================================================
 * Project Name : ezctp Application Programming Interface
 * Module Name  : ezctpTraderApi.cpp
 *
 * Description  : ezctp C API for CThostFtdcTraderApi class
 *
 * Copyright (C) 2012 by ezctp-project
 *
 * History      Rev       Description
 * 2012-03-02   0.1       Split it from ezctpApi.cpp
 * ============================================================================
 */

#include "ezctpTraderSpi.h"
#include "ezctpApi.h"

#include <iostream>
#include <stdio.h>
#include <string.h>

using namespace std;

/********************
 * FtdcTraderApi
 ********************/
void *ezctp_trader_CreateFtdcTraderApi(const char *pszFlowPath)
{
	return CThostFtdcTraderApi::CreateFtdcTraderApi(pszFlowPath);
}

void ezctp_trader_Release(void *thisC)
{
	static_cast<CThostFtdcTraderApi*>(thisC)->Release();
}

void ezctp_trader_Init(void *thisC)
{
	static_cast<CThostFtdcTraderApi*>(thisC)->Init();
}

int ezctp_trader_Join(void *thisC)
{
	return static_cast<CThostFtdcTraderApi*>(thisC)->Join();
}

const char *ezctp_trader_GetTradingDay(void *thisC)
{
	return static_cast<CThostFtdcTraderApi*>(thisC)->GetTradingDay();
}

void ezctp_trader_RegisterFront(void *thisC, char *pszFrontAddress)
{
	static_cast<CThostFtdcTraderApi*>(thisC)->RegisterFront(pszFrontAddress);
}

void ezctp_trader_RegisterSpi(void *thisC, void *pSpi)
{
	static_cast<CThostFtdcTraderApi*>(thisC)->RegisterSpi(static_cast<CThostFtdcTraderSpi*>(pSpi));
}

int ezctp_trader_ReqUserLogin(void *thisC, CThostFtdcReqUserLoginField *pReqUserLoginField, int nRequestID)
{
	return static_cast<CThostFtdcTraderApi*>(thisC)->ReqUserLogin(pReqUserLoginField, nRequestID);
}

int ezctp_trader_ReqUserLogout(void *thisC, CThostFtdcUserLogoutField *pUserLogout, int nRequestID)
{
	return static_cast<CThostFtdcTraderApi*>(thisC)->ReqUserLogout(pUserLogout, nRequestID);
}

void ezctp_trader_SubscribePrivateTopic(void *thisC, THOST_TE_RESUME_TYPE nResumeType)
{
	static_cast<CThostFtdcTraderApi*>(thisC)->SubscribePrivateTopic(nResumeType);
}

void ezctp_trader_SubscribePublicTopic(void *thisC, THOST_TE_RESUME_TYPE nResumeType)
{
	static_cast<CThostFtdcTraderApi*>(thisC)->SubscribePublicTopic(nResumeType);
}

/********************
 * ezctpTraderSpi
 ********************/
void *ezctp_trader_CreateCTraderSpi(struct ezctp_TraderDataField *pTraderData)
{
	if (pTraderData == NULL)
		return NULL;

	return CTraderSpi::CreateCTraderSpi(pTraderData);
}

void ezctp_trader_ReleaseCTraderSpi(void *thisC)
{
	if (thisC == NULL)
		return;

	delete static_cast<CTraderSpi*>(thisC);
}
