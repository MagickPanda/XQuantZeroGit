#include "cconsoleform.h"
#include "ui_cconsoleform.h"

CConsoleForm::CConsoleForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CConsoleForm)
{
    ui->setupUi(this);

    ui->label->setText("Fuck you");
}

CConsoleForm::~CConsoleForm()
{
    delete ui;
}

