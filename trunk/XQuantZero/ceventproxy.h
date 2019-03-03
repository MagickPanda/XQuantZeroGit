#ifndef CEVENTPROXY_H
#define CEVENTPROXY_H

#include <QThread>
#include <QObject>

class CEventProxy : public QObject
{
public:
    CEventProxy();
    ~CEventProxy() {
        delete m_Thread;
        m_Thread = nullptr;
    }

    static CEventProxy &getSingleton() {
        static CEventProxy m_Instance;
        return m_Instance;
    }

protected:
    QThread *m_Thread;
};

#endif // CEVENTPROXY_H
