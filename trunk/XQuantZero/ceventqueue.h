#ifndef CEVENTQUEUE_H
#define CEVENTQUEUE_H

#include <QtCore>
#include <QQueue>
#include <QObject>

#include "cevent.h"
#include "cabstractevent.h"

using namespace xq_event;

class CEventQueue : public QObject
{
public:

    CEventQueue();

    static CEventQueue &getSingleton()
    {
        static CEventQueue m_Instance;
        return m_Instance;
    }

    void pushEvent(CAbstractEvent &evt);
    void popEvent(void);

private:

private slots:
    void OnProcess() {

    }

    QQueue<CAbstractEvent> m_Queue;
    QMutex m_PushMutex;
};

#endif // CEVENTQUEUE_H
