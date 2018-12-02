#pragma once
#ifndef _CQUOTER_H_
#define _CQUOTER_H_

#include <iostream>
#include <string>
#include <windows.h>
#include <stack>  
#include <vector>  
#include <deque> 
#include <list>
#include "cfgutil.h"
#include "CTraderRsp.h"
#include "CTraderMD.h"
#include "STDLL.h"
#include "swbase.h"

#include "qobject.h"

using namespace std;

//HANDLE  g_hEvent;
//
/////撤单组合一
/////报单引用
//TThostFtdcOrderRefType	OrderRef;
/////会话编号
//TThostFtdcSessionIDType	SessionID;
/////前置编号
//TThostFtdcFrontIDType	FrontID;
//
/////撤单组合二
/////交易所代码
//TThostFtdcExchangeIDType	ExchangeID;
/////报单编号
//TThostFtdcOrderSysIDType	OrderSysID;
/////合约代码
//TThostFtdcInstrumentIDType	InstrumentID;
//
////用于删除预埋单
/////预埋编号
//TThostFtdcParkedOrderIDType	ParkedOrderID;

class CQuoter : public CThostFtdcMdSpi {
public:
	enum {
		SUCCESS_POST = 0, ///<成功
		FAILED_NETWORK = -1, ///<网络连接失败
		FAILED_REACH_LIMITS = -2, ///<未处理请求超过许可数
		FAILED_REACH_GRANTS = -3 ///<每秒发送请求超过许可数
	};

	enum MSG_TYPE {
		MSG_TYPE_LOGIN = 0,
		MSG_TYPE_LOGOUT,
		MSG_TYPE_QRY_POSTION,
		MSG_TYPE_QRY_HOLD,
	};


	CQuoter()
	{
		reqId = 0;
		getCfg = CfgUtil::getInstance("rsh.cfg");
		brokerId = getCfg->getPara("BrokerId");
		userId = getCfg->getPara("BrokerUser");
		passwd = getCfg->getPara("BrokerUserPasswd");
		/*tradeIp = getCfg->getPara("TradeFrontIp");*/
		mdIp = getCfg->getPara("MarketFrontIp");

		queryApi = CThostFtdcMdApi::CreateFtdcMdApi("./thostmduserapi.dll");
		queryApi->RegisterFront(const_cast<char*>(mdIp.c_str()));
		queryApi->RegisterSpi((CThostFtdcMdSpi*)this);
		queryApi->Init();
		//queryApi->Join();

	}

	~CQuoter()
	{
		//tradeApi->Join();
		queryApi->Release();

	}

	MSG_LIST getMsgList(void) { return m_MsgList; };

        void clearMsgList(void) { m_MsgList.clear(); };

        void genMsg(CQuoter::MSG_TYPE type, string &msg);
        void emitMsg(MSG_LIST_S &msg);

        int login(void);
        int logout(void);
        int qrySubMarket(char **Instruments, int n);
        int queryPos(void);

        void tick(void);

        string getBrokerID() { return brokerId; };
        string getUserID() { return userId; };
        string getPWD() { return passwd; };
        /*string getTradeIP() { return tradeIp; };*/
        string getMDIP() { return mdIp; };
        int getRequestID() { return reqId; };


	///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
        void OnFrontConnected();

	///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
	///@param nReason 错误原因
	///        0x1001 网络读失败
	///        0x1002 网络写失败
	///        0x2001 接收心跳超时
	///        0x2002 发送心跳失败
	///        0x2003 收到错误报文
        void OnFrontDisconnected(int nReason);

	///心跳超时警告。当长时间未收到报文时，该方法被调用。
	///@param nTimeLapse 距离上次接收报文的时间
        void OnHeartBeatWarning(int nTimeLapse);


	///登录请求响应
        void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///登出请求响应
        void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///错误应答
        void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///订阅行情应答
        void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///取消订阅行情应答
        void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///订阅询价应答
        void OnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///取消订阅询价应答
        void OnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///深度行情通知
        void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);

	///询价通知
        void OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp);
protected:

private:




	enum function
	{
		EXIT = -1,
		LOGIN = 1,
		LOGOUT,
		UPDATE_PASSWD,
		SETTLE_CONFIRM,
		ORDERINSERT,
		ORDERACTION,
		QRYPOSITION,
		QRYDEPTHMARKETDATA,
		PARKED_ORDER,
		PARKED_ORDER_ACTION,
		QRYORDER,
		QRYTRADE,
		QRY_TRADING_ACCOUNT,
		QRY_INSTRUMENT_RATIO,
		QRY_SUB_MARKET
	};

	CfgUtil* getCfg;

	CThostFtdcMdApi *queryApi;
	string brokerId;
	string userId;
	string passwd;
	/*string tradeIp;*/
	string mdIp;

	//请求编号
	int reqId;

	MSG_LIST m_MsgList;
};

//extern CQuoter *g_Quoter;

#endif
