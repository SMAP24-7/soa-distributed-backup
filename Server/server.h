#ifndef SERVER_H
#define SERVER_H

#include "widget.h"

#include <QAbstractSocket>
#include <QDataStream>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QList>
#include <QPair>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTextStream>

class QTcpSocket;
namespace Ui {
class Widget;
}

class LocalServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit LocalServer(QObject *parent = 0);
    ~LocalServer();

signals:

public slots:

    void leer();

    void nueva_Conexion();


    void desconexion();

    void envia();

private:

    Ui::Widget *widget_;

    QTcpSocket *mSocket, *sSocket;
    QTcpSocket *oSocket;
    quint64 Buffer_ = 1024*1024*1024;
    quint64 size_= 0;
    quint64 byte_read_ = 0;
    int num_conexiones_ = 0;
    int bytesize_;
    bool origen = false;

    QPair<QString,QString> data_;

     QList<QTcpSocket*> list_;

     QDataStream read_;
};

#endif // SERVER_H
