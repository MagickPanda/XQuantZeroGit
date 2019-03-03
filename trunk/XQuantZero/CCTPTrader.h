<<<<<<< HEAD:trunk/XQuantZero/CCTPTrader.h
﻿#pragma once
=======
#pragma once
>>>>>>> ab98e7dd9ed769e848a163473480d9a715d19ba7:trunk/XQuantZero/CCTPTrader.h
#ifndef _CCTPTrader_H_
#define _CCTPTrader_H_

#include <iostream>
#include <string>
#include <windows.h>
#include <stack>  
#include <vector>  
#include <deque> 
#include <list>
#include "cfgutil.h"
#include "CCTPQuery.h"
#include "CCTPTraderRsp.h"
#include "CCTPTraderMD.h"
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


<<<<<<< HEAD:trunk/XQuantZero/CCTPTrader.h
///TODO: 加入CurOrderRef锁，避免冲突
=======
///TODO: ����CurOrderRef���������ͻ
>>>>>>> ab98e7dd9ed769e848a163473480d9a715d19ba7:trunk/XQuantZero/CCTPTrader.h
class CCTPTrader : public CThostFtdcTraderSpi {
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
		MSG_TYPE_INSERT,
		MSG_LOG_ORDER_WITHDRAW,
	};

	enum {
		ORDER_REVERSE = 0,
		ORDER_OPEN_BUY = 1, ///<开多
		ORDER_OPEN_SELL = 2, ///<开空 
		ORDER_CLOSE_BUY = 3, ///<平多
		ORDER_CLOSE_SELL = 4, ///<平空
		ORDER_FOK = 5, ///<立即成交，否则撤销
		ORDER_FAK = 6, ///<立即成交，否则撤销
		ORDER_CONDITION = 7, ///<条件保留
		ORDER_ANYPRICE = 20, ///<市价单
		ORDER_CONDITION_TOUCH = 30, ///<止损
		ORDER_CONDITION_TOUCHPROFIT = 31, ///<止盈
		ORDER_CLOSE_BUY_TODAY,
		ORDER_CLOSE_SELL_TODAY,
		ORDER_CLOSE_BUY_YEST,
		ORDER_CLOSE_SELL_YEST
	};

    CCTPTrader()
	{
		m_MsgList = debug_new MSG_LIST();

		reqId = 0;
		getCfg = CfgUtil::getInstance("rsh.cfg");
		brokerId = getCfg->getPara("BrokerId");
		userId = getCfg->getPara("BrokerUser");
		passwd = getCfg->getPara("BrokerUserPasswd");
		tradeIp = getCfg->getPara("TradeFrontIp");
		/*mdIp = getCfg->getPara("MarketFrontIp");*/



        tradeApi = CThostFtdcTraderApi::CreateFtdcTraderApi("./thosttraderapi.dll");
		tradeApi->RegisterFront(const_cast<char*>(tradeIp.c_str()));
        tradeApi->RegisterSpi((CThostFtdcTraderSpi*)this);
		tradeApi->SubscribePrivateTopic(THOST_TERT_RESUME);
		tradeApi->SubscribePublicTopic(THOST_TERT_RESUME);
		tradeApi->Init();
		//tradeApi->Join();

        //CCTPTraderRsp *rsp = new CCTPTraderRsp;
		//tradeApi->RegisterSpi(rsp);
		CurOrderRefInt = 1;
		snprintf(CurOrderRef, 13, "%.12d", CurOrderRefInt);
		snprintf(LastOrderRef, 13, "%.12d", CurOrderRefInt);
		/*strcpy(LastOrderRef, "           1");*/
	}

//    ~CCTPTrader();

	void failIncRef() {
		CurOrderRefInt++;
		snprintf(CurOrderRef, 13, "%.12d", CurOrderRefInt);
		snprintf(LastOrderRef, 13, "%.12d", CurOrderRefInt);
	}

        /*MSG_LIST* getMsgList(void) { return m_MsgList; };

	void clearMsgList(void) { m_MsgList->clear(); };

    void genMsg(CCTPTrader::MSG_TYPE type, string &msg);
	void emitMsg(MSG_LIST_S &msg);*/

        int login(void);
        int logout(void);
        int queryHold(void);
        int queryPos(void);
        int queryDepthData(string instrumentID);
        int insert(string instrumentID, int price, int volume, int code, int reqID);
        int insert2(string instrumentID, int price, int volume, int code, int reqID, int FrontID, int sessionID, string orderRef);
        int insertAction(string instrumentID, int price, int volume, int code, int reqID, int FrontID, int sessionID, string orderRef);
        int orderAction(int FrontID, int SessionID, char* OrderRef, string instrumentID);
        int orderAction2(char* orderSysID, string instrumentID);
        int queryOrderAction(string instrumentID, TThostFtdcOrderRefType orderRef, int reqID);
        int queryTrade(string instrumentID, int reqID);
        int queryOrder(string instrumentID, int reqID);

        int qryTradingAccount();
        void settleConfirm(void);

        void tick(void);

        string getBrokerID() { return brokerId; };
        string getUserID() { return userId; };
        string getPWD() { return passwd; };
        string getTradeIP() { return tradeIp; };
        /*string getMDIP() { return mdIp; };*/
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

	///客户端认证响应
        void OnRspAuthenticate(CThostFtdcRspAuthenticateField *pRspAuthenticateField, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);


	///登录请求响应
        void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///登出请求响应
        void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///用户口令更新请求响应
        void OnRspUserPasswordUpdate(CThostFtdcUserPasswordUpdateField *pUserPasswordUpdate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///错误应答
        void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///报单录入请求响应
        void OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///投资者结算结果确认响应
        void OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///报单通知
        void OnRtnOrder(CThostFtdcOrderField *pOrder);

	///成交通知
        void OnRtnTrade(CThostFtdcTradeField *pTrade);

	///报单录入错误回报
        void OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo);

	///报单操作错误回报
        void OnErrRtnOrderAction(CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo);

	///报单操作请求响应
        void OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询投资者持仓响应
        void OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询行情响应
        void OnRspQryDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///预埋单录入请求响应
        void OnRspParkedOrderInsert(CThostFtdcParkedOrderField *pParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///预埋撤单录入请求响应
        void OnRspParkedOrderAction(CThostFtdcParkedOrderActionField *pParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///删除预埋单响应
        void OnRspRemoveParkedOrder(CThostFtdcRemoveParkedOrderField *pRemoveParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///删除预埋撤单响应
        void OnRspRemoveParkedOrderAction(CThostFtdcRemoveParkedOrderActionField *pRemoveParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询报单响应
        void OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询成交响应
        void OnRspQryTrade(CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询资金账户响应
        void OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询合约手续费率响应
        void OnRspQryInstrumentCommissionRate(CThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///前置编号
	TThostFtdcFrontIDType	FrontID;
	///会话编号
	TThostFtdcSessionIDType	SessionID;
	///最大报单引用
	TThostFtdcOrderRefType	MaxOrderRef;
	///当前报单引用
	int CurOrderRefInt;
	TThostFtdcOrderRefType	CurOrderRef;
	///上一个报单引用
	TThostFtdcOrderRefType	LastOrderRef;



	TThostFtdcExchangeIDType	ExchangeID;

	TThostFtdcOrderSysIDType	OrderSysID;

	char* failStrInc(char* input) {
		static char buffer[13];
		static char buffer2[13];
		stringstream ss;

		for (int i = 0; i < 13; i++) {
			buffer[i] = '\0';
			buffer2[i] = '\0';
		}

		int number = atoi(input);
		number += 1;

		ss << number;

		int len = ss.str().length();
		strcpy(buffer, ss.str().c_str());

		int count = 12 - len;

		int i;
		for (i = 0; i < count; i++) {
			buffer2[i] = ' ';
		}
		buffer2[count] = '\0';
		strcat(buffer2, buffer);
		return buffer2;
	}

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

    CThostFtdcTraderApi *tradeApi;
	/*CThostFtdcMdApi *queryApi;*/
	string brokerId;
	string userId;
	string passwd;
	string tradeIp;
	/*string mdIp;*/

	//请求编号
	int reqId;

	MSG_LIST *m_MsgList;
};

//extern CCTPTrader *g_Trader;

#endif
