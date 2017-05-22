#include "server.h"

#include <QTcpSocket>


LocalServer::LocalServer(QObject *parent) : QTcpServer(parent)
{
    mSocket = nullptr;
    sSocket = nullptr;



    qRegisterMetaType<int>("int");
    connect(this,SIGNAL(newConnection()),this,SLOT(nueva_Conexion()));
    connect(mSocket,SIGNAL(disconnected()),this,SLOT(desconexion()));

    //connect(this,SIGNAL(numero_clientes(int)),widget_,SLOT(numero_clientes(int)));
}
LocalServer::~LocalServer()
{
}

void LocalServer::leer()
{
    if (mSocket->readBufferSize() > mSocket->bytesAvailable()){

        QByteArray read(mSocket->read(mSocket->bytesAvailable()));

            for (int q  = 0; q < list_.size(); q++){
                QTcpSocket *cSocket = list_[q];

                cSocket->write(read,(qint64)read.size());
                cSocket->waitForBytesWritten(3000);
                qDebug() << read.size() << " bytes a enviar a " << list_.size() << " clientes";
                cSocket->flush();
            }
    }
}

void LocalServer::nueva_Conexion()
{
    mSocket = nextPendingConnection();

    mSocket->waitForReadyRead(500);
    if (!mSocket->bytesAvailable()){
        list_.append(mSocket);
        if(origen)
            envia();
    }
    else{
        origen = true;
        oSocket = mSocket;
        envia();
    }
}


void LocalServer::desconexion()
{
    mSocket = nextPendingConnection();
    list_.removeOne(mSocket);
}

void LocalServer::envia()
{
    mSocket = oSocket;
    if (mSocket){
        mSocket->setReadBufferSize(Buffer_);
        QDataStream read(mSocket->read(mSocket->bytesAvailable()));
        int bytesize;
        int num;
        quint64 size;
        QByteArray num_client;
        QDataStream out(&num_client, QIODevice::WriteOnly);

        read >> bytesize;
        read >> num;
        read >> size;
        qDebug() << bytesize;
        qDebug() << num;

        if (num != 0){
            size_ = size;
            num_conexiones_ = num;
            bytesize_ = bytesize;
        }


        if(num_conexiones_ >= list_.size() && list_.size() != 0 && origen){
            num = list_.size();
            qDebug() << "hay clientes";

            out << list_.size();
            out << num;

            mSocket->write(num_client);
            mSocket->flush();

            for (int q  = 0; q < list_.size(); q++){
                QTcpSocket *cSocket = list_[q];

                QByteArray client;
                QDataStream out_clients(&client, QIODevice::WriteOnly);
                quint64 byte_signal = 1;

                out_clients << byte_signal;

                out_clients << size_;

                cSocket->write(client);
                if (!cSocket->bytesToWrite())
                    cSocket->waitForBytesWritten(-1);
                cSocket->flush();
            }

            connect(mSocket,SIGNAL(readyRead()),this,SLOT(leer()));
        }
        else{

            out << list_.size();
            mSocket->write(num_client);
        }
    }
}
