#include <stddef.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ezctpApi.h"

void *pTraderApi = NULL;
void *pTraderSpi = NULL;


/* front address */
char FRONT_ADDR[] = "tcp://asp-sim2-front1.financial-trading-platform.com:26205";
TThostFtdcBrokerIDType  BROKER_ID = "2030";
TThostFtdcUserIDType USER_ID = "00069";
TThostFtdcInvestorIDType INVESTOR_ID = "00069";
TThostFtdcPasswordType  PASSWORD = "888888";
TThostFtdcInstrumentIDType INSTRUMENT_ID = "cu1206";
TThostFtdcDirectionType DIRECTION = THOST_FTDC_D_Sell;
TThostFtdcPriceType LIMIT_PRICE = 38850;
int iRequestID = 0;

int main(void)
{
	struct ezctp_TraderDataField myTraderData;

	pTraderApi = ezctp_trader_CreateFtdcTraderApi("", false);
	if (pTraderApi == NULL) {
		printf("pTraderApi = NULL\n");
		exit(EXIT_FAILURE);
	}

	memset(&myTraderData, 0, sizeof(myTraderData));
	myTraderData.pTraderApi = pTraderApi;
	strcpy(myTraderData.BrokerID, BROKER_ID);
	strcpy(myTraderData.UserID, USER_ID);
	strcpy(myTraderData.InvestorID, INVESTOR_ID);
	strcpy(myTraderData.Password, PASSWORD);
	myTraderData.iRequestID = iRequestID;

	pTraderSpi = ezctp_trader_CreateCTraderSpi(&myTraderData);
	if (pTraderSpi == NULL) {
		printf("pTraderSpi = NULL\n");
		ezctp_trader_Release(pTraderApi);
		exit(EXIT_FAILURE);
	}
	myTraderData.pTraderSpi = pTraderSpi;

	ezctp_trader_RegisterSpi(pTraderApi, pTraderSpi);
        ezctp_trader_SubscribePublicTopic(pTraderApi, THOST_TERT_RESTART);
        //ezctp_trader_SubscribePrivateTopic(pTraderApi, THOST_TERT_RESTART);
	ezctp_trader_RegisterFront(pTraderApi, FRONT_ADDR);
	ezctp_trader_Init(pTraderApi);
	ezctp_trader_Join(pTraderApi);
	ezctp_trader_Release(pTraderApi);

	return(EXIT_SUCCESS);
}
