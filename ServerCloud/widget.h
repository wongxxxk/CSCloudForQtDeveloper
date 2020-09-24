#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
    class Socketthread;
    class mytcpserver;
    class mytcpsocket;
QT_END_NAMESPACE

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;

    mytcpserver *Tcpserver;

};

#endif // WIDGET_H
