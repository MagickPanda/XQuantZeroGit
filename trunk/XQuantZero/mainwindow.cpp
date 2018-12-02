#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    uiConsole = new CConsoleForm();

    uiConsole->show();

    //trader = new CTrader();
   // quoter = new CQuoter();
}

MainWindow::~MainWindow()
{
    delete uiConsole;
    uiConsole = nullptr;
    delete ui;
    ui = nullptr;
    //delete trader;
    //trader = nullptr;
    //delete quoter;
    //quoter = nullptr;
}
