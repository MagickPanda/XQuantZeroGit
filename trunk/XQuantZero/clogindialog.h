#ifndef CLOGINDIALOG_H
#define CLOGINDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QDebug>


namespace Ui {
class CLoginDialog;
}

class CLoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CLoginDialog(QWidget *parent = 0);
    ~CLoginDialog();

private:
    Ui::CLoginDialog *ui;
};

#endif // CLOGINDIALOG_H
