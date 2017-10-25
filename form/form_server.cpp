#include "form_server.h"
#include "ui_form_server.h"

#include "util/networkinformation.h"

#include <QDateTime>
#include <QStringList>
#include <QTextCodec>
#include <QDebug>

Form_Server::Form_Server(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form_Server)
{
    ui->setupUi(this);

    this->setLayout(ui->layoutBase);

    showUI(false);
    connect(ui->pbtn_listening, SIGNAL(clicked(bool)), this, SLOT(listening()));
    connect(ui->pbtn_close, SIGNAL(clicked(bool)), this, SLOT(close()));
    connect(ui->pbtn_send, SIGNAL(clicked(bool)), this, SLOT(slotSend()));

    connect(ui->chk_showHexOut, SIGNAL(clicked(bool)), this, SLOT(slotShowHexOut(bool)));
    connect(ui->chk_autoSend, SIGNAL(clicked(bool)), this, SLOT(slotAutoSend(bool)));
    connect(ui->pbtn_clearReadBuffer, SIGNAL(clicked(bool)), this, SLOT(slotClearBufferIn()));
}

Form_Server::~Form_Server()
{
    delete ui;
}

void Form_Server::listening()
{
    int port = ui->edt_port->text().toInt();
    server = new TcpServer(this, port);
    connect(server, SIGNAL(updateClientInputBuffer(QString,int,QByteArray)), this, SLOT(updateInputMessage(QString,int,QByteArray)));
    connect(server, SIGNAL(updateClientList()), this, SLOT(updateClientList()));

    showUI(server->isListening());
}

void Form_Server::close()
{
    if(server->isListening())
        server->stopLisening();


    showUI(false);
}

void Form_Server::slotSend()
{
    QString msg = ui->edt_out->toPlainText();
    QByteArray btArray;

    if(ui->chk_showHexOut->isChecked()) {
        QStringList strList = msg.split(" ");
        for(QString str : strList)
            btArray.append(str.toInt(NULL, 16));
    } else {
        QTextCodec *codec = QTextCodec::codecForLocale();
        btArray = codec->fromUnicode(msg);
    }

    int socketIndex = ui->cb_connectedList->currentIndex() - 1;
    if( socketIndex < 0) {
        server->writeAll(btArray);
        return;
    }

    server->write(btArray, socketIndex);

}

void Form_Server::slotAutoSend(bool isAuto)
{
    if(!isAuto) {
        timer->stop();
        return;
    }

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(slotTimeDone()));

    int mill = ui->edt_autoSend->text().toInt();
    timer->start(mill);
}

void Form_Server::slotTimeDone()
{
    emit ui->pbtn_send->click();
}

/**
*   UI
*/
void Form_Server::showUI(bool isListening)
{
    NetWorkInformation *netInfo = new NetWorkInformation;
    ui->edt_ip->setText(netInfo->getHostInfomation());

    ui->edt_ip->setEnabled(!isListening);
    ui->edt_port->setEnabled(!isListening);
    ui->pbtn_listening->setEnabled(!isListening);
    ui->pbtn_close->setEnabled(isListening);

    ui->cb_connectedList->setEnabled(isListening);
    ui->pbtn_send->setEnabled(isListening);

}

void Form_Server::updateClientList()
{
    QStringList addrList;
    addrList.append(trUtf8("All Connections"));

    QString addr;
    QString ip;
    int port;
    for(int i=0; i<server->tcpClientList.size(); i++)
    {
        ip = server->tcpClientList.at(i)->peerAddress().toString();
        port = server->tcpClientList.at(i)->peerPort();
        addr = ip + ":" + QString::number(port);

        addrList.append(addr);
    }

    ui->cb_connectedList->clear();
    ui->cb_connectedList->addItems(addrList);
}

void Form_Server::updateInputMessage(QString ip, int port, QByteArray btArray)
{
    QString addr = ip + ":" + QString::number(port);
    QDateTime cTime = QDateTime::currentDateTime();
    QString strTime = cTime.toString("HH:mm:ss");

    QString message = "【 接受的数据 " + addr + " " + strTime + " 】";
    ui->edt_in->append(message);

    message.clear();
    if(ui->chk_showHexIn->isChecked())
        for(int i=0; i<btArray.size(); i++)
            if(i == 0)
                message += QString::number(btArray.at(i), 16);
            else
                message += " " + QString::number(btArray.at(i), 16);
    else
        message = btArray;

    ui->edt_in->append(message);
}

void Form_Server::slotShowHexOut(bool isChecked)
{
    QString msg = ui->edt_out->toPlainText().trimmed();
    ui->edt_out->setText(msg);

    QTextCodec *codec = QTextCodec::codecForLocale();
    QByteArray btArray;
    if(isChecked) {
        btArray = codec->fromUnicode(msg);
        for(int i=0; i<btArray.size(); i++)
            if(i == 0)
                msg = QString::number(btArray.at(i), 16);
            else
                msg += " " + QString::number(btArray.at(i), 16);
    } else {
        QStringList strList = msg.split(" ");
        for(QString str : strList)
            btArray.append(str.toInt(NULL, 16));
        msg = btArray;
    }

    ui->edt_out->setText(msg);
}

void Form_Server::slotClearBufferIn()
{
    ui->edt_in->clear();
}
