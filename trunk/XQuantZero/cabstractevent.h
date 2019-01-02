#ifndef CABSTRACTEVENT_H
#define CABSTRACTEVENT_H

#include <QObject>

class CAbstractEvent {
public:
    CAbstractEvent();
    virtual ~CAbstractEvent() {};

    qint32 getEventType(void) { return m_eventType; }


protected:
    void setEventType(qint32 type) { m_eventType = type; }
    qint32 m_eventType;
};


///> 中：登入抽象类 ## ENG:login abstract class
class CAbstractLoginEvent : public CAbstractEvent {
public:
    CAbstractLoginEvent();
    virtual ~CAbstractLoginEvent();
protected:

};

///> 中：登出抽象类 ## ENG:logout abstract class
class CAbstractLogoutEvent : public CAbstractEvent {
public:
    CAbstractLogoutEvent();
    virtual ~CAbstractLogoutEvent();
protected:
};

///> 中：超时断线抽象类 ## ENG:timeout disconnect abstract class
class CAbstractTimeoutEvent : public CAbstractEvent {
public:
    CAbstractTimeoutEvent();
    virtual ~CAbstractTimeoutEvent();
protected:

};

///> 中：断线抽象类 ## ENG:disconnect abstract class
class CAbstractDisconnectEvent : public CAbstractEvent {
public:
    CAbstractDisconnectEvent();
    virtual ~CAbstractDisconnectEvent();
protected:
};

#endif // CABSTRACTEVENT_H
