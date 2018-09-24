#pragma warning(disable:4996)

#ifndef SWBASE_H
#define SWBASE_H

#include <string>
#include <list>
#include <iostream>
#include <sstream>
#include <iomanip>


#include "STDLL.h"

#include "time.h"

#include <QApplication>
#include <QCoreApplication>
#include <QDebug>
#include <QStringList>


#ifdef _DEBUG

#define sw_debug 1 ///<flag for debug routines
#ifndef DBG_NEW
	#define debug_new new (_NORMAL_BLOCK , __FILE__ , __LINE__)
	//#define new DBG_NEW
#endif

#else
#define sw_debug 0 ///<flag for debug routines
#ifndef DBG_NEW

	#define debug_new new
	//#define new DBG_NEW
#endif

#endif

using namespace std;

typedef struct MSG_LIST_S {
	int type;
	string msg;
};


typedef list<MSG_LIST_S> MSG_LIST;


typedef struct _RATETABLE {
	string name;
	string nameEng;
	string exchangeID;
	int multiplier;
	string quantID;
	string exchangeName;

} RateTable;

static int nCons = 8;

static const char *sCons[64] = {
	"thostmduserapi.dllDialogRsp.con",
	"thostmduserapi.dllQueryRsp.con",
	"thostmduserapi.dllTradingDay.con",
	"thosttraderapi.dllDialogRsp.con",
	"thosttraderapi.dllPrivate.con",
	"thosttraderapi.dllPublic.con",
	"thosttraderapi.dllQueryRsp.con",
	"thosttraderapi.dllTradingDay.con"
};

enum {
	LOG_TRADE = 0,
	LOG_QUOTE = 1,
	LOG_MISC = 2,
	LOG_ORDER = 3,
	LOG_STRAT = 4,
};

enum {
	OS_DIR_BUY,
	OS_DIR_SELL,
};

enum {
	OS_ID = -1, //reserved filename is ID now
	OS_reqID1,
	OS_reqID2,
	OS_bDiff,
	OS_product1,
	OS_quantity1,
	OS_dir1,
	OS_price1,
	OS_price1_target,
	OS_price1_fail,
	OS_product2,
	OS_quantity2,
	OS_dir2,
	OS_price2,
	OS_price2_target,
	OS_price2_fail,
	OS_diff,
	OS_diff_target,
	OS_diff_fail,
	OS_user_ID,
	OS_type,
	OS_OrderRef1,
	OS_SessionID1,
	OS_FrontID1,
	OS_ExchangeID1,
	OS_OrderSysID1,
	OS_OrderRef2,
	OS_SessionID2,
	OS_FrontID2,
	OS_ExchangeID2,
	OS_OrderSysID2,
	//TODO:add second 5 params
	OS_InsertDate,
	OS_InsertTime,
	OS_FinishTime,
	OS_ActiveTime,
	OS_UpdateTime,
	OS_CancelTime,
	OS_SuspendTime,
	OS_OrderType,
};

typedef int OS_StrategyType;
enum {
	OS_STR_NONE = 0,
	OS_STR_MA,
	OS_STR_DIFF,
};

typedef int OS_OrderStatusType;
enum {
	OS_ST_INVALID = -1, ///<summary>错误状态（单子已删除）</summary> 
	OS_ST_NONE, ///<summary>初始状态</summary>
	OS_ST_INSERTING, ///<summary>下单中</summary>
	OS_ST_INSERTING_ACK, ///<summary>确认下单</summary>
	OS_ST_INSERTED, ///<summary>已下单回报</summary>
	OS_ST_CONFIRMED, ///<summary>成交</summary>
	OS_ST_INSERT_CANCEL, ///<summary>取消下单</summary>
	OS_ST_INSERT_CANCEL_ACK, ///<summary>确认取消下单</summary>
	OS_ST_INSERT_CANCELLED, ///<summary>已取消下单</summary>
	OS_ST_CONFIRM_CANCEL, ///<summary>取消回报单</summary>
	OS_ST_CONFIRM_CANCEL_ACK, ///<summary>确认取消回报单</summary>
	OS_ST_CONFIRM_CANCELLED, ///<summary>已取消回报单</summary>
	OS_ST_FINISHING, ///<summary>平仓中</summary>
	OS_ST_FINISHING_ACK, ///<summary>确认平仓</summary>
	OS_ST_FINISH_CANCEL, ///<summary>取消平仓</summary>
	OS_ST_FINISH_CANCEL_ACK, ///<summary>确认取消平仓</summary>
	OS_ST_FINISH_CANCELLED, ///<summary>已取消平仓单</summary>
	OS_ST_FINISHED, ///<summary>平仓后</summary>

	OS_ST_INSERT_FAIL, ///<summary>开仓失败</summary>
	OS_ST_FINISH_FAIL, ///<summary>平仓失败</summary>
	//OS_ST_INVALID, ///<summary>错误状态</summary>
};

static const int OS_ST_STRING_SIZE = 20;

static const char* os_st_table[20] = {
	"无状态","下单中","确认下单","成交确认中","成交","取消下单",
	"确认取消下单","已取消下单","取消回报单","确认取消回报单","已取消回报单",
	"平仓中","确认平仓","取消平仓","确认取消平仓","已取消平仓单","平仓后",
	"开仓失败", "平仓失败","错误状态",

};

static const int OS_STRING_SIZE = 38;

static const char* os_string_index = "订单参数";

static const char* os_string_table[OS_STRING_SIZE] = {
	"reqID1", "reqID2", "组合标志",
	"产品1", "持仓1", "方向1", "价格1", "价格1止盈", "价格1止损",
	"产品2", "持仓2", "方向2", "价格2", "价格2止盈", "价格2止损",
	"组合价差", "组合止盈", "组合止损", "用户账号", "单子状态",
	"OrderRef1", "SessionID1", "FrontID1", "ExchangeID1", "OrderSysID1",
	"OrderRef2", "SessionID2", "FrontID2", "ExchangeID2", "OrderSysID2",
	"插入日期", "插入时间", "完成时间", "激活时间", "更新时间", "撤销时间",
	"停止时间", "单子类型"
};

typedef struct order_struct {
	unsigned int reqID1; ///<reqID1 1
	unsigned int reqID2; ///<reqID2 2
	bool bDiff; ///<if a diff 3
	string product1; ///<产品1名称 4
	int quantity1; ///<产品1数量 5
	int dir1; ///<方向1 6
	double price1; ///<价格1 7
	double price1_target; ///<价格1目标 8
	double price1_fail; ///<价格1止损 9
	string product2; ///<产品2名称 10
	int quantity2; ///<产品2数量 11
	int dir2; ///<方向2 12
	double price2; ///<价格2 13
	double price2_target; ///<价格2目标 14
	double price2_fail; ///<价格2止损 15
	double diff; ///入场价差 16
	double diff_target; ///<价差目标 17
	double diff_fail; ///<价差止损 18

	string user_ID; ///<登录账户 19
	OS_OrderStatusType type; ///<类型 0挂单 1成交 2运行 3撤单 20

	char OrderRef1[13]; ///<单子Ref1 21
	int SessionID1; ///<单子SessionID1 22
	int FrontID1; ///<单子FrontID1 23
	char ExchangeID1[9]; ///<单子ExchangeID1 24
	char OrderSysID1[21]; ///<单子OrderSysID! 25

	char OrderRef2[13]; ///<单子Ref2 26
	int SessionID2; ///<单子SessionID2 27
	int FrontID2; ///<单子FrontID2 28
	char ExchangeID2[9]; ///<单子ExchangeID2 29
	char OrderSysID2[21]; ///<单子OrderSysID2 30

	char InsertDate[30]; ///<插入日期 31
	char InsertTime[36]; ///<插入时间 32
	char FinishTime[36]; ///<完成时间 33
	char ActiveTime[36]; ///<激活时间 34
	char UpdateTime[36]; ///<更新时间 35
	char CancelTime[36]; ///<撤销时间 36
	char SuspendTime[36]; ///<停止时间 37
	char OrderType[36]; ///<单子类型 38
};

#define SW_LEN_EXCHANGE 8
#define SW_LEN_SECID 24
#define SW_LEN_ISO_DATATIME 36

typedef struct SW_BAR
{
	//char                    exchange[SW_LEN_EXCHANGE];       ///<交易所代码
	//char                    sec_id[SW_LEN_SECID];            ///<证券ID
	//int                     bar_type;                     ///<分时周期类型(enum BarType)
	double                  utc_time;                     ///bar的开始时间
	char                    strtime[SW_LEN_ISO_DATATIME];    ///bar的开始时间
	double                  utc_endtime;                  ///bar的结束时间
	char                    strendtime[SW_LEN_ISO_DATATIME]; ///bar的结束时间

	float                   open;                         ///<开盘价
	float                   close;                        ///<收盘价
	float                   high;                         ///<最高价
	float                   low;                          ///<最低价
	double                  volume;                       ///<成交量
	double                  amount;                       ///<成交金额
	float                   pre_close;                    ///前收盘价

	long long                position;                    ///<持仓量
	float                   adj_factor;                   //复权因子
	int                     flag;                         //除权出息，停牌等标记

	double					bid; //买价
	double					ask; //卖价
} SWBar;

template <class Type>
Type StringToNum(const string& str)
{
	istringstream iss(str);
	Type num;
	iss >> num;
	return num;
}

template <class Type>
string NumberToString2(Type in)
{
	stringstream ss;
	ss.str("");;
	ss << in;
	return ss.str();
}

template <class Type>
void NumberToString(Type val, string &ret)
{
	stringstream ss;
	ss.str("");;
	ss << val;
	ss >> ret;
}

void DoubleToString(double val, string &ret);


string sw_getDate(void);
string sw_getTime(void);
string sw_getFullTime(void);
string orderToRef(int orderID);

static const int DEFAULT_TIMEOUTS = 50;
static const int INSERT_ACK_TIMEOUTS = 50;
static const int INSERT_CANCEL_TIMEOUTS = 50;
static const int INSERT_CANCEL_ACK_TIMEOUTS = 50;

static int orderID = 1;



//TODO:move to new file one day
class CStratMA {
public:
	CStratMA() {
		product = "NONE";
		direction = -1;
		runs = 0;
		insert_timeouts = DEFAULT_TIMEOUTS; ///<4次，相当于2秒
		insert_ack_timeouts = INSERT_ACK_TIMEOUTS;
		insert_cancel_timeouts = INSERT_CANCEL_TIMEOUTS; ///<4次，相当于2秒
		finish_timeouts = DEFAULT_TIMEOUTS; ///<4次，相当于2秒
		finish_ack_timeouts = INSERT_ACK_TIMEOUTS;
		finish_cancel_timeouts = DEFAULT_TIMEOUTS; ///<4次，相当于2秒
		confirm_timeouts = DEFAULT_TIMEOUTS; ///<4次，相当于2秒
		confirm_cancel_timeouts = DEFAULT_TIMEOUTS; ///<4次，相当于2秒
		memset(insertOrderRef1, 0, 13);
		memset(insertOrderRef2, 0, 13);
		memset(finishOrderRef1, 0, 13);
		memset(finishOrderRef2, 0, 13);
		memset(insertOrderSysID1, 0, 9);
		memset(finishOrderSysID1, 0, 9);
		memset(insertOrderSysID2, 0, 9);
		memset(finishOrderSysID2, 0, 9);

		memset(InsertDate, 0, 30);
		status = OS_ST_NONE;

		MA_interval = 33;
		K_interval = 10;
		win_percent = 0.1;
		lose_percent = 0.1;
		min_win = 1.0;
		min_lose = 1.0;

		price = -1;
		fail_price = -1;
		win_price = -1;
		insertID = -1;
		finishID = -1;

		insertID = -1;
		finishID = -1;
	}

	void reset(void) {
		runs = 0;
		insert_timeouts = DEFAULT_TIMEOUTS; ///<4次，相当于2秒
		insert_ack_timeouts = INSERT_ACK_TIMEOUTS;
		insert_cancel_timeouts = INSERT_CANCEL_TIMEOUTS; ///<4次，相当于2秒
		finish_timeouts = DEFAULT_TIMEOUTS; ///<4次，相当于2秒
		finish_ack_timeouts = INSERT_ACK_TIMEOUTS;
		finish_cancel_timeouts = DEFAULT_TIMEOUTS; ///<4次，相当于2秒
		confirm_timeouts = DEFAULT_TIMEOUTS; ///<4次，相当于2秒
		confirm_cancel_timeouts = DEFAULT_TIMEOUTS; ///<4次，相当于2秒
		memset(insertOrderRef1, 0, 13);
		memset(insertOrderRef2, 0, 13);
		memset(finishOrderRef1, 0, 13);
		memset(finishOrderRef2, 0, 13);

		memset(insertOrderSysID1, 0, 9);
		memset(finishOrderSysID1, 0, 9);
		memset(insertOrderSysID2, 0, 9);
		memset(finishOrderSysID2, 0, 9);

		memset(InsertDate, 0, 30);
		status = OS_ST_NONE;

		//MA_interval = 3;
		//K_interval = 10;
		//win_percent = 0.1;
		//lose_percent = 0.1;
		//min_win = 1.0;
		//min_lose = 1.0;

		price = - 1;
		fail_price = -1;
		win_price = -1;
		insertID = -1;
		finishID = -1;
	}

	void setStatus(OS_OrderStatusType s) {
		this->lastStatus = this->status;
		this->status = s;
	}

	void setInsertOrderSysID1(char* orderSysID) {
		strncpy(this->insertOrderSysID1, orderSysID, 9);
	}

	void setFinishOrderSysID1(char* orderSysID) {
		strncpy(this->finishOrderSysID1, orderSysID, 9);
	}

	CStratMA& operator=(const CStratMA& _r) {
		product = _r.product;
		direction = _r.direction;
		MA_interval = _r.MA_interval;

		K_interval = _r.K_interval;
		win_percent = _r.win_percent;
		lose_percent = _r.lose_percent;
		min_win = _r.min_win;
		min_lose = _r.min_lose;
		runs = _r.runs;

		price = _r.price;
		fail_price = _r.fail_price;
		win_price = _r.win_price;

		insert_timeouts = _r.insert_timeouts;
		insert_ack_timeouts = _r.insert_ack_timeouts;
		insert_cancel_timeouts = _r.insert_cancel_timeouts;
		finish_timeouts = _r.finish_timeouts;
		finish_ack_timeouts = _r.finish_ack_timeouts;
		finish_cancel_timeouts = _r.finish_cancel_timeouts;
		confirm_timeouts = _r.confirm_timeouts;
		confirm_cancel_timeouts = _r.confirm_cancel_timeouts;

		insertID = _r.insertID;
		finishID = _r.finishID;

		strncpy(insertOrderRef1, _r.insertOrderRef1, 13);
		strncpy(insertOrderRef2, _r.insertOrderRef2, 13);
		strncpy(finishOrderRef1, _r.finishOrderRef1, 13);
		strncpy(finishOrderRef2, _r.finishOrderRef2, 13);

		status = _r.status;

		strncpy(InsertDate, _r.InsertDate, 30);
		SessionID1 = _r.SessionID1;
		FrontID1 = _r.FrontID1;


		lastStatus = _r.lastStatus;

		strncpy(insertOrderSysID1, _r.insertOrderSysID1, 9);
		strncpy(finishOrderSysID1, _r.finishOrderSysID1, 9);
		strncpy(insertOrderSysID2, _r.insertOrderSysID2, 9);
		strncpy(finishOrderSysID2, _r.finishOrderSysID2, 9);

		return *this;
	}

	/* Group parameters */
	string product;
	int direction;
	int MA_interval;
	int K_interval;
	double win_percent;
	double lose_percent;
	double min_win;
	double min_lose;
	int runs;

	double price;
	double fail_price;
	double win_price;

	int insert_timeouts; ///<成交失败次数限制，到达时放弃策略重置
	int insert_ack_timeouts;
	int insert_cancel_timeouts;
	int finish_timeouts;
	int finish_ack_timeouts;
	int finish_cancel_timeouts;
	int confirm_timeouts;
	int confirm_cancel_timeouts;
	/* End */


	/* Group status */
	int insertID; ///<下单ID（用户）
	int finishID; ///<平仓ID（用户）

	char insertOrderRef1[13];
	char insertOrderRef2[13];
	char finishOrderRef1[13];
	char finishOrderRef2[13];

	OS_OrderStatusType status; ///<订单状态，挂起成交或完成

	//block for order info


	char InsertDate[30]; ///<插入日期 31
	int SessionID1; ///<单子SessionID1 22
	int FrontID1; ///<单子FrontID1 23
	/* End */

	//block for new status 
	OS_OrderStatusType lastStatus;
	char insertOrderSysID1[9];
	char finishOrderSysID1[9];
	char insertOrderSysID2[9];
	char finishOrderSysID2[9];
};

static const int MAX_STRAT_SLOTS = 10;

enum {
	STRAT_NONE = 0, ///<summary>无策略< / summary>
	STRAT_MA = 1, ///<summary>均线策略< / summary>
	STRAT_KA = 2, ///<summary>KA策略< / summary>
};

typedef struct strat_s {
	bool bRuns[MAX_STRAT_SLOTS];
	OS_StrategyType strat_type[MAX_STRAT_SLOTS];
	CStratMA stratsMA[MAX_STRAT_SLOTS];

};

#define CLEAR_SS(SS) SS.clear() \
					SS.str("")

#endif
