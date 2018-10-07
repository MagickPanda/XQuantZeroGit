#include "mainwindow.h"
#include <QApplication>

#include "clogindialog.h"
#include "cconfighandler.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // test section for config loader ## ≤‚ ‘≈‰÷√∂¡»°¿‡
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
