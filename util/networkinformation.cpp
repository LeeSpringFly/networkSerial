#include "networkinformation.h"

#include <QHostInfo>
#include <QDebug>
#include <QAbstractSocket>
#include <QNetworkInterface>

NetWorkInformation::NetWorkInformation()
{

}

QString NetWorkInformation::getHostInfomation()
{
    QList<QHostAddress> addrList = QNetworkInterface::allAddresses();

    for(QHostAddress addr : addrList)
        if(addr.protocol() == QAbstractSocket::IPv4Protocol && !addr.toString().contains("127.0."))
            return addr.toString();

    return "0.0.0.0";
}
