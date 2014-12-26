#include "addition.h"
#include "ui_addition.h"

addition::addition(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addition)
{
    ui->setupUi(this);
}

addition::~addition()
{
    delete ui;
}
