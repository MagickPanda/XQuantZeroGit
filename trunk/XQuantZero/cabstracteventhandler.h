#ifndef CABSTRACTEVENTHANDLER_H
#define CABSTRACTEVENTHANDLER_H

#include <QObject>
#include <QThread>

class CAbstractEventHandler : public QObject
{
    Q_OBJECT
public:
    CAbstractEventHandler();
};

#endif // CABSTRACTEVENTHANDLER_H
