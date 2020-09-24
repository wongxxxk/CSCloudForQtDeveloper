#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QWidget>
#include "mytcpsocket.h"
#include <QTcpServer>

class mytcpserver : public QTcpServer
{
    Q_OBJECT
public:
    explicit mytcpserver(const std::string &ip, quint16 port,QWidget *parent = nullptr);

signals:


protected:
    //重构该虚函数,有新连接会自动调用
    void incomingConnection(qintptr socketDescriptor);

private:
    //Socket队列,新客户端进入,通讯套接字压入队列,注册一条Connect
    QList<mytcpsocket*> m_socketList;

public slots:
};

#endif // MYTCPSERVER_H
