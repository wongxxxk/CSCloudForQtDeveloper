#include "mytcpserver.h"
#include "mytcpsocket.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include <QThread>

#define cout qDebug() << "[" << __FILE__ << ":" << __LINE__ <<"]"

mytcpserver::mytcpserver(const std::string &ip, quint16 port,QWidget *parent) : QTcpServer(parent)
{
    if(ip.empty())
    {
        this->listen(QHostAddress::LocalHost, port);
    }
    else
    {
        this->listen(QHostAddress(ip.c_str()), port);
    }
}

//传进来一个(socketDescriptor)套接字描述符
void mytcpserver::incomingConnection(qintptr socketDescriptor)
{
    mytcpsocket *Socket = new mytcpsocket(socketDescriptor);
    m_socketList.append(Socket);

    //为当前套接字注册Connect,每当readyRead执行处理槽函数
    connect(Socket, &mytcpsocket::readyRead, Socket, &mytcpsocket::ReadAndParseData);

    //为当前套接字new一个线程
    QThread *Thread = new QThread;

    //为当前套接字注册Connect,连接中断,线程退出
    connect(Socket, &mytcpsocket::disconnected, Thread, &QThread::quit);

    //为当前套接字注册Connect,线程停止,注销该套接字
    connect(Thread,&QThread::finished,Socket,&mytcpsocket::deleteLater);

    //mytcpsocket移入子线程
    Socket->moveToThread(Thread);
    Thread->start();
    cout << "新连接";
}
