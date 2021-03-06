﻿#ifndef CEVENTFACTORY_H
#define CEVENTFACTORY_H

#include "cevent.h"
#include "cabstractevent.h"

class CEventFactory
{
public:
    CEventFactory();
    virtual ~CEventFactory();

    ///
    virtual CAbstractEvent* produceEvent(int eventID, void *userinfo);
    virtual CAbstractEvent* produceEvent(int eventID, void *userinfo, int delay);


};

#endif // CEVENTFACTORY_H
