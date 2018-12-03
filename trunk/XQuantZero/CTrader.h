#pragma once
#ifndef _CTRADER_H_
#define _CTRADER_H_

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
/////�������һ
/////��������
//TThostFtdcOrderRefType	OrderRef;
/////�Ự���
//TThostFtdcSessionIDType	SessionID;
/////ǰ�ñ��
//TThostFtdcFrontIDType	FrontID;
//
/////������϶�
/////����������
//TThostFtdcExchangeIDType	ExchangeID;
/////�������
//TThostFtdcOrderSysIDType	OrderSysID;
/////��Լ����
//TThostFtdcInstrumentIDType	InstrumentID;
//
////����ɾ��Ԥ��
/////Ԥ����
//TThostFtdcParkedOrderIDType	ParkedOrderID;


///TODO: ����CurOrderRef���������ͻ
class CTrader : public CThostFtdcTraderSpi {
public:
	enum {
		SUCCESS_POST = 0, ///<�ɹ�
		FAILED_NETWORK = -1, ///<��������ʧ��
		FAILED_REACH_LIMITS = -2, ///<δ�������󳬹������
		FAILED_REACH_GRANTS = -3 ///<ÿ�뷢�����󳬹������
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
		ORDER_OPEN_BUY = 1, ///<����
		ORDER_OPEN_SELL = 2, ///<���� 
		ORDER_CLOSE_BUY = 3, ///<ƽ��
		ORDER_CLOSE_SELL = 4, ///<ƽ��
		ORDER_FOK = 5, ///<�����ɽ���������
		ORDER_FAK = 6, ///<�����ɽ���������
		ORDER_CONDITION = 7, ///<��������
		ORDER_ANYPRICE = 20, ///<�м۵�
		ORDER_CONDITION_TOUCH = 30, ///<ֹ��
		ORDER_CONDITION_TOUCHPROFIT = 31, ///<ֹӯ
		ORDER_CLOSE_BUY_TODAY,
		ORDER_CLOSE_SELL_TODAY,
		ORDER_CLOSE_BUY_YEST,
		ORDER_CLOSE_SELL_YEST
	};

	CTrader()
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

		//CTraderRsp *rsp = new CTraderRsp;
		//tradeApi->RegisterSpi(rsp);
		CurOrderRefInt = 1;
		snprintf(CurOrderRef, 13, "%.12d", CurOrderRefInt);
		snprintf(LastOrderRef, 13, "%.12d", CurOrderRefInt);
		/*strcpy(LastOrderRef, "           1");*/
	}

//    ~CTrader();

	void failIncRef() {
		CurOrderRefInt++;
		snprintf(CurOrderRef, 13, "%.12d", CurOrderRefInt);
		snprintf(LastOrderRef, 13, "%.12d", CurOrderRefInt);
	}

        /*MSG_LIST* getMsgList(void) { return m_MsgList; };

	void clearMsgList(void) { m_MsgList->clear(); };

	void genMsg(CTrader::MSG_TYPE type, string &msg);
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


	///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
        void OnFrontConnected();

	///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
	///@param nReason ����ԭ��
	///        0x1001 �����ʧ��
	///        0x1002 ����дʧ��
	///        0x2001 ����������ʱ
	///        0x2002 ��������ʧ��
	///        0x2003 �յ�������
        void OnFrontDisconnected(int nReason);

	///������ʱ���档����ʱ��δ�յ�����ʱ���÷��������á�
	///@param nTimeLapse �����ϴν��ձ��ĵ�ʱ��
        void OnHeartBeatWarning(int nTimeLapse);

	///�ͻ�����֤��Ӧ
        void OnRspAuthenticate(CThostFtdcRspAuthenticateField *pRspAuthenticateField, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);


	///��¼������Ӧ
        void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�ǳ�������Ӧ
        void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�û��������������Ӧ
        void OnRspUserPasswordUpdate(CThostFtdcUserPasswordUpdateField *pUserPasswordUpdate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///����Ӧ��
        void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///����¼��������Ӧ
        void OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///Ͷ���߽�����ȷ����Ӧ
        void OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///����֪ͨ
        void OnRtnOrder(CThostFtdcOrderField *pOrder);

	///�ɽ�֪ͨ
        void OnRtnTrade(CThostFtdcTradeField *pTrade);

	///����¼�����ر�
        void OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo);

	///������������ر�
        void OnErrRtnOrderAction(CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo);

	///��������������Ӧ
        void OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�����ѯͶ���ֲ߳���Ӧ
        void OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�����ѯ������Ӧ
        void OnRspQryDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///Ԥ��¼��������Ӧ
        void OnRspParkedOrderInsert(CThostFtdcParkedOrderField *pParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///Ԥ�񳷵�¼��������Ӧ
        void OnRspParkedOrderAction(CThostFtdcParkedOrderActionField *pParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///ɾ��Ԥ����Ӧ
        void OnRspRemoveParkedOrder(CThostFtdcRemoveParkedOrderField *pRemoveParkedOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///ɾ��Ԥ�񳷵���Ӧ
        void OnRspRemoveParkedOrderAction(CThostFtdcRemoveParkedOrderActionField *pRemoveParkedOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�����ѯ������Ӧ
        void OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�����ѯ�ɽ���Ӧ
        void OnRspQryTrade(CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�����ѯ�ʽ��˻���Ӧ
        void OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�����ѯ��Լ����������Ӧ
        void OnRspQryInstrumentCommissionRate(CThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///ǰ�ñ��
	TThostFtdcFrontIDType	FrontID;
	///�Ự���
	TThostFtdcSessionIDType	SessionID;
	///��󱨵�����
	TThostFtdcOrderRefType	MaxOrderRef;
	///��ǰ��������
	int CurOrderRefInt;
	TThostFtdcOrderRefType	CurOrderRef;
	///��һ����������
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

	//������
	int reqId;

	MSG_LIST *m_MsgList;
};

//extern CTrader *g_Trader;

#endif
