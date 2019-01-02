#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "cconsoleform.h"

#include "CCTPQuoter.h"
#include "CCTPTrader.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT



public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    static MainWindow &getSingleton()
    {
        static MainWindow m_Instance;
        return m_Instance;
    }

private:
    CCTPTrader *trader;
    CCTPQuoter *quoter;


    Ui::MainWindow *ui;

    CConsoleForm *uiConsole;

private slots:

};

#endif // MAINWINDOW_H
