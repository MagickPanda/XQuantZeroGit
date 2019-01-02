#include "ceventqueue.h"

CEventQueue::CEventQueue() {

}

/// Push event into queue(not immediate process) ## 队尾插入事件（不立即处理）
void CEventQueue::pushEvent(CAbstractEvent &evt) {
    m_PushMutex.lock();
    m_Queue.push_back(evt);
    m_PushMutex.unlock();
}

/// Process an event in front ## 处理一个队首事件
void CEventQueue::popEvent(void) {

}
