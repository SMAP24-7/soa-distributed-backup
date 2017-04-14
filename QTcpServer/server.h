#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTextStream>

class QTcpSocket;

class LocalServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit LocalServer(QObject *parent = 0);
    ~LocalServer();
    void envia(const QString &msj);
signals:

public slots:

private:
    QTcpSocket *mSocket;
};

#endif // SERVER_H
