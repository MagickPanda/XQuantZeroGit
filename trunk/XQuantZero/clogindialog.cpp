#include "clogindialog.h"
#include "ui_clogindialog.h"

CLoginDialog::CLoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CLoginDialog)
{
    ui->setupUi(this);

    QComboBox *serverListBox = ui->comboBox;

    CConfigHandler &cfg = CConfigHandler::getSingleton();

    QString server_ctp = cfg.getValue(CConfigHandler::CONFIG_CTP, "trade_front");
    QString server_simnow = cfg.getValue(CConfigHandler::CONFIG_SIMNOW, "trade_front");

    serverListBox->addItem(server_ctp);
    serverListBox->addItem(server_simnow);
}

CLoginDialog::~CLoginDialog()
{
    delete ui;
}

void CLoginDialog::on_buttonBox_clicked(QAbstractButton *button)
{
}

void CLoginDialog::on_buttonBox_accepted()
{
    
}

void CLoginDialog::on_comboBox_activated(const QString &arg1)
{

}
