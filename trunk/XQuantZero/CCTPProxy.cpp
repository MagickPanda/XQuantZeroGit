#include "CCTPProxy.h"

int CCTPProxy::login(void) {
    return mTrader.login();
}

int CCTPProxy::logout(void) {
    return mTrader.logout();
}

int CCTPProxy::queryHold(void) {
    return mTrader.queryHold();
}

int CCTPProxy::queryPos(void) {
    return mTrader.queryPos();
}
//virtual int queryDepthData(string instrumentID);
//virtual int insert(string instrumentID, int price, int volume, int code, int reqID);
//virtual int insert2(string instrumentID, int price, int volume, int code, int reqID, int FrontID, int sessionID, string orderRef);
//virtual int insertAction(string instrumentID, int price, int volume, int code, int reqID, int FrontID, int sessionID, string orderRef);
//virtual int orderAction(int FrontID, int SessionID, char* OrderRef, string instrumentID);
//virtual int orderAction2(char* orderSysID, string instrumentID);
//virtual int queryOrderAction(string instrumentID, TThostFtdcOrderRefType orderRef, int reqID);
//virtual int queryTrade(string instrumentID, int reqID);
//virtual int queryOrder(string instrumentID, int reqID);

//virtual int qryTradingAccount();
//virtual void settleConfirm(void);
