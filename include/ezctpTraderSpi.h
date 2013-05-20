#ifndef _EZCTP_TRADER_SPI_H_
#define _EZCTP_TRADER_SPI_H_

#include "ThostFtdcTraderApi.h"

class CTraderSpi : public CThostFtdcTraderSpi
{
public:
	static CTraderSpi *CreateCTraderSpi(struct ezctp_TraderDataField *pTraderData);

	virtual void OnFrontConnected();

	virtual void OnFrontDisconnected(int nReason);
		
	virtual void OnHeartBeatWarning(int nTimeLapse);

	virtual void OnRspAuthenticate(CThostFtdcRspAuthenticateField *pRspAuthenticateField, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,	CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	virtual void OnRspUserLogout(CThostFtdcUserLogoutField *pRspUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	virtual void OnRspUserPasswordUpdate(CThostFtdcUserPasswordUpdateField *pUserPasswordUpdate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	virtual void OnRspTradingAccountPasswordUpdate(CThostFtdcTradingAccountPasswordUpdateField *pTradingAccountPasswordUpdate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	virtual void OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	virtual void OnRspParkedOrderInsert(CThostFtdcParkedOrderField *pParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	virtual void OnRspParkedOrderAction(CThostFtdcParkedOrderActionField *pParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	virtual void OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	virtual void OnRspQueryMaxOrderVolume(CThostFtdcQueryMaxOrderVolumeField *pQueryMaxOrderVolume, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	virtual void OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	virtual void OnRspRemoveParkedOrder(CThostFtdcRemoveParkedOrderField *pRemoveParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	virtual void OnRspRemoveParkedOrderAction(CThostFtdcRemoveParkedOrderActionField *pRemoveParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	virtual void OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	virtual void OnRspQryTrade(CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	virtual void OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	virtual void OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	virtual void OnRspQryInvestor(CThostFtdcInvestorField *pInvestor, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	virtual void OnRspQryTradingCode(CThostFtdcTradingCodeField *pTradingCode, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	virtual void OnRspQryInstrumentMarginRate(CThostFtdcInstrumentMarginRateField *pInstrumentMarginRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	virtual void OnRspQryInstrumentCommissionRate(CThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	virtual void OnRspQryExchange(CThostFtdcExchangeField *pExchange, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	virtual void OnRspQryDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	virtual void OnRspQrySettlementInfo(CThostFtdcSettlementInfoField *pSettlementInfo, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	virtual void OnRspQryTransferBank(CThostFtdcTransferBankField *pTransferBank, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	virtual void OnRspQryInvestorPositionDetail(CThostFtdcInvestorPositionDetailField *pInvestorPositionDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	virtual void OnRspQryNotice(CThostFtdcNoticeField *pNotice, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	virtual void OnRspQrySettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	virtual void OnRspQryInvestorPositionCombineDetail(CThostFtdcInvestorPositionCombineDetailField *pInvestorPositionCombineDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	virtual void OnRspQryCFMMCTradingAccountKey(CThostFtdcCFMMCTradingAccountKeyField *pCFMMCTradingAccountKey, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	virtual void OnRspQryEWarrantOffset(CThostFtdcEWarrantOffsetField *pEWarrantOffset, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	virtual void OnRspQryTransferSerial(CThostFtdcTransferSerialField *pTransferSerial, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	virtual void OnRspQryAccountregister(CThostFtdcAccountregisterField *pAccountregister, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	virtual void OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	virtual void OnRtnOrder(CThostFtdcOrderField *pOrder);

	virtual void OnRtnTrade(CThostFtdcTradeField *pTrade);

	virtual void OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo);

	virtual void OnErrRtnOrderAction(CThostFtdcOrderActionField*, CThostFtdcRspInfoField*);

	virtual void OnRtnInstrumentStatus(CThostFtdcInstrumentStatusField*);

	virtual void OnRtnTradingNotice(CThostFtdcTradingNoticeInfoField*);

	virtual void OnRtnErrorConditionalOrder(CThostFtdcErrorConditionalOrderField*);

	virtual void OnRspQryContractBank(CThostFtdcContractBankField*, CThostFtdcRspInfoField*, int, bool);

	virtual void OnRspQryParkedOrder(CThostFtdcParkedOrderField*, CThostFtdcRspInfoField*, int, bool);

	virtual void OnRspQryParkedOrderAction(CThostFtdcParkedOrderActionField*, CThostFtdcRspInfoField*, int, bool);

	virtual void OnRspQryTradingNotice(CThostFtdcTradingNoticeField*, CThostFtdcRspInfoField*, int, bool);

	virtual void OnRspQryBrokerTradingParams(CThostFtdcBrokerTradingParamsField*, CThostFtdcRspInfoField*, int, bool);

	virtual void OnRspQryBrokerTradingAlgos(CThostFtdcBrokerTradingAlgosField*, CThostFtdcRspInfoField*, int, bool);

	virtual void OnRtnFromBankToFutureByBank(CThostFtdcRspTransferField*);

	virtual void OnRtnFromFutureToBankByBank(CThostFtdcRspTransferField*);

	virtual void OnRtnRepealFromBankToFutureByBank(CThostFtdcRspRepealField*);

	virtual void OnRtnRepealFromFutureToBankByBank(CThostFtdcRspRepealField*);

	virtual void OnRtnFromBankToFutureByFuture(CThostFtdcRspTransferField*);

	virtual void OnRtnFromFutureToBankByFuture(CThostFtdcRspTransferField*);

	virtual void OnRtnRepealFromBankToFutureByFutureManual(CThostFtdcRspRepealField*);

	virtual void OnRtnRepealFromFutureToBankByFutureManual(CThostFtdcRspRepealField*);

	virtual void OnRtnQueryBankBalanceByFuture(CThostFtdcNotifyQueryAccountField*);

	virtual void OnErrRtnBankToFutureByFuture(CThostFtdcReqTransferField*, CThostFtdcRspInfoField*);

	virtual void OnErrRtnFutureToBankByFuture(CThostFtdcReqTransferField*, CThostFtdcRspInfoField*);

	virtual void OnErrRtnRepealBankToFutureByFutureManual(CThostFtdcReqRepealField*, CThostFtdcRspInfoField*);

	virtual void OnErrRtnRepealFutureToBankByFutureManual(CThostFtdcReqRepealField*, CThostFtdcRspInfoField*);

	virtual void OnErrRtnQueryBankBalanceByFuture(CThostFtdcReqQueryAccountField*, CThostFtdcRspInfoField*);

	virtual void OnRtnRepealFromBankToFutureByFuture(CThostFtdcRspRepealField*);

	virtual void OnRtnRepealFromFutureToBankByFuture(CThostFtdcRspRepealField*);

	virtual void OnRspFromBankToFutureByFuture(CThostFtdcReqTransferField*, CThostFtdcRspInfoField*, int, bool);

	virtual void OnRspFromFutureToBankByFuture(CThostFtdcReqTransferField*, CThostFtdcRspInfoField*, int, bool);

	virtual void OnRspQueryBankAccountMoneyByFuture(CThostFtdcReqQueryAccountField*, CThostFtdcRspInfoField*, int, bool);

	virtual void OnRtnOpenAccountByBank(CThostFtdcOpenAccountField*);

	virtual void OnRtnCancelAccountByBank(CThostFtdcCancelAccountField*);

	virtual void OnRtnChangeAccountByBank(CThostFtdcChangeAccountField*);

private:
	void ReqUserLogin();
	void ReqSettlementInfoConfirm();
	void ReqQryInstrument();
	void ReqQryTradingAccount();
	void ReqQryInvestorPosition();
	void ReqOrderInsert();
	void ReqOrderAction(CThostFtdcOrderField *pOrder);

	bool IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo);
	bool IsMyOrder(CThostFtdcOrderField *pOrder);
	bool IsTradingOrder(CThostFtdcOrderField *pOrder);

	struct ezctp_TraderDataField *pTraderData;
};

#endif
