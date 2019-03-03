#include "ceventqueue.h"

CEventQueue::CEventQueue() {

}

/// Push event into queue(not immediate process) ## ��β�����¼�������������
void CEventQueue::pushEvent(CEvent &evt) {
    m_Mutex.lock();
    m_Queue.push_back(evt);
    m_Mutex.unlock();
}

/// Process an event in front ## ����һ�������¼�
void CEventQueue::popEvent(void) {

}
