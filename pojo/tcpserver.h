#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QList>
#include <QByteArray>

#include "pojo/tcpclientsocket.h"

class TcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = 0, int port = 0);
    QList<TcpClientSocket*> tcpClientList;

    void stopLisening();
    void write(QByteArray, int);
    void writeAll(QByteArray);

signals:
    void updateClientInputBuffer(QString ip, int port, QByteArray btArray);
    void updateClientList();


public slots:
    void slotClientInputBuffer(QString ip, int port, QByteArray btArray);
    void slotClientDisconnect(qintptr socketDescriptor);

protected:
    void incomingConnection(qintptr handle);
};

#endif // TCPSERVER_H
