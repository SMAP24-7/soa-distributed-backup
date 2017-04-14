#include "select_mode.h"
#include "ui_select_mode.h"

select_mode::select_mode(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::select_mode)
{
    ui->setupUi(this);
}

select_mode::~select_mode()
{
    delete ui;
}
