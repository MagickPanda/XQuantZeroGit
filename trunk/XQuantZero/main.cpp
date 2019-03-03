#include "mainwindow.h"
#include <QApplication>
#include <QThread>
#include <QTimer>

#include "clogindialog.h"
#include "cconfighandler.h"

#include "ceventqueue.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // test section for event queue ## 测试事件队列
#if 1
    QThread t;
    QTimer timer;
    CEventQueue eventQueue;

    QObject::connect(&timer, SIGNAL(timeout()), &eventQueue, SLOT(OnProcess()));
    timer.start(50);

    eventQueue.moveToThread(&t);
    t.start();


#endif


    // test section for config loader ## 测试配置读取类
#if 1
    CConfigHandler &configHandler = CConfigHandler::getSingleton();

    configHandler.loadConfig("test.json");

    QString test1, test2, test3;

    test1 = configHandler.getValue(CConfigHandler::CONFIG_SIMNOW, "trade_front");
    test2 = configHandler.getValue(CConfigHandler::CONFIG_SIMNOW, "trade_port");
    test3 = configHandler.getValue(CConfigHandler::CONFIG_SIMNOW, "trade_broker_id");
#endif

    MainWindow &mainWindow = MainWindow::getSingleton();

    mainWindow.show();

    CLoginDialog cd;
    cd.show();

    return a.exec();
}
