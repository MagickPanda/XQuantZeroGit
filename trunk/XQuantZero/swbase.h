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
	OS_ST_INVALID = -1, ///<summary>����״̬��������ɾ����</summary> 
	OS_ST_NONE, ///<summary>��ʼ״̬</summary>
	OS_ST_INSERTING, ///<summary>�µ���</summary>
	OS_ST_INSERTING_ACK, ///<summary>ȷ���µ�</summary>
	OS_ST_INSERTED, ///<summary>���µ��ر�</summary>
	OS_ST_CONFIRMED, ///<summary>�ɽ�</summary>
	OS_ST_INSERT_CANCEL, ///<summary>ȡ���µ�</summary>
	OS_ST_INSERT_CANCEL_ACK, ///<summary>ȷ��ȡ���µ�</summary>
	OS_ST_INSERT_CANCELLED, ///<summary>��ȡ���µ�</summary>
	OS_ST_CONFIRM_CANCEL, ///<summary>ȡ���ر���</summary>
	OS_ST_CONFIRM_CANCEL_ACK, ///<summary>ȷ��ȡ���ر���</summary>
	OS_ST_CONFIRM_CANCELLED, ///<summary>��ȡ���ر���</summary>
	OS_ST_FINISHING, ///<summary>ƽ����</summary>
	OS_ST_FINISHING_ACK, ///<summary>ȷ��ƽ��</summary>
	OS_ST_FINISH_CANCEL, ///<summary>ȡ��ƽ��</summary>
	OS_ST_FINISH_CANCEL_ACK, ///<summary>ȷ��ȡ��ƽ��</summary>
	OS_ST_FINISH_CANCELLED, ///<summary>��ȡ��ƽ�ֵ�</summary>
	OS_ST_FINISHED, ///<summary>ƽ�ֺ�</summary>

	OS_ST_INSERT_FAIL, ///<summary>����ʧ��</summary>
	OS_ST_FINISH_FAIL, ///<summary>ƽ��ʧ��</summary>
	//OS_ST_INVALID, ///<summary>����״̬</summary>
};

static const int OS_ST_STRING_SIZE = 20;

static const char* os_st_table[20] = {
	"��״̬","�µ���","ȷ���µ�","�ɽ�ȷ����","�ɽ�","ȡ���µ�",
	"ȷ��ȡ���µ�","��ȡ���µ�","ȡ���ر���","ȷ��ȡ���ر���","��ȡ���ر���",
	"ƽ����","ȷ��ƽ��","ȡ��ƽ��","ȷ��ȡ��ƽ��","��ȡ��ƽ�ֵ�","ƽ�ֺ�",
	"����ʧ��", "ƽ��ʧ��","����״̬",

};

static const int OS_STRING_SIZE = 38;

static const char* os_string_index = "��������";

static const char* os_string_table[OS_STRING_SIZE] = {
	"reqID1", "reqID2", "��ϱ�־",
	"��Ʒ1", "�ֲ�1", "����1", "�۸�1", "�۸�1ֹӯ", "�۸�1ֹ��",
	"��Ʒ2", "�ֲ�2", "����2", "�۸�2", "�۸�2ֹӯ", "�۸�2ֹ��",
	"��ϼ۲�", "���ֹӯ", "���ֹ��", "�û��˺�", "����״̬",
	"OrderRef1", "SessionID1", "FrontID1", "ExchangeID1", "OrderSysID1",
	"OrderRef2", "SessionID2", "FrontID2", "ExchangeID2", "OrderSysID2",
	"��������", "����ʱ��", "���ʱ��", "����ʱ��", "����ʱ��", "����ʱ��",
	"ֹͣʱ��", "��������"
};

typedef struct order_struct {
	unsigned int reqID1; ///<reqID1 1
	unsigned int reqID2; ///<reqID2 2
	bool bDiff; ///<if a diff 3
	string product1; ///<��Ʒ1���� 4
	int quantity1; ///<��Ʒ1���� 5
	int dir1; ///<����1 6
	double price1; ///<�۸�1 7
	double price1_target; ///<�۸�1Ŀ�� 8
	double price1_fail; ///<�۸�1ֹ�� 9
	string product2; ///<��Ʒ2���� 10
	int quantity2; ///<��Ʒ2���� 11
	int dir2; ///<����2 12
	double price2; ///<�۸�2 13
	double price2_target; ///<�۸�2Ŀ�� 14
	double price2_fail; ///<�۸�2ֹ�� 15
	double diff; ///�볡�۲� 16
	double diff_target; ///<�۲�Ŀ�� 17
	double diff_fail; ///<�۲�ֹ�� 18

	string user_ID; ///<��¼�˻� 19
	OS_OrderStatusType type; ///<���� 0�ҵ� 1�ɽ� 2���� 3���� 20

	char OrderRef1[13]; ///<����Ref1 21
	int SessionID1; ///<����SessionID1 22
	int FrontID1; ///<����FrontID1 23
	char ExchangeID1[9]; ///<����ExchangeID1 24
	char OrderSysID1[21]; ///<����OrderSysID! 25

	char OrderRef2[13]; ///<����Ref2 26
	int SessionID2; ///<����SessionID2 27
	int FrontID2; ///<����FrontID2 28
	char ExchangeID2[9]; ///<����ExchangeID2 29
	char OrderSysID2[21]; ///<����OrderSysID2 30

	char InsertDate[30]; ///<�������� 31
	char InsertTime[36]; ///<����ʱ�� 32
	char FinishTime[36]; ///<���ʱ�� 33
	char ActiveTime[36]; ///<����ʱ�� 34
	char UpdateTime[36]; ///<����ʱ�� 35
	char CancelTime[36]; ///<����ʱ�� 36
	char SuspendTime[36]; ///<ֹͣʱ�� 37
	char OrderType[36]; ///<�������� 38
};

#define SW_LEN_EXCHANGE 8
#define SW_LEN_SECID 24
#define SW_LEN_ISO_DATATIME 36

typedef struct SW_BAR
{
	//char                    exchange[SW_LEN_EXCHANGE];       ///<����������
	//char                    sec_id[SW_LEN_SECID];            ///<֤ȯID
	//int                     bar_type;                     ///<��ʱ��������(enum BarType)
	double                  utc_time;                     ///bar�Ŀ�ʼʱ��
	char                    strtime[SW_LEN_ISO_DATATIME];    ///bar�Ŀ�ʼʱ��
	double                  utc_endtime;                  ///bar�Ľ���ʱ��
	char                    strendtime[SW_LEN_ISO_DATATIME]; ///bar�Ľ���ʱ��

	float                   open;                         ///<���̼�
	float                   close;                        ///<���̼�
	float                   high;                         ///<��߼�
	float                   low;                          ///<��ͼ�
	double                  volume;                       ///<�ɽ���
	double                  amount;                       ///<�ɽ����
	float                   pre_close;                    ///ǰ���̼�

	long long                position;                    ///<�ֲ���
	float                   adj_factor;                   //��Ȩ����
	int                     flag;                         //��Ȩ��Ϣ��ͣ�Ƶȱ��

	double					bid; //���
	double					ask; //����
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
		insert_timeouts = DEFAULT_TIMEOUTS; ///<4�Σ��൱��2��
		insert_ack_timeouts = INSERT_ACK_TIMEOUTS;
		insert_cancel_timeouts = INSERT_CANCEL_TIMEOUTS; ///<4�Σ��൱��2��
		finish_timeouts = DEFAULT_TIMEOUTS; ///<4�Σ��൱��2��
		finish_ack_timeouts = INSERT_ACK_TIMEOUTS;
		finish_cancel_timeouts = DEFAULT_TIMEOUTS; ///<4�Σ��൱��2��
		confirm_timeouts = DEFAULT_TIMEOUTS; ///<4�Σ��൱��2��
		confirm_cancel_timeouts = DEFAULT_TIMEOUTS; ///<4�Σ��൱��2��
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
		insert_timeouts = DEFAULT_TIMEOUTS; ///<4�Σ��൱��2��
		insert_ack_timeouts = INSERT_ACK_TIMEOUTS;
		insert_cancel_timeouts = INSERT_CANCEL_TIMEOUTS; ///<4�Σ��൱��2��
		finish_timeouts = DEFAULT_TIMEOUTS; ///<4�Σ��൱��2��
		finish_ack_timeouts = INSERT_ACK_TIMEOUTS;
		finish_cancel_timeouts = DEFAULT_TIMEOUTS; ///<4�Σ��൱��2��
		confirm_timeouts = DEFAULT_TIMEOUTS; ///<4�Σ��൱��2��
		confirm_cancel_timeouts = DEFAULT_TIMEOUTS; ///<4�Σ��൱��2��
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

	int insert_timeouts; ///<�ɽ�ʧ�ܴ������ƣ�����ʱ������������
	int insert_ack_timeouts;
	int insert_cancel_timeouts;
	int finish_timeouts;
	int finish_ack_timeouts;
	int finish_cancel_timeouts;
	int confirm_timeouts;
	int confirm_cancel_timeouts;
	/* End */


	/* Group status */
	int insertID; ///<�µ�ID���û���
	int finishID; ///<ƽ��ID���û���

	char insertOrderRef1[13];
	char insertOrderRef2[13];
	char finishOrderRef1[13];
	char finishOrderRef2[13];

	OS_OrderStatusType status; ///<����״̬������ɽ������

	//block for order info


	char InsertDate[30]; ///<�������� 31
	int SessionID1; ///<����SessionID1 22
	int FrontID1; ///<����FrontID1 23
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
	STRAT_NONE = 0, ///<summary>�޲���< / summary>
	STRAT_MA = 1, ///<summary>���߲���< / summary>
	STRAT_KA = 2, ///<summary>KA����< / summary>
};

typedef struct strat_s {
	bool bRuns[MAX_STRAT_SLOTS];
	OS_StrategyType strat_type[MAX_STRAT_SLOTS];
	CStratMA stratsMA[MAX_STRAT_SLOTS];

};

#define CLEAR_SS(SS) SS.clear() \
					SS.str("")

#endif
