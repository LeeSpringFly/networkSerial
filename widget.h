#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QList>
#include <QWidget>
#include <QRadioButton>

#include "form/form_client.h"
#include "form/form_server.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

public slots:
    void showForm();

private:
    Ui::Widget *ui;

    QList<QWidget*> m_formList;
    QList<QRadioButton*> m_rBtnList;

protected:
    void resizeEvent(QResizeEvent *event);
};

#endif // WIDGET_H
