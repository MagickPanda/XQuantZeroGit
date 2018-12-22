#include "CQuoter.h"

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new (_NORMAL_BLOCK , __FILE__ , __LINE__)
#define new DBG_NEW
#endif
#endif

int CQuoter::login() {
	CThostFtdcReqUserLoginField loginField;
	memset(&loginField, 0, sizeof(loginField));
	strcpy(loginField.BrokerID, brokerId.c_str());
	strcpy(loginField.UserID, userId.c_str());
	strcpy(loginField.Password, passwd.c_str());
	strcpy(loginField.UserProductInfo, "ashu");

	string sMsg("���ڵ�½���������������\n");
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
		sResult = "��½����������ɹ���\n";
		break;
	case FAILED_NETWORK:
		sResult = "��½�����������������ʧ�ܡ�\n";
		break;
	case FAILED_REACH_LIMITS:
		sResult = "��½���������δ�������󳬹��������\n";
		break;
	case FAILED_REACH_GRANTS:
		sResult = "��½���������ÿ�뷢�����󳬹��������\n";
		break;
	default:
		sResult = "δ֪�����������½ʧ�ܡ�\n";
		break;
	}
	genMsg(MSG_TYPE_LOGIN, sResult);

	return iResult;
}

int CQuoter::logout() {
	CThostFtdcUserLogoutField userLogout;
	memset(&userLogout, 0, sizeof(userLogout));
	strcpy(userLogout.BrokerID, brokerId.c_str());
	strcpy(userLogout.UserID, userId.c_str());

	string sMsg("����ע��������\n");
	genMsg(MSG_TYPE_LOGIN, sMsg);

	string sResult = { 0 };
	int iResult = queryApi->ReqUserLogout(&userLogout, ++reqId);
	switch (iResult) {
	case SUCCESS_POST:
		sResult = "ע���ɹ���\n";
		break;
	case FAILED_NETWORK:
		sResult = "ע����������ʧ�ܡ�\n";
		break;
	case FAILED_REACH_LIMITS:
		sResult = "ע��δ�������󳬹��������\n";
		break;
	case FAILED_REACH_GRANTS:
		sResult = "ע��ÿ�뷢�����󳬹��������\n";
		break;
	default:
		sResult = "δ֪ע��ʧ�ܡ�\n";
		break;
	}
	genMsg(MSG_TYPE_LOGIN, sResult);

	return iResult;
}

int CQuoter::qrySubMarket(char **Instruments, int n) {
	int iResult2 = queryApi->UnSubscribeMarketData(Instruments, n);
	int iResult = queryApi->SubscribeMarketData(Instruments, n);

	return iResult;
}

void CQuoter::genMsg(CQuoter::MSG_TYPE type, string &msg) {
	MSG_LIST_S temp;
	temp.type = type;
	temp.msg = msg;
	MSG_LIST::iterator it = m_MsgList.end();
	m_MsgList.insert(it, temp);
}

void CQuoter::emitMsg(MSG_LIST_S &msg) {
	msg = m_MsgList.front();
	m_MsgList.pop_front();
}

void CQuoter::tick(void) {
	int iResult = login();


}


void CQuoter::OnFrontConnected() {

}

void CQuoter::OnFrontDisconnected(int nReason) {
}

void CQuoter::OnHeartBeatWarning(int nTimeLapse) {

}

void CQuoter::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	int test = 1;
	std::cerr << "BrokerID: " << pRspUserLogin->BrokerID << std::endl;
	std::cerr << "SessionID: " << pRspUserLogin->SessionID << std::endl;
	std::cerr << "UserID: " << pRspUserLogin->UserID << std::endl;

	std::cerr << "Login Status: " << pRspInfo->ErrorMsg << std::endl;
};

void CQuoter::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {

};


void CQuoter::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {

}


void CQuoter::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	if (pRspInfo) {
		std::cerr << pRspInfo->ErrorMsg << std::endl;
	}
}

void CQuoter::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {

}


///����ѯ��Ӧ��
void CQuoter::OnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {

};

///ȡ������ѯ��Ӧ��
void CQuoter::OnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {

};

///�������֪ͨ
void CQuoter::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData) {
	int test = 1;

#if 0
	if (pDepthMarketData) {
		//std::cerr << "��Լ���룺 " << pDepthMarketData->ExchangeID << std::endl;
		//std::cerr << "����    �� " << pDepthMarketData->TradingDay << std::endl;
		//std::cerr << "ʱ��    �� " << pDepthMarketData->UpdateTime << std::endl;
		//std::cerr << "����    �� " << pDepthMarketData->UpdateMillisec << std::endl;
		//std::cerr << "����    �� " << pDepthMarketData->AveragePrice << std::endl;
		//std::cerr << "���1    �� " << pDepthMarketData->AskPrice1 << std::endl;
		//std::cerr << "���2    �� " << pDepthMarketData->AskPrice2 << std::endl;
		//std::cerr << "���3    �� " << pDepthMarketData->AskPrice3 << std::endl;
		//std::cerr << "���4    �� " << pDepthMarketData->AskPrice4 << std::endl;
		//std::cerr << "���5    �� " << pDepthMarketData->AskPrice5 << std::endl;
		//std::cerr << "����1    �� " << pDepthMarketData->BidPrice1 << std::endl;

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

///ѯ��֪ͨ
void CQuoter::OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp) {

};
