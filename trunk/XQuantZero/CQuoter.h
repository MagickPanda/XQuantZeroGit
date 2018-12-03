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

class CQuoter : public CThostFtdcMdSpi {
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


	///��¼������Ӧ
        void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�ǳ�������Ӧ
        void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///����Ӧ��
        void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///��������Ӧ��
        void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///ȡ����������Ӧ��
        void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///����ѯ��Ӧ��
        void OnRspSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///ȡ������ѯ��Ӧ��
        void OnRspUnSubForQuoteRsp(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�������֪ͨ
        void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);

	///ѯ��֪ͨ
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

	//������
	int reqId;

	MSG_LIST m_MsgList;
};

//extern CQuoter *g_Quoter;

#endif
