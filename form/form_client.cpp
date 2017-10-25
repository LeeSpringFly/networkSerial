#include "form_client.h"
#include "ui_form_client.h"

#include "util/networkinformation.h"

#include <QTextCodec>
#include <QDateTime>
#include <QByteArray>

Form_Client::Form_Client(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form_Client)
{
    ui->setupUi(this);

    this->setLayout(ui->layoutBase);

    serverIP = new QHostAddress();

    initAddress();
    showUI(false);
    connect(ui->pbtn_connect, SIGNAL(clicked(bool)), this, SLOT(connecting()));
    connect(ui->pbtn_close, SIGNAL(clicked(bool)), this, SLOT(close()));
    connect(ui->pbtn_send, SIGNAL(clicked(bool)), this, SLOT(slotSend()));
    connect(ui->chk_autoSend, SIGNAL(clicked(bool)), this, SLOT(slotAutoSend(bool)));

    connect(ui->chk_showHexOut, SIGNAL(clicked(bool)), this, SLOT(slotShowHexOut(bool)));
    connect(ui->pbtn_clearReadBuffer, SIGNAL(clicked(bool)), this, SLOT(slotClearBufferIn()));
}

Form_Client::~Form_Client()
{
    delete ui;
}

void Form_Client::connecting()
{
    QString ip = ui->edt_ip->text();
    int port = ui->edt_port->text().toInt();

    serverIP->setAddress(ip);
    tcpClient = new QTcpSocket(this);
    connect(tcpClient, SIGNAL(connected()), this, SLOT(slotConnected()));
    connect(tcpClient, SIGNAL(readyRead()), this, SLOT(dataReceived()));
    connect(tcpClient, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(slotConnectToHostError()));

    tcpClient->connectToHost(*serverIP, port);
    showUI(tcpClient->isOpen());

    qDebug() << tcpClient->peerAddress().toString();
    qDebug() << tcpClient->localPort();
    qDebug() << tcpClient->peerPort();

    ui->edt_ip_2->setText(tcpClient->peerAddress().toString());
    ui->edt_port_2->setText(QString::number(tcpClient->peerPort()));
}

void Form_Client::close()
{
    tcpClient->close();
    showUI(false);
}

void Form_Client::slotConnected()
{

}

void Form_Client::slotSend()
{
    QString msg = ui->edt_out->toPlainText().trimmed();
    ui->edt_out->setText(msg);
    QTextCodec *codec = QTextCodec::codecForLocale();
    QByteArray btArray;

    if(ui->chk_showHexOut->isChecked()) {
        QStringList strList = msg.split(" ");
        for(QString str : strList)
            btArray.append(str.toInt(NULL, 16));
    } else {
        btArray = codec->fromUnicode(msg);
    }

    tcpClient->write(btArray);
}

void Form_Client::slotConnectToHostError()
{
    tcpClient->close();
    showUI(false);
}

void Form_Client::dataReceived()
{
    QByteArray btArray = tcpClient->readAll();

    QString msg = "";

    QString ip = tcpClient->peerAddress().toString();
    int port = tcpClient->peerPort();
    QString addr = ip + ":" + QString::number(port);
    QDateTime cTime = QDateTime::currentDateTime();
    msg = "【 接受的数据　" + addr + " " + cTime.toString("HH:mm:ss") + " 】";
    ui->edt_in->append(msg);

    if(ui->chk_showHexIn->isChecked()) {
        for(int i=0; i<btArray.size(); i++)
            if(i == 0)
                msg = QString::number(btArray.at(i) & 0xff, 16);
            else
                msg += " " + QString::number(btArray.at(i) & 0xff, 16);
    } else {
        msg = btArray;
    }

    ui->edt_in->append(msg);
}

void Form_Client::slotAutoSend(bool isAuto)
{
    if(!isAuto) {
        timer->stop();
        return;
    }

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(slotSend()));
    int mill = ui->edt_autoSend->text().toInt();
    timer->start(mill);

}

/**
*   UI
*/
void Form_Client::showUI(bool isListening)
{
    ui->edt_ip->setEnabled(!isListening);
    ui->edt_port->setEnabled(!isListening);
    ui->pbtn_connect->setEnabled(!isListening);
    ui->pbtn_close->setEnabled(isListening);

    ui->pbtn_send->setEnabled(isListening);

}

void Form_Client::initAddress()
{
    NetWorkInformation *netInfo = new NetWorkInformation;
    ui->edt_ip->setText(netInfo->getHostInfomation());
}

void Form_Client::slotShowHexOut(bool isChecked)
{
    QString msg = ui->edt_out->toPlainText();
    QTextCodec *codec = QTextCodec::codecForLocale();

    if(isChecked) {
        QByteArray encoded = codec->fromUnicode(msg);
        for(int i=0; i<encoded.size(); i++)
            if(i == 0)
                msg = QString::number(encoded.at(i) & 0xff, 16);
            else
                msg += " " + QString::number(encoded.at(i) & 0xff, 16);
    } else {
        QStringList msgList = msg.split(" ");
        QByteArray btArray;
        for(QString str : msgList)
            btArray.append(str.toInt(NULL, 16));
        msg = btArray;
    }

    ui->edt_out->setText(msg);

}

void Form_Client::slotClearBufferIn()
{
    ui->edt_in->clear();
}
