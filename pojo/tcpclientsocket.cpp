#include "tcpclientsocket.h"

#include <QHostAddress>

TcpClientSocket::TcpClientSocket(QObject *parent) : QTcpSocket(parent)
{
    connect(this, SIGNAL(readyRead()), this, SLOT(dataReceived()));
    connect(this, SIGNAL(disconnected()), this, SLOT(slotDisconnected()));
}

void TcpClientSocket::dataReceived()
{
    QString ip = peerAddress().toString();
    int port = peerPort();
    QByteArray btArray;
    while( bytesAvailable() > 0)
        btArray = readAll();

    emit updateInputBuffer(ip, port, btArray);
}

void TcpClientSocket::slotDisconnected()
{
    emit disconnected(this->socketDescriptor());
}
