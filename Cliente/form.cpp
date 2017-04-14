#include "form.h"
#include "ui_form.h"

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
}

Form::~Form()
{
    delete ui;
}


void Form::on_origen_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),"/home", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    data_.first = "Origen";
    data_.second = dir;
    ui->ok->setEnabled(true);
}

void Form::on_destino_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),"/home", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    data_.first = "Origen";
    data_.second = dir;
    ui->ok->setEnabled(true);
}


void Form::on_ok_clicked()
{
    this->setHidden(true);
}
