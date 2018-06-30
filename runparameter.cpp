#include "runparameter.h"
#include "ui_runparameter.h"

RunParameter::RunParameter(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RunParameter)
{
    ui->setupUi(this);
}

RunParameter::~RunParameter()
{
    delete ui;
}
