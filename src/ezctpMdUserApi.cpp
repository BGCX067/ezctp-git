/* ============================================================================
 * Project Name : ezctp Application Programming Interface
 * Module Name  : ezctpMdUserApi.cpp
 *
 * Description  : ezctp C API for CThostFtdcMdApi class
 *
 * Copyright (C) 2012 by ezctp-project
 *
 * History      Rev       Description
 * 2012-03-02   0.1       Split it from ezctpApi.cpp
 * ============================================================================
 */

#include "ezctpMdUserSpi.h"
#include "ezctpApi.h"

#include <iostream>
#include <stdio.h>
#include <string.h>

using namespace std;

/********************
 * FtdcMdApi
 ********************/
void *ezctp_md_CreateFtdcMdApi(const char *pszFlowPath, const bool bIsUsingUdp, const bool bIsMulticast)
{
	return CThostFtdcMdApi::CreateFtdcMdApi(pszFlowPath, bIsUsingUdp, bIsMulticast);
}

void ezctp_md_Release(void *thisC)
{
	static_cast<CThostFtdcMdApi*>(thisC)->Release();
}

void ezctp_md_Init(void *thisC)
{
	static_cast<CThostFtdcMdApi*>(thisC)->Init();
}

int ezctp_md_Join(void *thisC)
{
	return static_cast<CThostFtdcMdApi*>(thisC)->Join();
}

const char *ezctp_md_GetTradingDay(void *thisC)
{
	return static_cast<CThostFtdcMdApi*>(thisC)->GetTradingDay();
}

void ezctp_md_RegisterFront(void *thisC, char *pszFrontAddress)
{
	static_cast<CThostFtdcMdApi*>(thisC)->RegisterFront(pszFrontAddress);
}

void ezctp_md_RegisterSpi(void *thisC, void *pSpi)
{
	static_cast<CThostFtdcMdApi*>(thisC)->RegisterSpi(static_cast<CThostFtdcMdSpi*>(pSpi));
}

int ezctp_md_SubscribeMarketData(void *thisC, char *ppInstrumentID[], int nCount)
{
	return static_cast<CThostFtdcMdApi*>(thisC)->SubscribeMarketData(ppInstrumentID, nCount);
}

int ezctp_md_UnSubscribeMarketData(void *thisC, char *ppInstrumentID[], int nCount)
{
	return static_cast<CThostFtdcMdApi*>(thisC)->UnSubscribeMarketData(ppInstrumentID, nCount);
}

int ezctp_md_ReqUserLogin(void *thisC, CThostFtdcReqUserLoginField *pReqUserLoginField, int nRequestID)
{
	return static_cast<CThostFtdcMdApi*>(thisC)->ReqUserLogin(pReqUserLoginField, nRequestID);
}

int ezctp_md_ReqUserLogout(void *thisC, CThostFtdcUserLogoutField *pUserLogout, int nRequestID)
{
	return static_cast<CThostFtdcMdApi*>(thisC)->ReqUserLogout(pUserLogout, nRequestID);
}

/********************
 * ezctpMdUserSpi
 ********************/
void *ezctp_md_CreateCMdUserSpi(struct ezctp_MdUserDataField *pMdUserData)
{
	if (pMdUserData == NULL)
		return NULL;

	return CMdUserSpi::CreateCMdUserSpi(pMdUserData);
}

void ezctp_md_ReleaseCMdUserSpi(void *thisC)
{
	if (thisC == NULL)
		return;

	delete static_cast<CMdUserSpi*>(thisC);
}
