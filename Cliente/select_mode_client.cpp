#include "select_mode_client.h"
#include "ui_select_mode_client.h"

select_mode_client::select_mode_client(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::select_mode_client)
{
    ui->setupUi(this);
    ui->Ok->setDisabled(true);
}

select_mode_client::~select_mode_client()
{
    delete ui;
}

void select_mode_client::on_pushButton_clicked()
{
    QString ruta;
    ruta = QFileDialog::getExistingDirectory(this,"Open Directory", "/home", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    data.first = "origen";
    data.second = ruta;
    ui->Ok->setEnabled(true);
}

void select_mode_client::on_pushButton_2_clicked()
{
    QString ruta;
    ruta = QFileDialog::getExistingDirectory(this,"Open Directory", "/home", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    data.first = "destino";
    data.second = ruta;
    ui->Ok->setEnabled(true);
}

void select_mode_client::on_Ok_clicked()
{
    this->close();
}
