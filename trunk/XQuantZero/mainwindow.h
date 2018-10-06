#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "cconsoleform.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT



public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    static const MainWindow* getInstance()
    {
        return m_Instance;
    }

private:
   static const MainWindow *m_Instance;

    Ui::MainWindow *ui;

    CConsoleForm *uiConsole;
};

#endif // MAINWINDOW_H
