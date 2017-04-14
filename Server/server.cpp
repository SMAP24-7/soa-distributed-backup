#include "server.h"

#include <QTcpSocket>


LocalServer::LocalServer(QObject *parent) : QTcpServer(parent)
{
    mSocket = nullptr;
    sSocket = nullptr;

    qRegisterMetaType<QString>("QString");
    connect(this,SIGNAL(newConnection()),this,SLOT(nueva_Conexion()));
}

LocalServer::~LocalServer()
{
}

void LocalServer::leer()
{
    if (mSocket->readBufferSize() > mSocket->bytesAvailable()){
        QDataStream read(mSocket->read(mSocket->bytesAvailable()));
        qDebug() << sizeof(read) << " bytes a leer";
        quint64 bytesize;
        QString dir;
        int num = 0;
        QString name;
        QByteArray contain;

        read >> bytesize;
        read >> dir;
        read >> num;

        if (num > 0){
            read >> name;
            read >> contain;
        }
        byte_read_ += bytesize;
        if (bytesize > 0){
            for (int q  = 0; q < list_.size(); q++){
                QTcpSocket *cSocket = list_[q];

                QByteArray client;
                QDataStream out(&client, QIODevice::WriteOnly);

                out << bytesize;
                out << dir;
                out << num;
                if (num > 0){
                    out << name;
                    out << contain;
                }
                cSocket->write(client);
                cSocket->waitForReadyRead(300);
                cSocket->flush();
            }
        }
    }
}

void LocalServer::nueva_Conexion()
{
    mSocket = nextPendingConnection();
    if (!mSocket->waitForReadyRead(1000)){
        list_.append(mSocket);
    }
    else{
        envia();
    }
}

void LocalServer::envia()
{
    if (mSocket){
        mSocket->setReadBufferSize(Buffer_);
        QDataStream read(mSocket->read(mSocket->bytesAvailable()));
        quint64 bytesize;
        int num;
        quint64 size;

        read >> bytesize;
        read >> num;
        read >> size;
        qDebug() << bytesize;
        qDebug() << num;
        size_ = size;

        if(num <= list_.size()){
            num = list_.size();
            qDebug() << "hay clientes";
            QByteArray num_client;
            QDataStream out(&num_client, QIODevice::WriteOnly);
            out << (quint64)0;
            out << num;
            out.device()->seek(0);

            out << quint64((2*sizeof(quint64) - sizeof(quint64)));

            mSocket->write(num_client);
            mSocket->flush();

            for (int q  = 0; q < list_.size(); q++){
                QTcpSocket *cSocket = list_[q];

                QByteArray client;
                QDataStream out_clients(&client, QIODevice::WriteOnly);
                quint64 byte_signal = 1;

                out_clients << byte_signal;
                out_clients << size;

                cSocket->write(client);
                cSocket->waitForReadyRead(30);
                cSocket->flush();
            }

            connect(mSocket,SIGNAL(readyRead()),this,SLOT(leer()));
        }
        else{
            qDebug() << "no hay clientes";
        }
    }
}
