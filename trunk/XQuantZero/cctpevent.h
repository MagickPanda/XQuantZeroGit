#ifndef CCTPEVENT_H
#define CCTPEVENT_H

#include "cevent.h"
#include "cabstractevent.h"

#include "CCTPTraderRsp.h"
#include "CCTPTraderMD.h"

#include <QTableView>

namespace CTP_event {
    enum {
        EVENT_NONE = 0,
        EVENT_T_LOGIN,
        EVENT_T_LOGOUT,

        EVENT_Q_LOGIN = 10001,
        EVENT_Q_LOGOUT,
    };

    using namespace xq_event;

    static const char *usrProductInfo = "ashu";

    class TraderLoginEvent : public CAbstractLoginEvent {
    public:


        TraderLoginEvent(TraderLoginEvent &evt)
        {
            this->BrokerID = evt.BrokerID;
            this->UserID = evt.UserID;
            this->Password = evt.Password;
            this->UserProductInfo = evt.UserProductInfo;
        }

        QString BrokerID;
        QString UserID;
        QString Password;
        QString UserProductInfo;



    };

    class CCTPEvent : public CEvent
    {
    public:
        enum {
            NONE = 0,

        };

        CCTPEvent();
    };

}

#endif // CCTPEVENT_H
