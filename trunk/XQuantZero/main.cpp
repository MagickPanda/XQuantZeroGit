#include "mainwindow.h"
#include "clogindialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CLoginDialog cd;
    cd.show();

    return a.exec();
}
