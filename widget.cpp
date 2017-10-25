#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->setLayout(ui->layoutBase);

    Form_Server *fServer = new Form_Server(this);
    fServer->setParent(ui->widget_form);
    fServer->resize(fServer->parentWidget()->size());
    fServer->setWindowTitle(trUtf8("TCP 服务器"));
    fServer->hide();
    m_formList.append(fServer);

    Form_Client *fClient = new Form_Client(this);
    fClient->setParent(ui->widget_form);
    fClient->resize(fClient->parentWidget()->size());
    fClient->setWindowTitle(trUtf8("TCP 客户端"));
    fClient->hide();
    m_formList.append(fClient);

    m_rBtnList.append(ui->rBtn_server);
    m_rBtnList.append(ui->rBtn_client);

    connect(ui->rBtn_server, SIGNAL(clicked(bool)), this, SLOT(showForm()));
    connect(ui->rBtn_client, SIGNAL(clicked(bool)), this, SLOT(showForm()));

    emit ui->rBtn_server->click();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::showForm()
{
    QRadioButton *rBtn = (QRadioButton*) sender();
    int index = m_rBtnList.indexOf(rBtn);

    for(int i=0; i<m_formList.size(); i++)
        if(i == index) {
            m_formList.at(i)->show();
            ui->lb_protected->setText(m_formList.at(i)->windowTitle());
        }
        else {
            m_formList.at(i)->hide();
        }
}

void Widget::resizeEvent(QResizeEvent *event)
{
    for(QWidget *item : m_formList)
        item->resize(item->parentWidget()->size());
}
