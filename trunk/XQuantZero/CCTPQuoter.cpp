﻿#include "CCTPQuoter.h"

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new (_NORMAL_BLOCK , __FILE__ , __LINE__)
#define new DBG_NEW
#endif
#endif

///Inline utility function to convert result code into QString ## inline函数将CTP返回值转换为QString
inline QString resultString(int code) {
    // TODO: Change magic string into variable/external string

    QString sResult;
    switch (code) {
        case CCTPQuoter::SUCCESS_POST:
            sResult = "登陆成功。\n";
            break;
        case CCTPQuoter::FAILED_NETWORK:
            sResult = "登陆网络连接失败。\n";
            break;
        case CCTPQuoter::FAILED_REACH_LIMITS:
            sResult = "登陆未处理请求超过许可数。\n";
            break;
        case CCTPQuoter::FAILED_REACH_GRANTS:
            sResult = "登陆每秒发送请求超过许可数。\n";
            break;
        default:
            sResult.append("未知登陆失败 ");
            sResult.append("CodeID:");
            sResult.append(code);
            sResult.append(" \n");
            break;
    }
    return sResult;
}

int CCTPQuoter::login(void) {
	CThostFtdcReqUserLoginField loginField;
	memset(&loginField, 0, sizeof(loginField));
	strcpy(loginField.BrokerID, brokerId.c_str());
	strcpy(loginField.UserID, userId.c_str());
	strcpy(loginField.Password, passwd.c_str());
	strcpy(loginField.UserProductInfo, "ashu");

	string sMsg("正在登陆行情服务器。。。\n");
	genMsg(MSG_TYPE_LOGIN, sMsg);
	string sMsg2("USER ID: ");
	sMsg2.append(loginField.UserID);
	sMsg2.append("\n");
	sMsg2.append("Trade IP : ");
	sMsg2.append(this->mdIp);
	sMsg2.append("\n");
	genMsg(MSG_TYPE_LOGIN, sMsg2);


	string sResult = { 0 };
	int iResult = queryApi->ReqUserLogin(&loginField, ++reqId);
	switch (iResult) {
	case SUCCESS_POST:
		sResult = "登陆行情服务器成功。\n";
		break;
	case FAILED_NETWORK:
		sResult = "登陆行情服务器网络连接失败。\n";
		break;
	case FAILED_REACH_LIMITS:
		sResult = "登陆行情服务器未处理请求超过许可数。\n";
		break;
	case FAILED_REACH_GRANTS:
		sResult = "登陆行情服务器每秒发送请求超过许可数。\n";
		break;
	default:
		sResult = "未知行情服务器登陆失败。\n";
		break;
	}
	genMsg(MSG_TYPE_LOGIN, sResult);

	return iResult;
}

int CCTPQuoter::logout() {
	CThostFtdcUserLogoutField userLogout;
	memset(&userLogout, 0, sizeof(userLogout));
	strcpy(userLogout.BrokerID, brokerId.c_str());
	strcpy(userLogout.UserID, userId.c_str());

	string sMsg("正在注销。。。\n");
	genMsg(MSG_TYPE_LOGIN, sMsg);

	string sResult = { 0 };
	int iResult = queryApi->ReqUserLogout(&userLogout, ++reqId);
	switch (iResult) {
	case SUCCESS_POST:
		sResult = "注销成功。\n";
		break;
	case FAILED_NETWORK:
		sResult = "注销网络连接失败。\n";
		break;
	case FAILED_REACH_LIMITS:
		sResult = "注销未处理请求超过许可数。\n";
		break;
	case FAILED_REACH_GRANTS:
		sResult = "注销每秒发送请求超过许可数。\n";
		break;
	default:
		sResult = "未知注销失败。\n";
		break;
	}
	genMsg(MSG_TYPE_LOGIN, sResult);

	return iResult;
}

int CCTPQuoter::qrySubMarket(char **Instruments, int n) {
	int iResult2 = queryApi->UnSubscribeMarketData(Instruments, n);
	int iResult = queryApi->SubscribeMarketData(Instruments, n);

	return iResult;
}

void CCTPQuoter::genMsg(CCTPQuoter::MSG_TYPE type, string &msg) {
	MSG_LIST_S temp;
	temp.type = type;
	temp.msg = msg;
	MSG_LIST::iterator it = m_MsgList.end();
	m_MsgList.insert(it, temp);
}

void CCTPQuoter::emitMsg(MSG_LIST_S &msg) {
	msg = m_MsgList.front();
	m_MsgList.pop_front();
}

void CCTPQuoter::tick(void) {
	int iResult = login();


}


void CCTPQuoter::OnFrontConnected() {

}

void CCTPQuoter::OnFrontDisconnected(int nReason) {
}

void CCTPQuoter::OnHeartBeatWarning(int nTimeLapse) {

}

void CCTPQuoter::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	int test = 1;
	std::cerr << "BrokerID: " << pRspUserLogin->BrokerID << std::endl;
	std::cerr << "SessionID: " << pRspUserLogin->SessionID << std::endl;
	std::cerr << "UserID: " << pRspUserLogin->UserID << std::endl;

	std::cerr << "Login Status: " << pRspInfo->ErrorMsg << std::endl;
};

void CCTPQuoter::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {

};


void CCTPQuoter::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {

}


void CCTPQuoter::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	if (pRspInfo) {
		std::cerr << pRspInfo->ErrorMsg << std::endl;
	}
}

void CCTPQuoter::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {

}


///订阅询价应答
void CCTPQuoter::OnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {

};

///取消订阅询价应答
void CCTPQuoter::OnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {

};

///深度行情通知
void CCTPQuoter::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData) {
	int test = 1;

#if 0
	if (pDepthMarketData) {
		//std::cerr << "合约代码： " << pDepthMarketData->ExchangeID << std::endl;
		//std::cerr << "日期    ： " << pDepthMarketData->TradingDay << std::endl;
		//std::cerr << "时间    ： " << pDepthMarketData->UpdateTime << std::endl;
		//std::cerr << "毫秒    ： " << pDepthMarketData->UpdateMillisec << std::endl;
		//std::cerr << "均价    ： " << pDepthMarketData->AveragePrice << std::endl;
		//std::cerr << "买价1    ： " << pDepthMarketData->AskPrice1 << std::endl;
		//std::cerr << "买价2    ： " << pDepthMarketData->AskPrice2 << std::endl;
		//std::cerr << "买价3    ： " << pDepthMarketData->AskPrice3 << std::endl;
		//std::cerr << "买价4    ： " << pDepthMarketData->AskPrice4 << std::endl;
		//std::cerr << "买价5    ： " << pDepthMarketData->AskPrice5 << std::endl;
		//std::cerr << "卖价1    ： " << pDepthMarketData->BidPrice1 << std::endl;

		string temp = //g_Frame->getMonProduct1();
		if (!strcmp(temp.c_str(), pDepthMarketData->InstrumentID)) {
			//g_Frame->setMonPrice1(pDepthMarketData->AskPrice1);
			//g_Frame->lastUpdate1 = pDepthMarketData->UpdateMillisec;
			if (//g_Frame->lastUpdate1 == //g_Frame->lastUpdate2) 
				//g_Frame->CalculateDiff();
			//g_Frame->avgPrice1 = pDepthMarketData->AveragePrice;
			//g_Frame->setPrice1(pDepthMarketData->AskPrice1);
		}
		string temp2 = //g_Frame->getMonProduct2();
		if (!strcmp(temp2.c_str(), pDepthMarketData->InstrumentID)) {
			//g_Frame->setMonPrice2(pDepthMarketData->AskPrice1);
			//g_Frame->lastUpdate2 = pDepthMarketData->UpdateMillisec;
			if (//g_Frame->lastUpdate1 == //g_Frame->lastUpdate2)
				//g_Frame->CalculateDiff();
			//g_Frame->avgPrice2 = pDepthMarketData->AveragePrice;
			//g_Frame->setPrice2(pDepthMarketData->AskPrice1);
		}

		if (pDepthMarketData->UpdateMillisec == 0) {
			SWBar bar;
			strncpy(bar.strtime, pDepthMarketData->UpdateTime, 36);
			//bar.close = pDepthMarketData->AskPrice1;
			bar.close = pDepthMarketData->LastPrice;
			bar.bid = pDepthMarketData->BidPrice1;
			bar.ask = pDepthMarketData->AskPrice1;

			string qID;
			qID = pDepthMarketData->InstrumentID;
			//g_Frame->pushSeconds(qID, bar);

			
		}

		int index;
		for (index = 0; index < MAX_STRAT_SLOTS; index++) {
			if (//g_Frame->m_Strat_Run[index] == 1 &&
				!strcmp(//g_Frame->getStratProduct(index).c_str(), pDepthMarketData->InstrumentID)) {
				//g_Frame->SendEventSTMA(index);
			}
		}
		
	}
#endif
}

///询价通知
void CCTPQuoter::OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp) {

};
