#pragma once
class CBaseTrader
{
public:
        CBaseTrader();
        virtual ~CBaseTrader();

    /*
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
    string getMDIP() { return mdIp; };
    int getRequestID() { return reqId; };
    */
};



