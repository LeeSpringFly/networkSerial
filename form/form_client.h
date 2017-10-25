#ifndef FORM_CLIENT_H
#define FORM_CLIENT_H

#include <QWidget>
#include <QTcpSocket>
#include <QHostAddress>
#include <QTimer>

namespace Ui {
class Form_Client;
}

class Form_Client : public QWidget
{
    Q_OBJECT

public:
    explicit Form_Client(QWidget *parent = 0);
    ~Form_Client();

public slots:
    void connecting();
    void close();

    void slotSend();
    void slotConnected();
    void slotConnectToHostError();
    void dataReceived();
    void slotShowHexOut(bool);
    void slotClearBufferIn();
    void slotAutoSend(bool);

private:
    Ui::Form_Client *ui;

    QTcpSocket *tcpClient;
    QHostAddress *serverIP;
    QTimer *timer;

    void showUI(bool);
    void initAddress();
};

#endif // FORM_CLIENT_H
