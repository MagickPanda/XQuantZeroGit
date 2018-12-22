#include "clogindialog.h"
#include "ui_clogindialog.h"

CLoginDialog::CLoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CLoginDialog)
{
    ui->setupUi(this);
}

CLoginDialog::~CLoginDialog()
{
    delete ui;
}

void CLoginDialog::on_buttonBox_clicked(QAbstractButton *button)
{
    QString test = "meow";

    printf("meow");
}
