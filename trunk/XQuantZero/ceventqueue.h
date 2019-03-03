#ifndef CEVENTQUEUE_H
#define CEVENTQUEUE_H

#include <QtCore>
#include <QQueue>
#include <QObject>

#include "cevent.h"

class CEventQueue : public QObject
{
public:

    CEventQueue();

    static CEventQueue &getSingleton()
    {
        static CEventQueue m_Instance;
        return m_Instance;
    }

    void pushEvent(CEvent &evt);
    void popEvent(void);

private:

private slots:
    void OnProcess() {

    }

    QQueue<CEvent> m_Queue;
    QMutex m_Mutex;
};

#endif // CEVENTQUEUE_H
