#include "tcpserver.h"

#include <QHostAddress>

TcpServer::TcpServer(QObject *parent, int port) : QTcpServer(parent)
{
    listen(QHostAddress::AnyIPv4, port);
}

void TcpServer::incomingConnection(qintptr handle)
{
    TcpClientSocket *tcpClient = new TcpClientSocket(this);
    connect(tcpClient, SIGNAL(updateInputBuffer(QString,int,QByteArray)), this, SLOT(slotClientInputBuffer(QString,int,QByteArray)));
    connect(tcpClient, SIGNAL(disconnected(qintptr)), this, SLOT(slotClientDisconnect(qintptr)));

    tcpClient->setSocketDescriptor(handle);
    tcpClientList.append(tcpClient);

    emit updateClientList();
}

void TcpServer::slotClientInputBuffer(QString ip, int port, QByteArray btArray)
{
    emit updateClientInputBuffer(ip, port, btArray);
}

void TcpServer::slotClientDisconnect(qintptr socketDescriptor)
{
    for(int i=0; i<tcpClientList.size(); i++)
        if(tcpClientList.at(i)->socketDescriptor() == socketDescriptor) {
            tcpClientList.removeAt(i);
            break;
        }

    emit updateClientList();
}

void TcpServer::stopLisening()
{
    for(TcpClientSocket *item : tcpClientList)
        emit item->disconnected(item->socketDescriptor());

    close();
}

void TcpServer::write(QByteArray btArray, int index)
{
    tcpClientList.at(index)->write(btArray);
}

void TcpServer::writeAll(QByteArray btArray)
{
    for(TcpClientSocket *tcpClient : tcpClientList)
        tcpClient->write(btArray);
}
