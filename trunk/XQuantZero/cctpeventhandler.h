#ifndef CCTPEVENTHANDLER_H
#define CCTPEVENTHANDLER_H

#include "cabstractevent.h"
#include "cabstracteventhandler.h"
#include "cctpevent.h"

class CCTPEventHandler : public CAbstractEventHandler, QThread
{
public:
    CCTPEventHandler();

    static CCTPEventHandler &getSingleton()
    {
        static CCTPEventHandler m_Instance;
        return m_Instance;
    }

    void run();
public slots:
    void fireEvent(CAbstractEvent *event);
private:
    void inline processList(void);
    QList<CAbstractEvent> m_List;
};

#endif // CCTPEVENTHANDLER_H
