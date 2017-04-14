#include "widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    mServer = new LocalServer(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_iniciar_clicked()
{
    if (!mServer->listen(QHostAddress::Any,ui->puerto->value()))
        QMessageBox::critical(this,"Error",mServer->errorString());
    else
        QMessageBox::information(this,"Servidor", "Iniciado");
}

void Widget::on_cerrar_clicked()
{
    close();
}
