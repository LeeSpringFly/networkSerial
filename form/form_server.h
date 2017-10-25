#ifndef FORM_SERVER_H
#define FORM_SERVER_H

#include <QWidget>
#include <QByteArray>
#include <QTimer>

#include "pojo/tcpserver.h"

namespace Ui {
class Form_Server;
}

class Form_Server : public QWidget
{
    Q_OBJECT

public:
    explicit Form_Server(QWidget *parent = 0);
    ~Form_Server();

    TcpServer *server;

public slots:
    void listening();
    void close();

    void updateInputMessage(QString ip, int port, QByteArray btArray);
    void updateClientList();
    void slotSend();
    void slotShowHexOut(bool);
    void slotAutoSend(bool);
    void slotTimeDone();
    void slotClearBufferIn();

private:
    Ui::Form_Server *ui;

    QTimer *timer;
    void showUI(bool);
};

#endif // FORM_SERVER_H
