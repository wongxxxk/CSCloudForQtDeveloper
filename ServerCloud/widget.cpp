#include "widget.h"
#include "ui_widget.h"
#include "mytcpserver.h"
#include "mytcpsocket.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    mytcpserver* TcpServer;
    TcpServer = new mytcpserver("127.0.0.1",555,this);
}

Widget::~Widget()
{
    delete ui;
}
