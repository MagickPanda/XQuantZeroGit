#include "ceventqueue.h"

CEventQueue::CEventQueue() {

}

/// Push event into queue(not immediate process) ## 队尾插入事件（不立即处理）
void CEventQueue::pushEvent(CEvent &evt) {
    m_Mutex.lock();
    m_Queue.push_back(evt);
    m_Mutex.unlock();
}

/// Process an event in front ## 处理一个队首事件
void CEventQueue::popEvent(void) {

}
