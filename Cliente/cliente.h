#ifndef CLIENTE_H
#define CLIENTE_H

#include "form.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QPair>
#include <QTimer>
#include <QWidget>


namespace Ui {
class Cliente;
}

class QTcpSocket;

class Cliente : public QWidget
{
    Q_OBJECT

public:
    explicit Cliente(QWidget *parent = 0);
    ~Cliente();

    void get_ruta();

public slots:
    void read_block();

    void send_initial_block();

    void send_all_block();

private slots:
    void on_conectar_clicked();

    void on_cerrar_clicked();

    void on_radio_destino_clicked();

    void on_radio_origen_clicked();

    void on_pushFolder_clicked();

private:
    Ui::Cliente *ui;
    QTcpSocket *mSocket;
    QTcpSocket *cpy_mSocket;

    QPair<QString,QString> data_;

    bool selct_mode;
    QString Msj_completo_;
    quint16 blockSize_;

    quint64 Buffer_ = 1024;
    quint64 size_files_ = 0;
    quint64 sender_block_ = 0;

    void size_block(quint64 &byte, QDir dir);
    void do_block(QString relative_dir, QDir dir);



};

#endif // CLIENTE_H
