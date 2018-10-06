#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    uiConsole = new CConsoleForm();

    uiConsole->show();
}

MainWindow::~MainWindow()
{
    delete uiConsole;
    delete ui;
}
