#pragma once

#include "cctptrader.h"
#include "ctraderproxy.h"

class CCTPProxy : public CTraderProxy
{
public:
    CCTPProxy(CCTPTrader trader) : mTrader(trader) {}

    virtual int login(void);

    virtual int logout(void);
    virtual int queryHold(void);
    virtual int queryPos(void);
    /*
    virtual int queryDepthData(string instrumentID);
    virtual int insert(string instrumentID, int price, int volume, int code, int reqID);
    virtual int insert2(string instrumentID, int price, int volume, int code, int reqID, int FrontID, int sessionID, string orderRef);
    virtual int insertAction(string instrumentID, int price, int volume, int code, int reqID, int FrontID, int sessionID, string orderRef);
    virtual int orderAction(int FrontID, int SessionID, char* OrderRef, string instrumentID);
    virtual int orderAction2(char* orderSysID, string instrumentID);
    virtual int queryOrderAction(string instrumentID, TThostFtdcOrderRefType orderRef, int reqID);
    virtual int queryTrade(string instrumentID, int reqID);
    virtual int queryOrder(string instrumentID, int reqID);

    virtual int qryTradingAccount();
    virtual void settleConfirm(void);
    */

private:
    CCTPTrader mTrader;
};

