#include "ceventproxy.h"

CEventProxy::CEventProxy()
{
    m_Thread = new QThread(this);
}
