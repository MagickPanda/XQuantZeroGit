#include "CTrader.h"

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new (_NORMAL_BLOCK , __FILE__ , __LINE__)
#define new DBG_NEW
#endif
#endif

//CTrader::~CTrader() {
//    //tradeApi->Join();
//    tradeApi->Release();
//    delete m_MsgList;
//    m_MsgList = NULL;
//}

int CTrader::login() {
	CThostFtdcReqUserLoginField loginField;
	memset(&loginField, 0, sizeof(loginField));
	strcpy(loginField.BrokerID, brokerId.c_str());
	strcpy(loginField.UserID, userId.c_str());
	strcpy(loginField.Password, passwd.c_str());
	strcpy(loginField.UserProductInfo, "ashu");

	string sMsg("���ڵ�½������\n");
	//g_Frame->writeLog(sMsg, LOG_TRADE);
	//wxTheApp->GetTopWindow()->CallAfter(&MyFrame::consoleOutput, sMsg);
	string sMsg2("USER ID: ");
	sMsg2.append(loginField.UserID);
	sMsg2.append("\n");
	sMsg2.append("Trade IP : ");
	sMsg2.append(this->tradeIp);
	sMsg2.append("\n");
	//g_Frame->writeLog(sMsg2, LOG_TRADE);
	//wxTheApp->GetTopWindow()->CallAfter(&MyFrame::consoleOutput, sMsg2);

	string sResult = { 0 };
	int iResult = tradeApi->ReqUserLogin(&loginField, ++reqId);
	switch (iResult) {
		case SUCCESS_POST:
			sResult = "��½�ɹ���\n";
			break;
		case FAILED_NETWORK:
			sResult = "��½��������ʧ�ܡ�\n";
			break;
		case FAILED_REACH_LIMITS:
			sResult = "��½δ�������󳬹��������\n";
			break;
		case FAILED_REACH_GRANTS:
			sResult = "��½ÿ�뷢�����󳬹��������\n";
			break;
		default:
			sResult = "δ֪��½ʧ�ܡ�\n";
			break;
	}
	//g_Frame->writeLog(sResult, LOG_TRADE);
	//wxTheApp->GetTopWindow()->CallAfter(&MyFrame::consoleOutput, sResult);

	return iResult;
}

int CTrader::logout() {
	CThostFtdcUserLogoutField userLogout;
	memset(&userLogout, 0, sizeof(userLogout));
	strcpy(userLogout.BrokerID, brokerId.c_str());
	strcpy(userLogout.UserID, userId.c_str());

	string sMsg("����ע��������\n");
	//g_Frame->writeLog(sMsg, LOG_TRADE);
	//wxTheApp->GetTopWindow()->CallAfter(&MyFrame::consoleOutput, sMsg);;

	string sResult = { 0 };
	int iResult = tradeApi->ReqUserLogout(&userLogout, ++reqId);
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
	//g_Frame->writeLog(sResult, LOG_TRADE);
	//wxTheApp->GetTopWindow()->CallAfter(&MyFrame::consoleOutput, sResult);

	return iResult;
}

int CTrader::queryPos() {
	CThostFtdcQryInvestorPositionField QryInvestorPosition;
	memset(&QryInvestorPosition, 0, sizeof(QryInvestorPosition));
	strcpy(QryInvestorPosition.BrokerID, brokerId.c_str());
	strcpy(QryInvestorPosition.InvestorID, userId.c_str());

	string sMsg("���ڻ�ȡ�ֲ֡�����\n");
	//g_Frame->writeLog(sMsg, LOG_TRADE);
	//wxTheApp->GetTopWindow()->CallAfter(&MyFrame::consoleOutput, sMsg);

	string sResult = { 0 };
	int iResult = tradeApi->ReqQryInvestorPosition(&QryInvestorPosition, ++reqId);
	switch (iResult) {
	case SUCCESS_POST:
		sResult = "��ȡ�ֲֳɹ���\n";
		break;
	case FAILED_NETWORK:
		sResult = "��ȡ�ֲ���������ʧ�ܡ�\n";
		break;
	case FAILED_REACH_LIMITS:
		sResult = "��ȡ�ֲ�δ�������󳬹��������\n";
		break;
	case FAILED_REACH_GRANTS:
		sResult = "��ȡ�ֲ�ÿ�뷢�����󳬹��������\n";
		break;
	default:
		sResult = "��ȡ�ֲ�ע��ʧ�ܡ�\n";
		break;
	}
	//g_Frame->writeLog(sResult, LOG_TRADE);
	//wxTheApp->GetTopWindow()->CallAfter(&MyFrame::consoleOutput, sResult);


	return iResult;
}

int  CTrader::insert(string instrumentID, int price, int volume, int code, int reqID)
{
	CThostFtdcInputOrderField inputOrder; //���뱨������
	bool bInsert = false;

	memset(&inputOrder, 0, sizeof(inputOrder));
	//brokerid
	strcpy(inputOrder.BrokerID, brokerId.c_str());
	//Ͷ���ߴ���
	strcpy(inputOrder.InvestorID, userId.c_str());
	strcpy(inputOrder.UserID, userId.c_str());

	strcpy(inputOrder.InstrumentID, instrumentID.c_str());

	inputOrder.MinVolume = 0;
	inputOrder.StopPrice = 0;
	inputOrder.IsAutoSuspend = 0;

	//�۸�
	inputOrder.LimitPrice = price;

	inputOrder.VolumeTotalOriginal = volume;
	//��Ч������ --������Ч
	inputOrder.TimeCondition = THOST_FTDC_TC_GFD;

	strcpy(inputOrder.GTDDate, "");
	//���Ͷ���ױ���־
	inputOrder.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation; 
	//inputOrder.CombHedgeFlag[0] = THOST_FTDC_ECIDT_Hedge;
	//�ɽ������� ///�κ����� THOST_FTDC_VC_AV '1' ///��С���� THOST_FTDC_VC_MV '2' ///ȫ������ THOST_FTDC_VC_CV '3'
	inputOrder.VolumeCondition = THOST_FTDC_VC_AV;

	//��������
	inputOrder.OrderPriceType = THOST_FTDC_OPT_LimitPrice; //�޼۵���ʶ

														   //������
	inputOrder.ContingentCondition = THOST_FTDC_CC_Immediately;
	inputOrder.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
	inputOrder.UserForceClose = 0;
	//inputOrder.Direction = THOST_FTDC_D_Buy;
	//inputOrder.CombOffsetFlag[0] = THOST_FTDC_OF_Open;

	//strcpy(inputOrder.ExchangeID, instrumentID.c_str());
	strcpy(inputOrder.OrderRef, this->CurOrderRef);
	//strcpy(this->LastOrderRef, this->CurOrderRef);
	//inputOrder.RequestID = reqID;
	this->failIncRef();




	//enum {
	//	ORDER_REVERSE = 0,
	//	ORDER_OPEN_BUY = 1, ///<����
	//	ORDER_OPEN_SELL = 2, ///<���� 
	//	ORDER_CLOSE_BUY = 3, ///<ƽ��
	//	ORDER_CLOSE_SELL = 4, ///<ƽ��
	//	ORDER_FOK = 5, ///<�����ɽ���������
	//	ORDER_FAK = 6, ///<�����ɽ���������
	//	ORDER_CONDITION = 7, ///<��������
	//	ORDER_ANYPRICE = 20, ///<�м۵�
	//	ORDER_CONDITION_TOUCH = 30, ///<ֹ��
	//	ORDER_CONDITION_TOUCHPROFIT = 31, ///<ֹӯ
	//};
	string code_type = "���ţ� ";
	switch (code) {
	case ORDER_OPEN_BUY:
		inputOrder.Direction = THOST_FTDC_D_Buy;
		inputOrder.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
		code_type.append("����");
		break;
	case ORDER_OPEN_SELL:
		inputOrder.Direction = THOST_FTDC_D_Sell;
		inputOrder.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
		code_type.append("����");
		break;
	case ORDER_CLOSE_BUY:
		inputOrder.Direction = THOST_FTDC_D_Sell;
		inputOrder.CombOffsetFlag[0] = THOST_FTDC_OF_Close;
		code_type.append("ƽ��");
		break;
	case ORDER_CLOSE_SELL:
		inputOrder.Direction = THOST_FTDC_D_Buy;
		inputOrder.CombOffsetFlag[0] = THOST_FTDC_OF_Close;
		code_type.append("ƽ��");
		break;
	case ORDER_CLOSE_BUY_TODAY:
		inputOrder.Direction = THOST_FTDC_D_Sell;
		inputOrder.CombOffsetFlag[0] = THOST_FTDC_OF_CloseToday;
		code_type.append("ƽ���");
		break;
	case ORDER_CLOSE_SELL_TODAY:
		inputOrder.Direction = THOST_FTDC_D_Buy;
		inputOrder.CombOffsetFlag[0] = THOST_FTDC_OF_CloseToday;
		code_type.append("ƽ���");
		break;
	case ORDER_CLOSE_BUY_YEST:
		inputOrder.Direction = THOST_FTDC_D_Sell;
		inputOrder.CombOffsetFlag[0] = THOST_FTDC_OF_CloseYesterday;
		code_type.append("ƽ���");
		break;
	case ORDER_CLOSE_SELL_YEST:
		inputOrder.Direction = THOST_FTDC_D_Buy;
		inputOrder.CombOffsetFlag[0] = THOST_FTDC_OF_CloseYesterday;
		code_type.append("ƽ���");
		break;
	case ORDER_FOK:
		inputOrder.TimeCondition = THOST_FTDC_TC_IOC; //�����ɽ���������
		inputOrder.VolumeCondition = THOST_FTDC_VC_AV; //~MV������
		inputOrder.ContingentCondition = THOST_FTDC_CC_Immediately;
		code_type.append("FOK");
		break;
	case ORDER_FAK:
		inputOrder.TimeCondition = THOST_FTDC_TC_IOC; //�����ɽ���������
		inputOrder.VolumeCondition = THOST_FTDC_VC_CV; //ȫ������
		inputOrder.ContingentCondition = THOST_FTDC_CC_Immediately;
		code_type.append("FAK");
		break;
	case ORDER_CONDITION:
		inputOrder.ContingentCondition = THOST_FTDC_CC_LastPriceGreaterThanStopPrice; //���¼۴���������
		code_type.append("����������");
		break;
	case ORDER_ANYPRICE:
		//�м۵�
		inputOrder.OrderPriceType = THOST_FTDC_OPT_AnyPrice;
		inputOrder.LimitPrice = 0;
		inputOrder.TimeCondition = THOST_FTDC_TC_IOC;
		inputOrder.VolumeCondition = THOST_FTDC_VC_AV;
		inputOrder.ContingentCondition = THOST_FTDC_CC_Immediately;
		code_type.append("�м۵�");
		break;
	case ORDER_CONDITION_TOUCH:
		inputOrder.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
		inputOrder.TimeCondition = THOST_FTDC_TC_GFD;
		inputOrder.VolumeCondition = THOST_FTDC_VC_AV;
		inputOrder.ContingentCondition = THOST_FTDC_CC_Touch;
		code_type.append("ֹ��");
		break;
	case ORDER_CONDITION_TOUCHPROFIT:
		inputOrder.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
		inputOrder.TimeCondition = THOST_FTDC_TC_GFD;
		inputOrder.VolumeCondition = THOST_FTDC_VC_AV;
		inputOrder.ContingentCondition = THOST_FTDC_CC_TouchProfit;
		code_type.append("ֹӯ");
		break;
	default:
		break;
	}


	string sMsg("�����µ���");
	sMsg.append(instrumentID);
	sMsg.append("\n");
	//g_Frame->writeLog(sMsg, LOG_TRADE);
	//wxTheApp->GetTopWindow()->CallAfter(&MyFrame::consoleOutput, sMsg);

	//g_Frame->writeLog(code_type, LOG_TRADE);
	//wxTheApp->GetTopWindow()->CallAfter(&MyFrame::consoleOutput, code_type);

	string sResult = { 0 };
	int iResult = tradeApi->ReqOrderInsert(&inputOrder, reqID);
	switch (iResult) {
	case SUCCESS_POST:
		sResult = "�µ��ɹ���\n";
		break;
	case FAILED_NETWORK:
		sResult = "�µ���������ʧ�ܡ�\n";
		break;
	case FAILED_REACH_LIMITS:
		sResult = "�µ�δ�������󳬹��������\n";
		break;
	case FAILED_REACH_GRANTS:
		sResult = "�µ�ÿ�뷢�����󳬹��������\n";
		break;
	default:
		sResult = "�µ�ʧ�ܡ�\n";
		break;
	}
	//g_Frame->writeLog(sResult, LOG_TRADE);
	//wxTheApp->GetTopWindow()->CallAfter(&MyFrame::consoleOutput, sResult);

	return iResult;
}

int  CTrader::insert2(string instrumentID, int price, int volume, int code, int reqID, int FrontID, int sessionID, string orderRef)
{
	CThostFtdcInputOrderField inputOrder; //���뱨������
	bool bInsert = false;

	memset(&inputOrder, 0, sizeof(inputOrder));
	//brokerid
	strcpy(inputOrder.BrokerID, brokerId.c_str());
	//Ͷ���ߴ���
	strcpy(inputOrder.InvestorID, userId.c_str());
	strcpy(inputOrder.UserID, userId.c_str());

	strcpy(inputOrder.InstrumentID, instrumentID.c_str());

	//inputOrder.FrontID = FrontID;
	//inputOrder.SessionID = sessionID;

	strcpy(inputOrder.OrderRef, orderRef.c_str());

	strcpy(inputOrder.ExchangeID, instrumentID.c_str());
	inputOrder.RequestID = reqID;

	//�۸�
	inputOrder.LimitPrice = price;

	inputOrder.VolumeTotalOriginal = volume;
	//��Ч������ --������Ч
	inputOrder.TimeCondition = THOST_FTDC_TC_GFD;
	//���Ͷ���ױ���־
	inputOrder.CombHedgeFlag[0] = THOST_FTDC_ECIDT_Hedge;
	//�ɽ������� ///�κ����� THOST_FTDC_VC_AV '1' ///��С���� THOST_FTDC_VC_MV '2' ///ȫ������ THOST_FTDC_VC_CV '3'
	inputOrder.VolumeCondition = THOST_FTDC_VC_AV;

	//��������
	inputOrder.OrderPriceType = THOST_FTDC_OPT_LimitPrice; //�޼۵���ʶ

	inputOrder.ContingentCondition = THOST_FTDC_CC_Immediately; //������
	inputOrder.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
	inputOrder.UserForceClose = 0;


	string code_type = "���ţ� ";
	switch (code) {
	case ORDER_CLOSE_BUY:
		inputOrder.Direction = THOST_FTDC_D_Buy;
		inputOrder.CombOffsetFlag[0] = THOST_FTDC_OF_Close;
		code_type.append("ƽ��");
		break;
	case ORDER_CLOSE_SELL:
		inputOrder.Direction = THOST_FTDC_D_Sell;
		inputOrder.CombOffsetFlag[0] = THOST_FTDC_OF_Close;
		code_type.append("ƽ��");
		break;
	case ORDER_CLOSE_BUY_TODAY:
		inputOrder.Direction = THOST_FTDC_D_Buy;
		inputOrder.CombOffsetFlag[0] = THOST_FTDC_OF_CloseToday;
		code_type.append("ƽ���");
		break;
	case ORDER_CLOSE_SELL_TODAY:
		inputOrder.Direction = THOST_FTDC_D_Sell;
		inputOrder.CombOffsetFlag[0] = THOST_FTDC_OF_CloseToday;
		code_type.append("ƽ���");
		break;
	case ORDER_CLOSE_BUY_YEST:
		inputOrder.Direction = THOST_FTDC_D_Buy;
		inputOrder.CombOffsetFlag[0] = THOST_FTDC_OF_CloseYesterday;
		code_type.append("ƽ���");
		break;
	case ORDER_CLOSE_SELL_YEST:
		inputOrder.Direction = THOST_FTDC_D_Sell;
		inputOrder.CombOffsetFlag[0] = THOST_FTDC_OF_CloseYesterday;
		code_type.append("ƽ���");
		break;
	default:
		break;
	}

	string sMsg("����ƽ�֣�");
	sMsg.append(instrumentID);
	sMsg.append("\n");
	//g_Frame->writeLog(sMsg, LOG_TRADE);
	//wxTheApp->GetTopWindow()->CallAfter(&MyFrame::consoleOutput, sMsg);

	//g_Frame->writeLog(code_type, LOG_TRADE);
	//wxTheApp->GetTopWindow()->CallAfter(&MyFrame::consoleOutput, code_type);

	string sResult = { 0 };
	int iResult = tradeApi->ReqOrderInsert(&inputOrder, reqID);
	switch (iResult) {
	case SUCCESS_POST:
		sResult = "�µ��ɹ���\n";
		break;
	case FAILED_NETWORK:
		sResult = "�µ���������ʧ�ܡ�\n";
		break;
	case FAILED_REACH_LIMITS:
		sResult = "�µ�δ�������󳬹��������\n";
		break;
	case FAILED_REACH_GRANTS:
		sResult = "�µ�ÿ�뷢�����󳬹��������\n";
		break;
	default:
		sResult = "�µ�ʧ�ܡ�\n";
		break;
	}
	//g_Frame->writeLog(sResult, LOG_TRADE);
	//wxTheApp->GetTopWindow()->CallAfter(&MyFrame::consoleOutput, sResult);

	return iResult;
}

///
int  CTrader::insertAction(string instrumentID, int price, int volume, int code, int reqID, int FrontID, int sessionID, string orderRef)
{
	CThostFtdcInputOrderActionField inputOrder; //���뱨������
	bool bInsert = false;

	memset(&inputOrder, 0, sizeof(inputOrder));
	//brokerid
	strcpy(inputOrder.BrokerID, brokerId.c_str());
	//Ͷ���ߴ���
	strcpy(inputOrder.InvestorID, userId.c_str());
	strcpy(inputOrder.UserID, userId.c_str());

	strcpy(inputOrder.InstrumentID, instrumentID.c_str());

	inputOrder.FrontID = FrontID;
	inputOrder.SessionID = sessionID;

	strcpy(inputOrder.OrderRef, orderRef.c_str());

	strcpy(inputOrder.ExchangeID, instrumentID.c_str());
	strcpy(inputOrder.OrderRef, this->CurOrderRef);
	//strcpy(this->LastOrderRef, this->CurOrderRef);
	inputOrder.RequestID = reqID;
	this->failIncRef();



	string sMsg("����ƽ�֣�");
	sMsg.append(instrumentID);
	sMsg.append("\n");
	//g_Frame->writeLog(sMsg, LOG_TRADE);
	//wxTheApp->GetTopWindow()->CallAfter(&MyFrame::consoleOutput, sMsg);
	

	string sResult = { 0 };
	int iResult = tradeApi->ReqOrderAction(&inputOrder, reqID);
	switch (iResult) {
	case SUCCESS_POST:
		sResult = "�µ��ɹ���\n";
		break;
	case FAILED_NETWORK:
		sResult = "�µ���������ʧ�ܡ�\n";
		break;
	case FAILED_REACH_LIMITS:
		sResult = "�µ�δ�������󳬹��������\n";
		break;
	case FAILED_REACH_GRANTS:
		sResult = "�µ�ÿ�뷢�����󳬹��������\n";
		break;
	default:
		sResult = "�µ�ʧ�ܡ�\n";
		break;
	}
	//g_Frame->writeLog(sResult, LOG_TRADE);
	//wxTheApp->GetTopWindow()->CallAfter(&MyFrame::consoleOutput, sResult);

	return iResult;
}

///����
int CTrader::orderAction(int FrontID, int SessionID, char* OrderRef, string InstrumentID)
{
	CThostFtdcInputOrderActionField InputOrderAction;
	memset(&InputOrderAction, 0, sizeof(InputOrderAction));
	strcpy(InputOrderAction.BrokerID, brokerId.c_str());
	strcpy(InputOrderAction.InvestorID, userId.c_str());


	InputOrderAction.FrontID = FrontID;
	InputOrderAction.SessionID = SessionID;
	strcpy(InputOrderAction.OrderRef, OrderRef);

	//strcpy(InputOrderAction.ExchangeID, ExchangeID);
	//strcpy(InputOrderAction.OrderSysID, OrderSysID);

	InputOrderAction.ActionFlag = THOST_FTDC_AF_Delete;
	strcpy(InputOrderAction.InstrumentID, InstrumentID.c_str());  //����Ǳ����
	int iResult = tradeApi->ReqOrderAction(&InputOrderAction, ++reqId);

	return iResult;
}

int CTrader::orderAction2(char * orderSysID, string instrumentID)
{
	CThostFtdcInputOrderActionField InputOrderAction;
	memset(&InputOrderAction, 0, sizeof(InputOrderAction));
	strcpy(InputOrderAction.BrokerID, brokerId.c_str());
	strcpy(InputOrderAction.InvestorID, userId.c_str());


	strncpy(InputOrderAction.ExchangeID, instrumentID.c_str(), 9);
	strncpy(InputOrderAction.OrderSysID, orderSysID, 21);

	//strcpy(InputOrderAction.ExchangeID, ExchangeID);
	//strcpy(InputOrderAction.OrderSysID, OrderSysID);

	InputOrderAction.ActionFlag = THOST_FTDC_AF_Delete;
	int iResult = tradeApi->ReqOrderAction(&InputOrderAction, ++reqId);

	return iResult;
}

int CTrader::queryOrderAction(string instrumentID, TThostFtdcOrderRefType orderRef, int reqID)
{
	CThostFtdcInputOrderActionField InputOrderAction;
	memset(&InputOrderAction, 0, sizeof(InputOrderAction));
	strcpy(InputOrderAction.BrokerID, brokerId.c_str());
	strcpy(InputOrderAction.InvestorID, userId.c_str());

	InputOrderAction.FrontID = FrontID;
	InputOrderAction.SessionID = SessionID;
	strcpy(InputOrderAction.OrderRef, orderRef);
	//strcpy(InputOrderAction.ExchangeID, ExchangeID);
	//strcpy(InputOrderAction.OrderSysID, OrderSysID);

	InputOrderAction.ActionFlag = THOST_FTDC_AF_Delete;
	strcpy(InputOrderAction.InstrumentID, instrumentID.c_str());  //����Ǳ����

	int iResult;
	iResult = tradeApi->ReqOrderAction(&InputOrderAction, reqID);
	return iResult;
}

int CTrader::queryTrade(string instrumentID, int reqID)
{
	CThostFtdcQryTradeField QryTrade;
	memset(&QryTrade, 0, sizeof(QryTrade));
	strcpy(QryTrade.BrokerID, brokerId.c_str());
	strcpy(QryTrade.InvestorID, userId.c_str());

	int iResult;
	iResult = tradeApi->ReqQryTrade(&QryTrade, reqID);
	return iResult;
}

int CTrader::queryOrder(string instrumentID, int reqID)
{
	CThostFtdcQryOrderField QryOrder;
	memset(&QryOrder, 0, sizeof(QryOrder));
	strcpy(QryOrder.BrokerID, brokerId.c_str());
	strcpy(QryOrder.InvestorID, userId.c_str());

	int iResult;
	iResult = tradeApi->ReqQryOrder(&QryOrder, reqID);
	return iResult;
}

int  CTrader::queryDepthData(string instrumentID)
{
	char _instrumentId[10];
	CThostFtdcQryDepthMarketDataField QryDepthMarketData;
	memset(&QryDepthMarketData, 0, sizeof(QryDepthMarketData));
	strcpy(QryDepthMarketData.InstrumentID, instrumentID.c_str());
	int iResult = tradeApi->ReqQryDepthMarketData(&QryDepthMarketData, ++reqId);

	string sMsg("���ڻ�ȡ���飺");
	sMsg.append(instrumentID);
	sMsg.append("\n");
	//g_Frame->writeLog(sMsg, LOG_TRADE);
	//wxTheApp->GetTopWindow()->CallAfter(&MyFrame::consoleOutput, sMsg);

	string sResult = { 0 };
	switch (iResult) {
	case SUCCESS_POST:
		sResult = "��ȡ����ɹ���\n";
		break;
	case FAILED_NETWORK:
		sResult = "��ȡ������������ʧ�ܡ�\n";
		break;
	case FAILED_REACH_LIMITS:
		sResult = "��ȡ����δ�������󳬹��������\n";
		break;
	case FAILED_REACH_GRANTS:
		sResult = "��ȡ����ÿ�뷢�����󳬹��������\n";
		break;
	default:
		sResult = "��ȡ����ע��ʧ�ܡ�\n";
		break;
	}
	//g_Frame->writeLog(sResult, LOG_TRADE);
	//wxTheApp->GetTopWindow()->CallAfter(&MyFrame::consoleOutput, sResult);

	return iResult;
}

int CTrader::qryTradingAccount() {
	string sMsg("���ڻ�ȡ���飺");

	CThostFtdcQryTradingAccountField QryTradingAccount;
	memset(&QryTradingAccount, 0, sizeof(QryTradingAccount));
	strcpy(QryTradingAccount.BrokerID, brokerId.c_str());
	strcpy(QryTradingAccount.InvestorID, userId.c_str());
	strcpy(QryTradingAccount.CurrencyID, "CNY");

	sMsg.append("\n");
	sMsg.append("UserID: ");
	sMsg.append(QryTradingAccount.InvestorID);
	sMsg.append("\n");

	//g_Frame->writeLog(sMsg, LOG_TRADE);
	//wxTheApp->GetTopWindow()->CallAfter(&MyFrame::consoleOutput, sMsg);

	int iResult = tradeApi->ReqQryTradingAccount(&QryTradingAccount, ++reqId);

	return iResult;
}

void CTrader::tick(void) {
	int iResult = login();

	
}

///������ȷ��
void CTrader::settleConfirm(void)
{
	CThostFtdcSettlementInfoConfirmField SettlementInfoConfirm;
	strcpy(SettlementInfoConfirm.BrokerID, brokerId.c_str());
	strcpy(SettlementInfoConfirm.InvestorID, userId.c_str());
	int iResult = tradeApi->ReqSettlementInfoConfirm(&SettlementInfoConfirm, ++reqId);
}

void CTrader::OnFrontConnected()
{
	//g_Frame->consoleOutput("���׷����������ӡ�\n");
}

void CTrader::OnFrontDisconnected(int nReason)
{
	std::cerr << "--->>> " << __FUNCTION__ << std::endl;
	std::cerr << "--->>> " << "reason: " << nReason << std::endl;

	//g_Frame->m_Loader->removeCon();
	//g_Frame->Reconnect();
}

void CTrader::OnHeartBeatWarning(int nTimeLapse)
{
	std::cerr << "--->>> " << __FUNCTION__ << std::endl;
	std::cerr << "--->>> " << nTimeLapse << std::endl;
}

void CTrader::OnRspAuthenticate(CThostFtdcRspAuthenticateField *pRspAuthenticateField, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	std::cerr << "--->>> " << __FUNCTION__ << std::endl;
	std::cerr << "brokerID: " << pRspAuthenticateField->BrokerID << " UserID: " << pRspAuthenticateField->UserID << " ProductInfo: " << pRspAuthenticateField->UserProductInfo << std::endl;
	std::cerr << "RspInfo: " << pRspInfo->ErrorMsg << std::endl;
}

void CTrader::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	//SessionID = pRspUserLogin->SessionID;
	//FrontID = pRspUserLogin->FrontID;
	std::cerr << "--->>> " << __FUNCTION__ << std::endl;
	std::cerr << "brokerID: " << pRspUserLogin->BrokerID << " UserID: " << pRspUserLogin->UserID << " ProductInfo: " << std::endl;
	std::cerr << "RspInfo: " << pRspInfo->ErrorMsg << std::endl;

	if (pRspInfo->ErrorID != 0) {
		std::cerr << "Error Login RspInfo: " << pRspInfo->ErrorMsg << std::endl;
		return;
	}

	CurOrderRefInt = atoi(pRspUserLogin->MaxOrderRef);
	snprintf(this->CurOrderRef, 13, "%.12d", CurOrderRefInt);
	strcpy(this->MaxOrderRef, this->CurOrderRef);
	this->FrontID = pRspUserLogin->FrontID;
	this->SessionID = pRspUserLogin->SessionID;

	//g_Frame->SendEventSMTAGUI();
}

void CTrader::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	std::cerr << "--->>> " << __FUNCTION__ << std::endl;
	std::cerr << "brokerID: " << pUserLogout->BrokerID << " UserID: " << pUserLogout->UserID << std::endl;
	std::cerr << "RspInfo: " << pRspInfo->ErrorMsg << std::endl;
}

void CTrader::OnRspUserPasswordUpdate(CThostFtdcUserPasswordUpdateField *pUserPasswordUpdate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	std::cerr << "--->>> " << __FUNCTION__ << std::endl;
	std::cerr << "brokerID: " << pUserPasswordUpdate->BrokerID << " UserID: " << pUserPasswordUpdate->UserID
		<< " oldPasswd: " << pUserPasswordUpdate->OldPassword << " newPasswd: " << pUserPasswordUpdate->NewPassword << std::endl;
	std::cerr << "--->>> " << "RspInfo: " << pRspInfo->ErrorMsg << std::endl;
}

///����Ӧ��
void CTrader::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	std::cerr << "--->>> " << __FUNCTION__ << std::endl;
	std::cerr << "ErrorID: " << pRspInfo->ErrorID << " ErrorMsg: " << pRspInfo->ErrorMsg
		<< std::endl;
}

///����¼��������Ӧ
void CTrader::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (pRspInfo) {
		//wxTheApp->GetTopWindow()->CallAfter(&MyFrame::consoleOutput, pRspInfo->ErrorMsg);
	}

	std::cerr << "--->>> " << __FUNCTION__ << std::endl;
	std::cerr << "brokerID: " << pInputOrder->BrokerID << " UserID: " << pInputOrder->UserID
		<< " instrumentId: " << pInputOrder->InstrumentID << " ��������: " << (pInputOrder->Direction == '0' ? "��" : "��")
		<< " ��Ͽ�ƽ��־: " << (pInputOrder->CombOffsetFlag[0] == '0' ? "����" : "ƽ��") << std::endl;
	std::cerr << "--->>> " << "RspInfo: " << pRspInfo->ErrorMsg << std::endl;

	string temp(pInputOrder->OrderRef);

	//vector<order_struct>::iterator it = //g_Frame->m_order_list.begin();
	//for (; it != //g_Frame->m_order_list.end(); it++) {

#if 0
	//TODO: make this thread safe, thread-related bugs!
	//wxMutexLocker	lock(s_order_list_lock);
	for (int index = 0; index < //g_Frame->m_order_list.size(); index++) {
		order_struct &target = //g_Frame->m_order_list.at(index);
		if (!strcmp(temp.c_str(), target.OrderRef1)) {
			int test = 1;
		}

		if (!strcmp(temp.c_str(), target.OrderRef1)) {

			/*strcpy((*it).OrderSysID1, pInputOrder->OrderSysID);*/
			int type = target.type;

			String t, t2;

			switch (type) {
				case OS_ST_INSERTING:
					target.type = OS_ST_NONE;

					//g_Frame->m_Loader->write_order(default_order_path, target);
					//g_Frame->updateGridsExt();


					t2 = os_st_table[target.type];
					t = __FUNCTION__ + String(" errorID: ") + pRspInfo->ErrorMsg + t2 + String("reqID1: ") + String::Format(wxT("%i"), target.reqID1);
					t.append(" ");
					t.append("OS_ST_INSERTING Reverting to OS_ST_NONE\n");
					//g_Frame->writeLog(t.ToStdString(), LOG_TRADE);
					//wxTheApp->GetTopWindow()->CallAfter(&MyFrame::consoleOutput, t);
				case OS_ST_FINISHING:
					target.type = OS_ST_NONE;

					//g_Frame->m_Loader->write_order(default_order_path, target);
					//g_Frame->updateGridsExt();


					t2 = os_st_table[target.type];
					t = __FUNCTION__ + String(" errorID: ") + pRspInfo->ErrorMsg + t2 + String("reqID1: ")  + String::Format(wxT("%i"), target.reqID1);
					t.append(" ");
					t.append("OS_ST_FINISH Reverting to OS_ST_NONE\n");
					//g_Frame->writeLog(t.ToStdString(), LOG_TRADE);
					//wxTheApp->GetTopWindow()->CallAfter(&MyFrame::consoleOutput, t);
					break;
				default:
					t2 = os_st_table[target.type];
					t = __FUNCTION__ + String(" errorID: ") + pRspInfo->ErrorMsg + t2 + String("reqID1: ") + String::Format(wxT("%i"), target.reqID1);
					t.append(" ");
					t.append("UNKNOWN STATUS Reverting to OS_ST_NONE\n");
					//g_Frame->writeLog(t.ToStdString(), LOG_TRADE);
					//wxTheApp->GetTopWindow()->CallAfter(&MyFrame::consoleOutput, t);
					break;
			}


			//if (target.type != OS_ST_INSERTING) {
			//	string t2(os_st_table[target.type]);
			//	string t = "WARN:invalid order before inserted: " + t2;
			//	t.append("\n");
			//	//wxTheApp->GetTopWindow()->CallAfter(&MyFrame::consoleOutput, t);
			//}

			////target.type = OS_ST_NONE;

			//strcpy(target.InsertDate, sw_getDate().c_str());
			//strcpy(target.InsertTime, sw_getTime().c_str());

			//g_Frame->m_Loader->write_order(default_order_path, target);
			//g_Frame->updateGridsExt();

			break;

		}
	}
#endif
}

///Ͷ���߽�����ȷ����Ӧ
void CTrader::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	std::cerr << "--->>> " << __FUNCTION__ << std::endl;
	std::cerr << "brokerID: " << pSettlementInfoConfirm->BrokerID << " UserID: " << pSettlementInfoConfirm->InvestorID
		<< std::endl;
	std::cerr << "RspInfo: " << pRspInfo->ErrorMsg << std::endl;
}

///����֪ͨ
void CTrader::OnRtnOrder(CThostFtdcOrderField *pOrder)
{
	//strcpy(InstrumentID, pOrder->InstrumentID);
	//strcpy(OrderRef, pOrder->OrderRef);
	//strcpy(ExchangeID, pOrder->ExchangeID);
	//strcpy(OrderSysID, pOrder->OrderSysID);

	std::cerr << "--->>>" << __FUNCTION__ << std::endl;
	std::cerr << "InvestorID: " << pOrder->InvestorID << " InstrumentID: " << pOrder->InstrumentID << " OrderStatus: " << pOrder->OrderStatus << " OrderRef" << pOrder->OrderRef
		<< "OrderStatus" << pOrder->StatusMsg << std::endl;

	string temp(pOrder->OrderRef);
	
#if 0
	/*vector<order_struct>::iterator it = //g_Frame->m_order_list.begin();*/
	int count = 0;

	wxMutexLocker	lock(s_order_list_lock);
	/*for (; it != //g_Frame->m_order_list.end(); ++it) {*/
	for (int index = 0; index < //g_Frame->m_order_list.size(); index++) {
		order_struct& target = //g_Frame->m_order_list.at(index);
		if (!strcmp(temp.c_str(), target.OrderRef1)) {
			int test = 1;
		}

		// ��֤�ط�ʱ���
		int time_old = sprintf("%d", target.UpdateTime);
		int time_new = sprintf("%d", pOrder->UpdateTime);
		if (time_old > time_new) {
			string temp;
			temp.append(target.UpdateTime);
			temp.append(" ");
			temp.append("<");
			temp.append(" ");
			temp.append(pOrder->UpdateTime);
			temp.append(" ");
			temp.append("ignoring.");

			//g_Frame->writeLog(temp, LOG_QUOTE);
			continue;
		}

		count++;

		if (!strcmp(temp.c_str(), target.OrderRef1) &&
			pOrder->FrontID == target.FrontID1 &&
			pOrder->SessionID == target.SessionID1) {

			strcpy(target.OrderSysID1, pOrder->OrderSysID );

			bool bValidStatus = false;
			string	s_OrderStatus;

			switch (pOrder->OrderStatus) {
			case THOST_FTDC_OST_AllTraded:///ȫ���ɽ�
				bValidStatus = true;
				s_OrderStatus = "ȫ���ɽ�";
				if (target.type == OS_ST_FINISHING || 
					target.type == OS_ST_FINISHING_ACK || 
					target.type == OS_ST_FINISH_CANCEL ||
					target.type == OS_ST_FINISH_CANCEL_ACK) {
					target.type = OS_ST_FINISHED;
					//g_Frame->m_Loader->write_order(default_order_path, target);
					//g_Frame->updateGridsExt();
					break;
				}
				if (target.type == OS_ST_INSERTED || 
					target.type == OS_ST_INSERTING ||
					target.type == OS_ST_INSERTING_ACK ||
					target.type == OS_ST_INSERT_CANCEL ||
					target.type == OS_ST_INSERT_CANCEL_ACK) {
					target.type = OS_ST_CONFIRMED;
					//g_Frame->m_Loader->write_order(default_order_path, target);
					//g_Frame->updateGridsExt();
					break;
				}
				break;
			case THOST_FTDC_OST_PartTradedQueueing:///���ֳɽ����ڶ�����
				s_OrderStatus = "���ֳɽ����ڶ�����";
				bValidStatus = true;
				//if (target.type == OS_ST_FINISHING) {
				//	target.type = OS_ST_FINISHED;
				//	//g_Frame->m_Loader->write_order(default_order_path, target);
				//	//g_Frame->updateGridsExt();
				//	break;
				//}
				//if (target.type == OS_ST_INSERTED ||
				//	target.type == OS_ST_INSERTING ||
				//	target.type == OS_ST_INSERTING_ACK) {
				//	target.type = OS_ST_CONFIRMED;
				//	//g_Frame->m_Loader->write_order(default_order_path, target);
				//	//g_Frame->updateGridsExt();
				//	break;
				//}
				break;
			case THOST_FTDC_OST_NoTradeQueueing:///���ֳɽ����ڶ�����
				s_OrderStatus = "���ֳɽ����ڶ�����";
				bValidStatus = true;
				//if (target.type == OS_ST_FINISHING) {
				//	target.type = OS_ST_FINISHED;
				//	//g_Frame->m_Loader->write_order(default_order_path, target);
				//	//g_Frame->updateGridsExt();
				//	break;
				//}
				//if (target.type == OS_ST_INSERTED ||
				//	target.type == OS_ST_INSERTING ||
				//	target.type == OS_ST_INSERTING_ACK) {
				//	target.type = OS_ST_CONFIRMED;
				//	//g_Frame->m_Loader->write_order(default_order_path, target);
				//	//g_Frame->updateGridsExt();
				//	break;
				//}
				break;
			case THOST_FTDC_OST_PartTradedNotQueueing:///δ�ɽ����ڶ�����
				s_OrderStatus = "δ�ɽ����ڶ�����";
				bValidStatus = true;
				break;
			case THOST_FTDC_OST_Canceled:	///����
				s_OrderStatus = "�ѳ���";
				bValidStatus = true;
				if (target.type == OS_ST_INSERT_CANCEL) {
					target.type = OS_ST_INSERT_CANCELLED;
					//g_Frame->m_Loader->write_order(default_order_path, target);
					//g_Frame->updateGridsExt();
					break;
				}
				if (target.type == OS_ST_FINISH_CANCEL) {
					target.type = OS_ST_FINISH_CANCELLED;
					//g_Frame->m_Loader->write_order(default_order_path, target);
					//g_Frame->updateGridsExt();
					break;
				}
				break;
			case THOST_FTDC_OST_Unknown:///δ֪
				s_OrderStatus = "δ֪";
				bValidStatus = false;
				break;
			case THOST_FTDC_OST_NotTouched:	///��δ����
				s_OrderStatus = "��δ����";
				bValidStatus = true;
				break;
			case THOST_FTDC_OST_Touched:///�Ѵ���
				s_OrderStatus = "�Ѵ���";
				bValidStatus = true;
				break;
			default:
				s_OrderStatus = "δ֪����״̬";
				break;

			}

			if (bValidStatus) {
				//if (target.type == OS_ST_INSERTING) {
				//	target.type = OS_ST_INSERTING_ACK;
				//	//g_Frame->m_Loader->write_order(default_order_path, target);
				//	//g_Frame->updateGridsExt();
				//	//break;
				//}

				//if (target.type == OS_ST_FINISHING) {
				//	target.type = OS_ST_FINISHING_ACK;
				//	//g_Frame->m_Loader->write_order(default_order_path, target);
				//	//g_Frame->updateGridsExt();
				//	//break;
				//}

				//if (target.type == OS_ST_INSERT_CANCEL) {
				//	target.type = OS_ST_INSERT_CANCEL_ACK;
				//	//g_Frame->m_Loader->write_order(default_order_path, target);
				//	//g_Frame->updateGridsExt();
				//	//break;
				//}

				//if (target.type == OS_ST_FINISH_CANCEL) {
				//	target.type = OS_ST_FINISH_CANCEL_ACK;
				//	//g_Frame->m_Loader->write_order(default_order_path, target);
				//	//g_Frame->updateGridsExt();
				//	//break;
				//}
			}


			if (target.type > 14) {
				int fail = 1;
			}

			string t2(os_st_table[target.type]);
			string t = "WARN:OnRtnOrder invalid order before confirmed: " + t2;
			t.append("\n");
			t.append(s_OrderStatus);
			t.append("\n");

			std::cerr << "ReqID: " << target.reqID1 << std::endl;

			//wxTheApp->GetTopWindow()->CallAfter(&MyFrame::consoleOutput, t);

			//g_Frame->writeLog(t, LOG_ORDER);

			if (bValidStatus) {
				int test = 1;
			}

			continue;

		}
	}
#endif

}

///�ɽ�֪ͨ
void CTrader::OnRtnTrade(CThostFtdcTradeField *pTrade)
{
	std::cerr << "--->>>" << __FUNCTION__ << std::endl;
	std::cerr << "InvestorID: " << pTrade->InvestorID << " InstrumentID: " << pTrade->InstrumentID << " OrderSysID: " << pTrade->OrderSysID
		  << std::endl;

	string temp(pTrade->OrderRef);

#if 0

	/*vector<order_struct>::iterator it = //g_Frame->m_order_list.begin();*/
	int count = 0;
	/*for (; it != //g_Frame->m_order_list.end(); it++) {*/

	bool bFound = false;

	wxMutexLocker	lock(s_order_list_lock);
	for (int index = 0; index < //g_Frame->m_order_list.size(); index++) {
		order_struct& target = //g_Frame->m_order_list.at(index);
		if (!strcmp(pTrade->OrderRef, target.OrderRef1)) {
			//!strcmp(pTrade->ExchangeID, (*it).ExchangeID1 ) ) {

			bFound = true;

			count++;

			//strcpy(target.OrderSysID1, pTrade->OrderSysID);

			//if (target.type == OS_ST_INSERTING_ACK ||
			//	target.type == OS_ST_INSERTING) {
			//	target.type = OS_ST_CONFIRMED;
			//	//g_Frame->m_Loader->write_order(default_order_path, target);
			//	//g_Frame->updateGridsExt();
			//	break;
			//}

			//if (target.type == OS_ST_FINISHING_ACK ||
			//	target.type == OS_ST_FINISHING) {
			//	target.type = OS_ST_FINISHED;
			//	//g_Frame->m_Loader->write_order(default_order_path, target);
			//	//g_Frame->updateGridsExt();
			//	break;
			//}
			//
			//if (target.type == OS_ST_INSERT_CANCEL_ACK ||
			//	target.type == OS_ST_INSERT_CANCEL) {
			//	target.type = OS_ST_INSERT_CANCELLED;
			//	//g_Frame->m_Loader->write_order(default_order_path, target);
			//	//g_Frame->updateGridsExt();
			//	break;
			//}

			//if (target.type == OS_ST_FINISH_CANCEL_ACK ||
			//	target.type == OS_ST_FINISH_CANCEL) {
			//	target.type = OS_ST_FINISH_CANCELLED;
			//	//g_Frame->m_Loader->write_order(default_order_path, target);
			//	//g_Frame->updateGridsExt();
			//	break;
			//}

			//if (target.type == OS_ST_INSERTING) {
			//	target.type = OS_ST_CONFIRMED;
			//	//g_Frame->m_Loader->write_order(default_order_path, target);
			//	//g_Frame->updateGridsExt();
			//	break;
			//}

			//if (target.type == OS_ST_FINISHING) {
			//	target.type = OS_ST_FINISHED;
			//	//g_Frame->m_Loader->write_order(default_order_path, target);
			//	//g_Frame->updateGridsExt();
			//	break;
			//}

			//string t2(os_st_table[target.type]);
			//string t = "WARN:invalid order before confirmed: " + t2;
			//t.append("\n");

			//std::cerr << "ReqID: " << target.reqID1 << std::endl;

			//wxTheApp->GetTopWindow()->CallAfter(&MyFrame::consoleOutput, t);

			//g_Frame->writeLog(t, LOG_ORDER);


			continue;
		}
	}

	if (bFound == false) {
		int fail = 1;
	}
#endif
}

///����¼�����ر�
void CTrader::OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo)
{
	std::cerr << "--->>>" << __FUNCTION__ << std::endl;
	std::cerr << "InvestorID: " << pInputOrder->InvestorID << " InstrumentID: " << pInputOrder->InstrumentID << " OrderRef: " << pInputOrder->OrderRef << std::endl;
	std::cerr << "ErrorMsg: " << pRspInfo->ErrorMsg << std::endl;

	string temp(pInputOrder->OrderRef);

#if 0

	/*vector<order_struct>::iterator it = //g_Frame->m_order_list.begin();*/
	int count = 0;
	/*for (; it != //g_Frame->m_order_list.end(); it++) {*/

	bool bFound = false;

	wxMutexLocker	lock(s_order_list_lock);
	for (int index = 0; index < //g_Frame->m_order_list.size(); index++) {
		order_struct& target = //g_Frame->m_order_list.at(index);
		if (!strcmp(pInputOrder->OrderRef, target.OrderRef1)) {
			//!strcmp(pInputOrder->ExchangeID, (*it).ExchangeID1 ) ) {

			target.type = OS_ST_NONE;
			//g_Frame->m_Loader->write_order(default_order_path, target);
			//g_Frame->updateGridsExt();


			String t2 = os_st_table[target.type];
			String t = __FUNCTION__ + String(" errorID: ") + pRspInfo->ErrorMsg + t2 + String("reqID1: ") + String::Format(wxT("%i"), target.reqID1);
			t.append(" ");
			t.append("OS_ST_INSERTING Reverting to OS_ST_NONE\n");
			//g_Frame->writeLog(t.ToStdString(), LOG_TRADE);

			bFound = true;

			count++;
			break;
		}
	}
#endif
}

///������������ر�
void CTrader::OnErrRtnOrderAction(CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo)
{
	std::cerr << "--->>>" << __FUNCTION__ << std::endl;
	std::cerr << "InvestorID: " << pOrderAction->InvestorID << " InstrumentID: " << pOrderAction->InstrumentID << " OrderRef: " << pOrderAction->OrderRef << std::endl;
	std::cerr << "ErrorMsg: " << pRspInfo->ErrorMsg << std::endl;

#if 0
	wxMutexLocker	lock(s_order_list_lock);
	for (int index = 0; index < //g_Frame->m_order_list.size(); index++) {
		order_struct& target = //g_Frame->m_order_list.at(index);
		if (!strcmp(pOrderAction->OrderRef, target.OrderRef1)) {
			//!strcmp(pInputOrder->ExchangeID, (*it).ExchangeID1 ) ) {

			target.type = OS_ST_NONE;
			//g_Frame->m_Loader->write_order(default_order_path, target);
			//g_Frame->updateGridsExt();

			break;
		}
	}
#endif
}

///��������������Ӧ
void CTrader::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (!pRspInfo) {
		return;
	}

	std::cerr << "--->>>" << __FUNCTION__ << std::endl;
	std::cerr << "InvestorID: " << pInputOrderAction->InvestorID << " InstrumentID: " << pInputOrderAction->InstrumentID << " OrderRef: " << pInputOrderAction->OrderRef << std::endl;
	std::cerr << "ErrorMsg: " << pRspInfo->ErrorMsg << std::endl;

#if 0

	// 25 = �Ҳ�������
	if (pRspInfo->ErrorID == 25) {
		string temp(pInputOrderAction->OrderRef);

		/*vector<order_struct>::iterator it = //g_Frame->m_order_list.begin();*/
		int count = 0;
		/*for (; it != //g_Frame->m_order_list.end(); it++) {*/


		wxMutexLocker	lock(s_order_list_lock);
		for (int index = 0; index < //g_Frame->m_order_list.size(); index++) {
			order_struct& target = //g_Frame->m_order_list.at(index);
			if (!strcmp(temp.c_str(), target.OrderRef1)) {
				//!strcmp(pTrade->ExchangeID, (*it).ExchangeID1 ) ) {

				count++;


				strcpy(target.OrderSysID1, pInputOrderAction->OrderSysID);


				string t2(os_st_table[target.type]);
				string t = "���󳷵�25����Ϊ��״̬��֮ǰ״̬�� {" + t2;
				t.append("}\n");
				//g_Frame->writeLog(t, LOG_ORDER);

				// �Ҳ�����Ϊ��״̬
				target.type = OS_ST_NONE;
				break;
			}
		}
	}

	// 26 = �ѳɽ����ѳ�������
	if (pRspInfo->ErrorID == 26) {
		string temp(pInputOrderAction->OrderRef);

		/*vector<order_struct>::iterator it = //g_Frame->m_order_list.begin();*/
		int count = 0;
		/*for (; it != //g_Frame->m_order_list.end(); it++) {*/


		wxMutexLocker	lock(s_order_list_lock);
		for (int index = 0; index < //g_Frame->m_order_list.size(); index++) {
			order_struct& target = //g_Frame->m_order_list.at(index);
			if (!strcmp(temp.c_str(), target.OrderRef1)) {
				//!strcmp(pTrade->ExchangeID, (*it).ExchangeID1 ) ) {

				count++;


				strcpy(target.OrderSysID1, pInputOrderAction->OrderSysID);

				int failTest = 1;
				if (failTest == 1) {
					if (target.type == OS_ST_INSERT_CANCELLED ||
						target.type == OS_ST_FINISH_CANCELLED) {
						string t2(os_st_table[target.type]);
						string t = "���󳷵�֮ǰ״̬�� {" + t2;
						t.append("}\n");
						//g_Frame->writeLog(t, LOG_ORDER);
						break;
					}

					//ȫ���ɽ����ܳ������Ϊ�ɽ�
					if (target.type == OS_ST_INSERT_CANCEL ||
						target.type == OS_ST_INSERT_CANCEL_ACK) {
						string t2(os_st_table[target.type]);
						string t = "���󳷵�֮ǰ״̬�� {" + t2;
						t.append("}\n");
						//g_Frame->writeLog(t, LOG_ORDER);

						target.type = OS_ST_CONFIRMED;
						//g_Frame->m_Loader->write_order(default_order_path, target);
						//g_Frame->updateGridsExt();
						break;
					}
					if (target.type == OS_ST_FINISH_CANCEL ||
						target.type == OS_ST_FINISH_CANCEL_ACK) {

						string t2(os_st_table[target.type]);
						string t = "���󳷵�֮ǰ״̬�� {" + t2;
						t.append("}\n");
						//g_Frame->writeLog(t, LOG_ORDER);

						target.type = OS_ST_FINISHED;
						//g_Frame->m_Loader->write_order(default_order_path, target);
						//g_Frame->updateGridsExt();
						break;
					}



					//if (target.type == OS_ST_INSERT_CANCEL) {
					//	target.type = OS_ST_INSERT_CANCEL_ACK;
					//	//g_Frame->m_Loader->write_order(default_order_path, target);
					//	//g_Frame->updateGridsExt();
					//	break;
					//}

					//if (target.type == OS_ST_FINISH_CANCEL) {
					//	target.type = OS_ST_FINISH_CANCEL_ACK;
					//	//g_Frame->m_Loader->write_order(default_order_path, target);
					//	//g_Frame->updateGridsExt();
					//	break;
					//}
				}


				string t2(os_st_table[target.type]);
				string t = "WARN:OnRspOrderAction invalid status: {" + t2;
				t.append("}\n");

				//wxTheApp->GetTopWindow()->CallAfter(&MyFrame::consoleOutput, t);

				//g_Frame->writeLog(t, LOG_ORDER);


				break;
			}
		}
	} 
	/// 25 �Ҳ������� resets order status to default
	else if (pRspInfo->ErrorID == 25) {
		string temp(pInputOrderAction->OrderRef);

		/*vector<order_struct>::iterator it = //g_Frame->m_order_list.begin();*/
		int count = 0;
		/*for (; it != //g_Frame->m_order_list.end(); it++) {*/


		wxMutexLocker	lock(s_order_list_lock);
		for (int index = 0; index < //g_Frame->m_order_list.size(); index++) {
			order_struct& target = //g_Frame->m_order_list.at(index);
			if (!strcmp(temp.c_str(), target.OrderRef1)) {
				//!strcmp(pTrade->ExchangeID, (*it).ExchangeID1 ) ) {

				count++;


				strcpy(target.OrderSysID1, pInputOrderAction->OrderSysID);

				int failTest = 1;
				if (failTest == 1) {
					if (target.type == OS_ST_INSERT_CANCELLED ||
						target.type == OS_ST_FINISH_CANCELLED) {
						string t2(os_st_table[target.type]);
						string t = "���󳷵�֮ǰ״̬�� {" + t2;
						t.append("}\n");
						//g_Frame->writeLog(t, LOG_ORDER);
						break;
					}

					//ȫ���ɽ����ܳ������Ϊ�ɽ�
					if (target.type == OS_ST_INSERT_CANCEL ||
						target.type == OS_ST_INSERT_CANCEL_ACK) {
						string t2(os_st_table[target.type]);
						string t = "���󳷵�֮ǰ״̬�� {" + t2;
						t.append("}\n");
						//g_Frame->writeLog(t, LOG_ORDER);

						target.type = OS_ST_NONE;
						//g_Frame->m_Loader->write_order(default_order_path, target);
						//g_Frame->updateGridsExt();
						break;
					}
					if (target.type == OS_ST_FINISH_CANCEL ||
						target.type == OS_ST_FINISH_CANCEL_ACK) {

						string t2(os_st_table[target.type]);
						string t = "���󳷵�֮ǰ״̬�� {" + t2;
						t.append("}\n");
						//g_Frame->writeLog(t, LOG_ORDER);

						target.type = OS_ST_NONE;
						//g_Frame->m_Loader->write_order(default_order_path, target);
						//g_Frame->updateGridsExt();
						break;
					}

				}


				string t2(os_st_table[target.type]);
				string t = "WARN:invalid order before confirmed: {" + t2;
				t.append("}\n");

				//wxTheApp->GetTopWindow()->CallAfter(&MyFrame::consoleOutput, t);

				//g_Frame->writeLog(t, LOG_ORDER);


				break;
			}
		}
	}
#endif
	
}

///�����ѯͶ���ֲ߳���Ӧ
void CTrader::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	//std::cerr << "--->>>" << __FUNCTION__ << std::endl;
	//std::cerr << "��ֲ֣�" << pInvestorPosition->TodayPosition << " ��ֲ֣�" << pInvestorPosition->YdPosition
	//	<< " ��Լ���룺" << pInvestorPosition->InstrumentID << " ռ�õı�֤��" << pInvestorPosition->UseMargin << std::endl;
	//g_Frame->RecvPositionInfo(pInvestorPosition, pRspInfo, nRequestID);
}

///�����ѯ������Ӧ
void CTrader::OnRspQryDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	std::cerr << "--->>>" << __FUNCTION__ << std::endl;
	std::cerr << "InstrumentID: " << pDepthMarketData->InstrumentID << " UpdateTime: " << pDepthMarketData->UpdateTime << std::endl;
}

///Ԥ��¼��������Ӧ
void CTrader::OnRspParkedOrderInsert(CThostFtdcParkedOrderField *pParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	//strcpy(InstrumentID, pParkedOrder->InstrumentID);
	//strcpy(ParkedOrderID, pParkedOrder->ParkedOrderID);
	std::cerr << "--->>>" << __FUNCTION__ << std::endl;
	std::cerr << "InvestorID: " << pParkedOrder->InvestorID << " InstrumentID: " << pParkedOrder->InstrumentID
		<< " Status: " << pParkedOrder->Status << " OrderRef: " << pParkedOrder->OrderRef << " ParkedOrderID: " << pParkedOrder->ParkedOrderID << std::endl;
	if (pRspInfo)
	{
		std::cerr << "ErrorMsg: " << pRspInfo->ErrorMsg << std::endl;
	}
}

///Ԥ�񳷵�����������Ӧ
void CTrader::OnRspParkedOrderAction(CThostFtdcParkedOrderActionField *pParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	std::cerr << "--->>>" << __FUNCTION__ << std::endl;
	std::cerr << "InvestorID: " << pParkedOrderAction->InvestorID << " InstrumentID: " << pParkedOrderAction->InstrumentID
		<< " Status: " << pParkedOrderAction->Status << " OrderRef: " << pParkedOrderAction->OrderRef << std::endl;
	if (pRspInfo)
	{
		std::cerr << "ErrorMsg: " << pRspInfo->ErrorMsg << std::endl;
	}
}

///ɾ��Ԥ����Ӧ
void CTrader::OnRspRemoveParkedOrder(CThostFtdcRemoveParkedOrderField *pRemoveParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	std::cerr << "--->>>" << __FUNCTION__ << std::endl;
	std::cerr << "BrokerID: " << pRemoveParkedOrder->BrokerID << " InvestorID: " << pRemoveParkedOrder->InvestorID
		<< " ParkedOrderID: " << pRemoveParkedOrder->ParkedOrderID << std::endl;
	if (pRspInfo)
	{
		std::cerr << "ErrorMsg: " << pRspInfo->ErrorMsg << std::endl;
	}
}

///ɾ��Ԥ�񳷵���Ӧ
void CTrader::OnRspRemoveParkedOrderAction(CThostFtdcRemoveParkedOrderActionField *pRemoveParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	std::cerr << "--->>>" << __FUNCTION__ << std::endl;
	if (pRspInfo)
	{
		std::cerr << "ErrorMsg: " << pRspInfo->ErrorMsg << std::endl;
	}
}

///�����ѯ������Ӧ
void CTrader::OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (pOrder == NULL) {
		int test = 1;
	}

	std::cerr << "--->>>" << __FUNCTION__ << std::endl;
	std::cerr << "IstrumentID: " << pOrder->InstrumentID << " OrderRef: " << pOrder->OrderRef << std::endl;
	if (pRspInfo)
	{
		std::cerr << "ErrorMsg: " << pRspInfo->ErrorMsg << std::endl;
	}

	//if (!strcmp(pOrder->OrderRef, //g_Frame->getFrameMA()->StratMA.orderRef1)) {
	//	//g_Frame->getFrameMA()->StratMA.status = OS_ST_CONFIRMED;
	//}

	//if (!strcmp(pOrder->OrderRef, //g_Frame->getFrameMA()->StratMA.orderRef2)) {
	//	//g_Frame->getFrameMA()->StratMA.status = OS_ST_FINISHING;
	//}
}

///�����ѯ�ɽ���Ӧ
void CTrader::OnRspQryTrade(CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (pTrade == NULL) {
		int test = 1;
		return;
	}

	std::cerr << "--->>>" << __FUNCTION__ << std::endl;
	std::cerr << "InstrumentID: " << pTrade->InstrumentID << " OrderRef: " << pTrade->OrderRef << std::endl;
	if (pRspInfo)
	{
		std::cerr << "ErrorMsg: " << pRspInfo->ErrorMsg << std::endl;
	}

	//if (!strcmp(pTrade->OrderRef, //g_Frame->getFrameMA()->StratMA.insertOrderRef1)) {
	//	//g_Frame->getFrameMA()->StratMA.status = OS_ST_CONFIRMED;
	//}

	//if (!strcmp(pTrade->OrderRef, //g_Frame->getFrameMA()->StratMA.finishOrderRef1)) {
	//	//g_Frame->getFrameMA()->StratMA.status = OS_ST_FINISHING;
	//}
}

///�����ѯ�ʽ��˻���Ӧ
void CTrader::OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	std::cerr << "--->>>" << __FUNCTION__ << std::endl;

	string test;
	//test << "AccountID: " << pTradingAccount->AccountID << " Available: " << pTradingAccount->Available
	//	<< " Commission: " << pTradingAccount->Commission << " CloseProfit: " << pTradingAccount->CloseProfit
	//	<< " PositionProfit: " << pTradingAccount->PositionProfit << std::endl;

	test.append("�˺�ID  ��");
	test.append(pTradingAccount->AccountID);
	test.append("\n");
	test.append("�����ʽ�");
	test.append(  to_string(pTradingAccount->Available) );
	test.append("\n");
	test.append("��֤�ʽ�");
	test.append( to_string(pTradingAccount->Commission) );
	test.append("\n");

	//wxTheApp->GetTopWindow()->CallAfter(&MyFrame::consoleOutput, test);
	if (pRspInfo)
	{
		std::cerr << "ErrorMsg: " << pRspInfo->ErrorMsg << std::endl;
	}
}

///�����ѯ��Լ����������Ӧ
void CTrader::OnRspQryInstrumentCommissionRate(CThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	;
	std::cerr << "--->>>" << __FUNCTION__ << std::endl;
	std::cerr << "InstrumentID " << pInstrumentCommissionRate->InstrumentID << std::endl;
	std::cerr << "CloseRatioByMoney " << pInstrumentCommissionRate->CloseRatioByMoney << " CloseRatioByVolume " << pInstrumentCommissionRate->CloseRatioByVolume << std::endl;
	std::cerr << " CloseTodayRatioByMoney " << pInstrumentCommissionRate->CloseTodayRatioByMoney << " CloseTodayRatioByVolume " << pInstrumentCommissionRate->CloseTodayRatioByVolume << std::endl;
	std::cerr << " OpenRatioByMoney " << pInstrumentCommissionRate->OpenRatioByMoney << " OpenRatioByVolume " << pInstrumentCommissionRate->OpenRatioByVolume << std::endl;
	if (pRspInfo)
	{
		std::cerr << "ErrorMsg: " << pRspInfo->ErrorMsg << std::endl;
	}
}
