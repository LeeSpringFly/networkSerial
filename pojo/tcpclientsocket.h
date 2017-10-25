#ifndef TCPCLIENTSOCKET_H
#define TCPCLIENTSOCKET_H

#include <QObject>
#include <QTcpSocket>
#include <QByteArray>

class TcpClientSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit TcpClientSocket(QObject *parent = 0);

signals:
    void updateInputBuffer(QString ip, int port, QByteArray btArray);
    void disconnected(qintptr);

public slots:
    void dataReceived();
    void slotDisconnected();
};

#endif // TCPCLIENTSOCKET_H
