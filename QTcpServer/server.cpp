#include "server.h"

#include <QTcpSocket>


LocalServer::LocalServer(QObject *parent) : QTcpServer(parent)
{
    mSocket = nullptr;

    connect(this,&LocalServer::newConnection,[&](){
        mSocket = nextPendingConnection();
    });
}

LocalServer::~LocalServer()
{

}

void LocalServer::envia(const QString &msj)
{
    if (mSocket){
        QTextStream T(mSocket);
        T << msj;
        mSocket->flush();
    }
}
