#include "CCTPTrader.h"
#include "cctpshared.h"


#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new (_NORMAL_BLOCK , __FILE__ , __LINE__)
#define new DBG_NEW
#endif
#endif



//CCTPTrader::~CCTPTrader() {
//    //tradeApi->Join();
//    tradeApi->Release();
//    delete m_MsgList;
//    m_MsgList = NULL;
//}


///Inline utility function to convert result code into QString ## inline函数将CTP返回值转换为QString
inline QString resultString(int code) {
    // TODO: Change magic string into variable/external string

    QString sResult;
    switch (code) {
        case CCTPTrader::SUCCESS_POST:
            sResult = "登陆成功。\n";
            break;
        case CCTPTrader::FAILED_NETWORK:
            sResult = "登陆网络连接失败。\n";
            break;
        case CCTPTrader::FAILED_REACH_LIMITS:
            sResult = "登陆未处理请求超过许可数。\n";
            break;
        case CCTPTrader::FAILED_REACH_GRANTS:
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

int CCTPTrader::login() {
	CThostFtdcReqUserLoginField loginField;
	memset(&loginField, 0, sizeof(loginField));
	strcpy(loginField.BrokerID, brokerId.c_str());
	strcpy(loginField.UserID, userId.c_str());
	strcpy(loginField.Password, passwd.c_str());
	strcpy(loginField.UserProductInfo, "ashu");

	string sMsg("正在登陆。。。\n");
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


    int iResult = tradeApi->ReqUserLogin(&loginField, ++reqId);
    QString result_str = resultString(iResult);

	//g_Frame->writeLog(sResult, LOG_TRADE);
	//wxTheApp->GetTopWindow()->CallAfter(&MyFrame::consoleOutput, sResult);

    CEventQueue &eq = CEventQueue::getSingleton();


    //eq.pushEvent(evtPointer);

	return iResult;
}

int CCTPTrader::logout() {
	CThostFtdcUserLogoutField userLogout;
	memset(&userLogout, 0, sizeof(userLogout));
	strcpy(userLogout.BrokerID, brokerId.c_str());
	strcpy(userLogout.UserID, userId.c_str());

	string sMsg("正在注销。。。\n");
	//g_Frame->writeLog(sMsg, LOG_TRADE);
	//wxTheApp->GetTopWindow()->CallAfter(&MyFrame::consoleOutput, sMsg);;

    int iResult = tradeApi->ReqUserLogout(&userLogout, ++reqId);
    QString result_str = resultString(iResult);

	//g_Frame->writeLog(sResult, LOG_TRADE);
	//wxTheApp->GetTopWindow()->CallAfter(&MyFrame::consoleOutput, sResult);

	return iResult;
}

int CCTPTrader::queryPos() {
	CThostFtdcQryInvestorPositionField QryInvestorPosition;
	memset(&QryInvestorPosition, 0, sizeof(QryInvestorPosition));
	strcpy(QryInvestorPosition.BrokerID, brokerId.c_str());
	strcpy(QryInvestorPosition.InvestorID, userId.c_str());

	string sMsg("正在获取持仓。。。\n");
	//g_Frame->writeLog(sMsg, LOG_TRADE);
	//wxTheApp->GetTopWindow()->CallAfter(&MyFrame::consoleOutput, sMsg);


    int iResult = tradeApi->ReqQryInvestorPosition(&QryInvestorPosition, ++reqId);
    QString result_str = resultString(iResult);

    //g_Frame->writeLog(sResult, LOG_TRADE);
	//wxTheApp->GetTopWindow()->CallAfter(&MyFrame::consoleOutput, sResult);


	return iResult;
}

///TODO: ENG:add queryHold ## 中： 加入queryHold实体
int CCTPTrader::queryHold() {
    return -1;
}

int  CCTPTrader::insert(string instrumentID, int price, int volume, int code, int reqID)
{
	CThostFtdcInputOrderField inputOrder; //插入报单数据
	bool bInsert = false;

	memset(&inputOrder, 0, sizeof(inputOrder));
	//brokerid
	strcpy(inputOrder.BrokerID, brokerId.c_str());
	//投资者代码
	strcpy(inputOrder.InvestorID, userId.c_str());
	strcpy(inputOrder.UserID, userId.c_str());

	strcpy(inputOrder.InstrumentID, instrumentID.c_str());

	inputOrder.MinVolume = 0;
	inputOrder.StopPrice = 0;
	inputOrder.IsAutoSuspend = 0;

	//价格！
	inputOrder.LimitPrice = price;

	inputOrder.VolumeTotalOriginal = volume;
	//有效期类型 --当日有效
	inputOrder.TimeCondition = THOST_FTDC_TC_GFD;

	strcpy(inputOrder.GTDDate, "");
	//组合投机套保标志
	inputOrder.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation; 
	//inputOrder.CombHedgeFlag[0] = THOST_FTDC_ECIDT_Hedge;
	//成交量类型 ///任何数量 THOST_FTDC_VC_AV '1' ///最小数量 THOST_FTDC_VC_MV '2' ///全部数量 THOST_FTDC_VC_CV '3'
	inputOrder.VolumeCondition = THOST_FTDC_VC_AV;

	//报价类型
	inputOrder.OrderPriceType = THOST_FTDC_OPT_LimitPrice; //限价单标识

														   //立即单
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
	//	ORDER_OPEN_BUY = 1, ///<开多
	//	ORDER_OPEN_SELL = 2, ///<开空 
	//	ORDER_CLOSE_BUY = 3, ///<平多
	//	ORDER_CLOSE_SELL = 4, ///<平空
	//	ORDER_FOK = 5, ///<立即成交，否则撤销
	//	ORDER_FAK = 6, ///<立即成交，否则撤销
	//	ORDER_CONDITION = 7, ///<条件保留
	//	ORDER_ANYPRICE = 20, ///<市价单
	//	ORDER_CONDITION_TOUCH = 30, ///<止损
	//	ORDER_CONDITION_TOUCHPROFIT = 31, ///<止盈
	//};
	string code_type = "符号： ";
	switch (code) {
	case ORDER_OPEN_BUY:
		inputOrder.Direction = THOST_FTDC_D_Buy;
		inputOrder.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
		code_type.append("开多");
		break;
	case ORDER_OPEN_SELL:
		inputOrder.Direction = THOST_FTDC_D_Sell;
		inputOrder.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
		code_type.append("开空");
		break;
	case ORDER_CLOSE_BUY:
		inputOrder.Direction = THOST_FTDC_D_Sell;
		inputOrder.CombOffsetFlag[0] = THOST_FTDC_OF_Close;
		code_type.append("平多");
		break;
	case ORDER_CLOSE_SELL:
		inputOrder.Direction = THOST_FTDC_D_Buy;
		inputOrder.CombOffsetFlag[0] = THOST_FTDC_OF_Close;
		code_type.append("平空");
		break;
	case ORDER_CLOSE_BUY_TODAY:
		inputOrder.Direction = THOST_FTDC_D_Sell;
		inputOrder.CombOffsetFlag[0] = THOST_FTDC_OF_CloseToday;
		code_type.append("平今多");
		break;
	case ORDER_CLOSE_SELL_TODAY:
		inputOrder.Direction = THOST_FTDC_D_Buy;
		inputOrder.CombOffsetFlag[0] = THOST_FTDC_OF_CloseToday;
		code_type.append("平今空");
		break;
	case ORDER_CLOSE_BUY_YEST:
		inputOrder.Direction = THOST_FTDC_D_Sell;
		inputOrder.CombOffsetFlag[0] = THOST_FTDC_OF_CloseYesterday;
		code_type.append("平昨多");
		break;
	case ORDER_CLOSE_SELL_YEST:
		inputOrder.Direction = THOST_FTDC_D_Buy;
		inputOrder.CombOffsetFlag[0] = THOST_FTDC_OF_CloseYesterday;
		code_type.append("平昨空");
		break;
	case ORDER_FOK:
		inputOrder.TimeCondition = THOST_FTDC_TC_IOC; //立即成交，否则撤销
		inputOrder.VolumeCondition = THOST_FTDC_VC_AV; //~MV都可以
		inputOrder.ContingentCondition = THOST_FTDC_CC_Immediately;
		code_type.append("FOK");
		break;
	case ORDER_FAK:
		inputOrder.TimeCondition = THOST_FTDC_TC_IOC; //立即成交，否则撤销
		inputOrder.VolumeCondition = THOST_FTDC_VC_CV; //全部数量
		inputOrder.ContingentCondition = THOST_FTDC_CC_Immediately;
		code_type.append("FAK");
		break;
	case ORDER_CONDITION:
		inputOrder.ContingentCondition = THOST_FTDC_CC_LastPriceGreaterThanStopPrice; //最新价大于条件价
		code_type.append("错误条件单");
		break;
	case ORDER_ANYPRICE:
		//市价单
		inputOrder.OrderPriceType = THOST_FTDC_OPT_AnyPrice;
		inputOrder.LimitPrice = 0;
		inputOrder.TimeCondition = THOST_FTDC_TC_IOC;
		inputOrder.VolumeCondition = THOST_FTDC_VC_AV;
		inputOrder.ContingentCondition = THOST_FTDC_CC_Immediately;
		code_type.append("市价单");
		break;
	case ORDER_CONDITION_TOUCH:
		inputOrder.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
		inputOrder.TimeCondition = THOST_FTDC_TC_GFD;
		inputOrder.VolumeCondition = THOST_FTDC_VC_AV;
		inputOrder.ContingentCondition = THOST_FTDC_CC_Touch;
		code_type.append("止损");
		break;
	case ORDER_CONDITION_TOUCHPROFIT:
		inputOrder.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
		inputOrder.TimeCondition = THOST_FTDC_TC_GFD;
		inputOrder.VolumeCondition = THOST_FTDC_VC_AV;
		inputOrder.ContingentCondition = THOST_FTDC_CC_TouchProfit;
		code_type.append("止盈");
		break;
	default:
		break;
	}


	string sMsg("正在下单：");
	sMsg.append(instrumentID);
	sMsg.append("\n");
	//g_Frame->writeLog(sMsg, LOG_TRADE);
	//wxTheApp->GetTopWindow()->CallAfter(&MyFrame::consoleOutput, sMsg);

	//g_Frame->writeLog(code_type, LOG_TRADE);
	//wxTheApp->GetTopWindow()->CallAfter(&MyFrame::consoleOutput, code_type);

	int iResult = tradeApi->ReqOrderInsert(&inputOrder, reqID);
    QString result_str = resultString(iResult);

	//g_Frame->writeLog(sResult, LOG_TRADE);
	//wxTheApp->GetTopWindow()->CallAfter(&MyFrame::consoleOutput, sResult);

	return iResult;
}

int  CCTPTrader::insert2(string instrumentID, int price, int volume, int code, int reqID, int FrontID, int sessionID, string orderRef)
{
	CThostFtdcInputOrderField inputOrder; //插入报单数据
	bool bInsert = false;

	memset(&inputOrder, 0, sizeof(inputOrder));
	//brokerid
	strcpy(inputOrder.BrokerID, brokerId.c_str());
	//投资者代码
	strcpy(inputOrder.InvestorID, userId.c_str());
	strcpy(inputOrder.UserID, userId.c_str());

	strcpy(inputOrder.InstrumentID, instrumentID.c_str());

	//inputOrder.FrontID = FrontID;
	//inputOrder.SessionID = sessionID;

	strcpy(inputOrder.OrderRef, orderRef.c_str());

	strcpy(inputOrder.ExchangeID, instrumentID.c_str());
	inputOrder.RequestID = reqID;

	//价格！
	inputOrder.LimitPrice = price;

	inputOrder.VolumeTotalOriginal = volume;
	//有效期类型 --当日有效
	inputOrder.TimeCondition = THOST_FTDC_TC_GFD;
	//组合投机套保标志
	inputOrder.CombHedgeFlag[0] = THOST_FTDC_ECIDT_Hedge;
	//成交量类型 ///任何数量 THOST_FTDC_VC_AV '1' ///最小数量 THOST_FTDC_VC_MV '2' ///全部数量 THOST_FTDC_VC_CV '3'
	inputOrder.VolumeCondition = THOST_FTDC_VC_AV;

	//报价类型
	inputOrder.OrderPriceType = THOST_FTDC_OPT_LimitPrice; //限价单标识

	inputOrder.ContingentCondition = THOST_FTDC_CC_Immediately; //立即单
	inputOrder.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
	inputOrder.UserForceClose = 0;


	string code_type = "符号： ";
	switch (code) {
	case ORDER_CLOSE_BUY:
		inputOrder.Direction = THOST_FTDC_D_Buy;
		inputOrder.CombOffsetFlag[0] = THOST_FTDC_OF_Close;
		code_type.append("平多");
		break;
	case ORDER_CLOSE_SELL:
		inputOrder.Direction = THOST_FTDC_D_Sell;
		inputOrder.CombOffsetFlag[0] = THOST_FTDC_OF_Close;
		code_type.append("平空");
		break;
	case ORDER_CLOSE_BUY_TODAY:
		inputOrder.Direction = THOST_FTDC_D_Buy;
		inputOrder.CombOffsetFlag[0] = THOST_FTDC_OF_CloseToday;
		code_type.append("平今多");
		break;
	case ORDER_CLOSE_SELL_TODAY:
		inputOrder.Direction = THOST_FTDC_D_Sell;
		inputOrder.CombOffsetFlag[0] = THOST_FTDC_OF_CloseToday;
		code_type.append("平今空");
		break;
	case ORDER_CLOSE_BUY_YEST:
		inputOrder.Direction = THOST_FTDC_D_Buy;
		inputOrder.CombOffsetFlag[0] = THOST_FTDC_OF_CloseYesterday;
		code_type.append("平昨多");
		break;
	case ORDER_CLOSE_SELL_YEST:
		inputOrder.Direction = THOST_FTDC_D_Sell;
		inputOrder.CombOffsetFlag[0] = THOST_FTDC_OF_CloseYesterday;
		code_type.append("平昨空");
		break;
	default:
		break;
	}

	string sMsg("正在平仓：");
	sMsg.append(instrumentID);
	sMsg.append("\n");
	//g_Frame->writeLog(sMsg, LOG_TRADE);
	//wxTheApp->GetTopWindow()->CallAfter(&MyFrame::consoleOutput, sMsg);

	//g_Frame->writeLog(code_type, LOG_TRADE);
	//wxTheApp->GetTopWindow()->CallAfter(&MyFrame::consoleOutput, code_type);

	int iResult = tradeApi->ReqOrderInsert(&inputOrder, reqID);
    QString result_str = resultString(iResult);

	//g_Frame->writeLog(sResult, LOG_TRADE);
	//wxTheApp->GetTopWindow()->CallAfter(&MyFrame::consoleOutput, sResult);

	return iResult;
}

///
int  CCTPTrader::insertAction(string instrumentID, int price, int volume, int code, int reqID, int FrontID, int sessionID, string orderRef)
{
	CThostFtdcInputOrderActionField inputOrder; //插入报单数据
	bool bInsert = false;

	memset(&inputOrder, 0, sizeof(inputOrder));
	//brokerid
	strcpy(inputOrder.BrokerID, brokerId.c_str());
	//投资者代码
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



	string sMsg("正在平仓：");
	sMsg.append(instrumentID);
	sMsg.append("\n");
	//g_Frame->writeLog(sMsg, LOG_TRADE);
	//wxTheApp->GetTopWindow()->CallAfter(&MyFrame::consoleOutput, sMsg);

	int iResult = tradeApi->ReqOrderAction(&inputOrder, reqID);
    QString result_str = resultString(iResult);

	//g_Frame->writeLog(sResult, LOG_TRADE);
	//wxTheApp->GetTopWindow()->CallAfter(&MyFrame::consoleOutput, sResult);

	return iResult;
}

///撤单
int CCTPTrader::orderAction(int FrontID, int SessionID, char* OrderRef, string InstrumentID)
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
	strcpy(InputOrderAction.InstrumentID, InstrumentID.c_str());  //这个是必须的
	int iResult = tradeApi->ReqOrderAction(&InputOrderAction, ++reqId);

	return iResult;
}

int CCTPTrader::orderAction2(char * orderSysID, string instrumentID)
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

int CCTPTrader::queryOrderAction(string instrumentID, TThostFtdcOrderRefType orderRef, int reqID)
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
	strcpy(InputOrderAction.InstrumentID, instrumentID.c_str());  //这个是必须的

	int iResult;
	iResult = tradeApi->ReqOrderAction(&InputOrderAction, reqID);
	return iResult;
}

int CCTPTrader::queryTrade(string instrumentID, int reqID)
{
	CThostFtdcQryTradeField QryTrade;
	memset(&QryTrade, 0, sizeof(QryTrade));
	strcpy(QryTrade.BrokerID, brokerId.c_str());
	strcpy(QryTrade.InvestorID, userId.c_str());

	int iResult;
	iResult = tradeApi->ReqQryTrade(&QryTrade, reqID);
	return iResult;
}

int CCTPTrader::queryOrder(string instrumentID, int reqID)
{
	CThostFtdcQryOrderField QryOrder;
	memset(&QryOrder, 0, sizeof(QryOrder));
	strcpy(QryOrder.BrokerID, brokerId.c_str());
	strcpy(QryOrder.InvestorID, userId.c_str());

	int iResult;
	iResult = tradeApi->ReqQryOrder(&QryOrder, reqID);
	return iResult;
}

int  CCTPTrader::queryDepthData(string instrumentID)
{
	char _instrumentId[10];
	CThostFtdcQryDepthMarketDataField QryDepthMarketData;
	memset(&QryDepthMarketData, 0, sizeof(QryDepthMarketData));
	strcpy(QryDepthMarketData.InstrumentID, instrumentID.c_str());
	int iResult = tradeApi->ReqQryDepthMarketData(&QryDepthMarketData, ++reqId);

	string sMsg("正在获取行情：");
	sMsg.append(instrumentID);
	sMsg.append("\n");
	//g_Frame->writeLog(sMsg, LOG_TRADE);
	//wxTheApp->GetTopWindow()->CallAfter(&MyFrame::consoleOutput, sMsg);

    QString result_str = resultString(iResult);

	//g_Frame->writeLog(sResult, LOG_TRADE);
	//wxTheApp->GetTopWindow()->CallAfter(&MyFrame::consoleOutput, sResult);

	return iResult;
}

int CCTPTrader::qryTradingAccount() {
	string sMsg("正在获取行情：");

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

void CCTPTrader::tick(void) {
	int iResult = login();

	
}

///结算结果确认
void CCTPTrader::settleConfirm(void)
{
	CThostFtdcSettlementInfoConfirmField SettlementInfoConfirm;
	strcpy(SettlementInfoConfirm.BrokerID, brokerId.c_str());
	strcpy(SettlementInfoConfirm.InvestorID, userId.c_str());
	int iResult = tradeApi->ReqSettlementInfoConfirm(&SettlementInfoConfirm, ++reqId);
}

void CCTPTrader::OnFrontConnected()
{
	//g_Frame->consoleOutput("交易服务器已连接。\n");
}

void CCTPTrader::OnFrontDisconnected(int nReason)
{
	std::cerr << "--->>> " << __FUNCTION__ << std::endl;
	std::cerr << "--->>> " << "reason: " << nReason << std::endl;

	//g_Frame->m_Loader->removeCon();
	//g_Frame->Reconnect();
}

void CCTPTrader::OnHeartBeatWarning(int nTimeLapse)
{
	std::cerr << "--->>> " << __FUNCTION__ << std::endl;
	std::cerr << "--->>> " << nTimeLapse << std::endl;
}

void CCTPTrader::OnRspAuthenticate(CThostFtdcRspAuthenticateField *pRspAuthenticateField, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	std::cerr << "--->>> " << __FUNCTION__ << std::endl;
	std::cerr << "brokerID: " << pRspAuthenticateField->BrokerID << " UserID: " << pRspAuthenticateField->UserID << " ProductInfo: " << pRspAuthenticateField->UserProductInfo << std::endl;
	std::cerr << "RspInfo: " << pRspInfo->ErrorMsg << std::endl;
}

void CCTPTrader::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
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

void CCTPTrader::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	std::cerr << "--->>> " << __FUNCTION__ << std::endl;
	std::cerr << "brokerID: " << pUserLogout->BrokerID << " UserID: " << pUserLogout->UserID << std::endl;
	std::cerr << "RspInfo: " << pRspInfo->ErrorMsg << std::endl;
}

void CCTPTrader::OnRspUserPasswordUpdate(CThostFtdcUserPasswordUpdateField *pUserPasswordUpdate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	std::cerr << "--->>> " << __FUNCTION__ << std::endl;
	std::cerr << "brokerID: " << pUserPasswordUpdate->BrokerID << " UserID: " << pUserPasswordUpdate->UserID
		<< " oldPasswd: " << pUserPasswordUpdate->OldPassword << " newPasswd: " << pUserPasswordUpdate->NewPassword << std::endl;
	std::cerr << "--->>> " << "RspInfo: " << pRspInfo->ErrorMsg << std::endl;
}

///错误应答
void CCTPTrader::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	std::cerr << "--->>> " << __FUNCTION__ << std::endl;
	std::cerr << "ErrorID: " << pRspInfo->ErrorID << " ErrorMsg: " << pRspInfo->ErrorMsg
		<< std::endl;
}

///报单录入请求响应
void CCTPTrader::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (pRspInfo) {
		//wxTheApp->GetTopWindow()->CallAfter(&MyFrame::consoleOutput, pRspInfo->ErrorMsg);
	}

	std::cerr << "--->>> " << __FUNCTION__ << std::endl;
	std::cerr << "brokerID: " << pInputOrder->BrokerID << " UserID: " << pInputOrder->UserID
		<< " instrumentId: " << pInputOrder->InstrumentID << " 买卖方向: " << (pInputOrder->Direction == '0' ? "买" : "卖")
		<< " 组合开平标志: " << (pInputOrder->CombOffsetFlag[0] == '0' ? "开仓" : "平仓") << std::endl;
	std::cerr << "--->>> " << "RspInfo: " << pRspInfo->ErrorMsg << std::endl;

	string temp(pInputOrder->OrderRef);

	//vector<order_struct>::iterator it = //g_Frame->m_order_list.begin();
    //for (; it != //g_Frame->m_order_list.end(); it++) {
}

///投资者结算结果确认响应
void CCTPTrader::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	std::cerr << "--->>> " << __FUNCTION__ << std::endl;
	std::cerr << "brokerID: " << pSettlementInfoConfirm->BrokerID << " UserID: " << pSettlementInfoConfirm->InvestorID
		<< std::endl;
	std::cerr << "RspInfo: " << pRspInfo->ErrorMsg << std::endl;
}

///报单通知
void CCTPTrader::OnRtnOrder(CThostFtdcOrderField *pOrder)
{
	//strcpy(InstrumentID, pOrder->InstrumentID);
	//strcpy(OrderRef, pOrder->OrderRef);
	//strcpy(ExchangeID, pOrder->ExchangeID);
	//strcpy(OrderSysID, pOrder->OrderSysID);

	std::cerr << "--->>>" << __FUNCTION__ << std::endl;
	std::cerr << "InvestorID: " << pOrder->InvestorID << " InstrumentID: " << pOrder->InstrumentID << " OrderStatus: " << pOrder->OrderStatus << " OrderRef" << pOrder->OrderRef
		<< "OrderStatus" << pOrder->StatusMsg << std::endl;

	string temp(pOrder->OrderRef);


}

///成交通知
void CCTPTrader::OnRtnTrade(CThostFtdcTradeField *pTrade)
{
	std::cerr << "--->>>" << __FUNCTION__ << std::endl;
	std::cerr << "InvestorID: " << pTrade->InvestorID << " InstrumentID: " << pTrade->InstrumentID << " OrderSysID: " << pTrade->OrderSysID
		  << std::endl;

	string temp(pTrade->OrderRef);

}

///报单录入错误回报
void CCTPTrader::OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo)
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

///报单操作错误回报
void CCTPTrader::OnErrRtnOrderAction(CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo)
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

///报单操作请求响应
void CCTPTrader::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (!pRspInfo) {
		return;
	}

	std::cerr << "--->>>" << __FUNCTION__ << std::endl;
	std::cerr << "InvestorID: " << pInputOrderAction->InvestorID << " InstrumentID: " << pInputOrderAction->InstrumentID << " OrderRef: " << pInputOrderAction->OrderRef << std::endl;
	std::cerr << "ErrorMsg: " << pRspInfo->ErrorMsg << std::endl;

#if 0

	// 25 = 找不到撤单
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
				string t = "错误撤单25（改为无状态）之前状态： {" + t2;
				t.append("}\n");
				//g_Frame->writeLog(t, LOG_ORDER);

				// 找不到改为无状态
				target.type = OS_ST_NONE;
				break;
			}
		}
	}

	// 26 = 已成交或已撤销。。
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
						string t = "错误撤单之前状态： {" + t2;
						t.append("}\n");
						//g_Frame->writeLog(t, LOG_ORDER);
						break;
					}

					//全部成交不能撤，则改为成交
					if (target.type == OS_ST_INSERT_CANCEL ||
						target.type == OS_ST_INSERT_CANCEL_ACK) {
						string t2(os_st_table[target.type]);
						string t = "错误撤单之前状态： {" + t2;
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
						string t = "错误撤单之前状态： {" + t2;
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
	/// 25 找不到撤单 resets order status to default
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
						string t = "错误撤单之前状态： {" + t2;
						t.append("}\n");
						//g_Frame->writeLog(t, LOG_ORDER);
						break;
					}

					//全部成交不能撤，则改为成交
					if (target.type == OS_ST_INSERT_CANCEL ||
						target.type == OS_ST_INSERT_CANCEL_ACK) {
						string t2(os_st_table[target.type]);
						string t = "错误撤单之前状态： {" + t2;
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
						string t = "错误撤单之前状态： {" + t2;
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

///请求查询投资者持仓响应
void CCTPTrader::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	//std::cerr << "--->>>" << __FUNCTION__ << std::endl;
	//std::cerr << "今持仓：" << pInvestorPosition->TodayPosition << " 昨持仓：" << pInvestorPosition->YdPosition
	//	<< " 合约代码：" << pInvestorPosition->InstrumentID << " 占用的保证金：" << pInvestorPosition->UseMargin << std::endl;
	//g_Frame->RecvPositionInfo(pInvestorPosition, pRspInfo, nRequestID);
}

///请求查询行情响应
void CCTPTrader::OnRspQryDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	std::cerr << "--->>>" << __FUNCTION__ << std::endl;
	std::cerr << "InstrumentID: " << pDepthMarketData->InstrumentID << " UpdateTime: " << pDepthMarketData->UpdateTime << std::endl;
}

///预埋单录入请求响应
void CCTPTrader::OnRspParkedOrderInsert(CThostFtdcParkedOrderField *pParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
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

///预埋撤单操作请求响应
void CCTPTrader::OnRspParkedOrderAction(CThostFtdcParkedOrderActionField *pParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	std::cerr << "--->>>" << __FUNCTION__ << std::endl;
	std::cerr << "InvestorID: " << pParkedOrderAction->InvestorID << " InstrumentID: " << pParkedOrderAction->InstrumentID
		<< " Status: " << pParkedOrderAction->Status << " OrderRef: " << pParkedOrderAction->OrderRef << std::endl;
	if (pRspInfo)
	{
		std::cerr << "ErrorMsg: " << pRspInfo->ErrorMsg << std::endl;
	}
}

///删除预埋单响应
void CCTPTrader::OnRspRemoveParkedOrder(CThostFtdcRemoveParkedOrderField *pRemoveParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	std::cerr << "--->>>" << __FUNCTION__ << std::endl;
	std::cerr << "BrokerID: " << pRemoveParkedOrder->BrokerID << " InvestorID: " << pRemoveParkedOrder->InvestorID
		<< " ParkedOrderID: " << pRemoveParkedOrder->ParkedOrderID << std::endl;
	if (pRspInfo)
	{
		std::cerr << "ErrorMsg: " << pRspInfo->ErrorMsg << std::endl;
	}
}

///删除预埋撤单响应
void CCTPTrader::OnRspRemoveParkedOrderAction(CThostFtdcRemoveParkedOrderActionField *pRemoveParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	std::cerr << "--->>>" << __FUNCTION__ << std::endl;
	if (pRspInfo)
	{
		std::cerr << "ErrorMsg: " << pRspInfo->ErrorMsg << std::endl;
	}
}

///请求查询报单响应
void CCTPTrader::OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
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

///请求查询成交响应
void CCTPTrader::OnRspQryTrade(CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
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

///请求查询资金账户响应
void CCTPTrader::OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	std::cerr << "--->>>" << __FUNCTION__ << std::endl;

	string test;
	//test << "AccountID: " << pTradingAccount->AccountID << " Available: " << pTradingAccount->Available
	//	<< " Commission: " << pTradingAccount->Commission << " CloseProfit: " << pTradingAccount->CloseProfit
	//	<< " PositionProfit: " << pTradingAccount->PositionProfit << std::endl;

	test.append("账号ID  ：");
	test.append(pTradingAccount->AccountID);
	test.append("\n");
	test.append("可用资金：");
	test.append(  to_string(pTradingAccount->Available) );
	test.append("\n");
	test.append("保证资金：");
	test.append( to_string(pTradingAccount->Commission) );
	test.append("\n");

	//wxTheApp->GetTopWindow()->CallAfter(&MyFrame::consoleOutput, test);
	if (pRspInfo)
	{
		std::cerr << "ErrorMsg: " << pRspInfo->ErrorMsg << std::endl;
	}
}

///请求查询合约手续费率响应
void CCTPTrader::OnRspQryInstrumentCommissionRate(CThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
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
