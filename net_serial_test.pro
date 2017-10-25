#-------------------------------------------------
#
# Project created by QtCreator 2016-09-09T23:51:47
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = net_serial_test
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    form/form_server.cpp \
    form/form_client.cpp \
    util/networkinformation.cpp \
    pojo/tcpserver.cpp \
    pojo/tcpclientsocket.cpp

HEADERS  += widget.h \
    form/form_server.h \
    form/form_client.h \
    util/networkinformation.h \
    pojo/tcpserver.h \
    pojo/tcpclientsocket.h

FORMS    += widget.ui \
    form/form_server.ui \
    form/form_client.ui

RESOURCES += \
    img/img.qrc

RC_FILE += net_serial.rc
