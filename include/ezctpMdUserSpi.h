#ifndef _EZCTP_MDUSER_SPI_H_
#define _EZCTP_MDUSER_SPI_H_

#include "ThostFtdcMdApi.h"

class CMdUserSpi : public CThostFtdcMdSpi
{
public:
	static CMdUserSpi *CreateCMdUserSpi(struct ezctp_MdUserDataField *pMdUserData);
	~CMdUserSpi() {};

	virtual void OnFrontConnected();
	
	virtual void OnFrontDisconnected(int nReason);
		
	virtual void OnHeartBeatWarning(int nTimeLapse);

	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,	CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	virtual void OnRspUserLogout(CThostFtdcUserLogoutField *pRspUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	virtual void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	virtual void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);

private:
	void ReqUserLogin();
	void SubscribeMarketData();
	bool IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo);

	struct ezctp_MdUserDataField *pMdUserData;
};

#endif
