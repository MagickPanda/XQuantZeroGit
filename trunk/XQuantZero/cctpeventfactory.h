#ifndef CCTPEVENTFACTORY_H
#define CCTPEVENTFACTORY_H

#include <QtGlobal>
#include <QSharedPointer>

#include "cctpevent.h"
#include "ceventfactory.h"

// 中：包含CTP头文件 ## ENG:Include CTP headers
#include "ThostFtdcMdApi.h"
#include "ThostFtdcTraderApi.h"


using namespace xq_event;

class CCTPEventFactory : public CEventFactory
{
public:
    CCTPEventFactory();
    ~CCTPEventFactory();

    static CCTPEventFactory &getSingleton()
    {
        static CCTPEventFactory m_Instance;
        return m_Instance;
    }

    //CAbstractEvent produceEvent(TraderLoginEvent &evt);

    CAbstractEvent* produceEvent(qint32 eventID, void *userinfo);
    CAbstractEvent* produceEvent(qint32 eventID, void *userinfo, qint32 delay);

private:
    inline CAbstractEvent* traderLoginEvent(void *userinfo);
    inline CAbstractEvent* quoterLoginEvent(void *userinfo);

};

CAbstractEvent* CCTPEventFactory::produceEvent(qint32 eventID, void *userinfo) {
    CAbstractEvent *newEvent;
    switch (eventID) {
    case CTP_event::EVENT_T_LOGIN:
        newEvent = traderLoginEvent(userinfo);
        break;
    case CTP_event::EVENT_NONE:
    default:
        newEvent = nullptr;
        printf("wrong event type");
        break;

    }
    return newEvent;
}

static CCTPEventFactory ctpFactory;

#endif // CCTPEVENTFACTORY_H
