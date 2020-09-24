#include "mytcpsocket.h"
#include <QDebug>
#include <QThread>
#include <QElapsedTimer>

#define cout qDebug() << "[" << __FILE__ << ":" << __LINE__ <<"]"

mytcpsocket::mytcpsocket(qintptr socketdesc ,QTcpSocket *parent) : QTcpSocket(parent)
{
    //初始化
    this->setSocketDescriptor(socketdesc);
    //信号和槽函数的参数要一致;重名的error很多,特指QAbstractSocket::error
    connect(this, static_cast<void (QTcpSocket::*)(QAbstractSocket::SocketError)>(&QTcpSocket::error), this,&mytcpsocket::SocketErr);
}

void mytcpsocket::SocketErr(QAbstractSocket::SocketError socketError)
{
    mytcpsocket* Socket = static_cast<mytcpsocket*>(sender());
    cout << Socket->peerName() << socketError;
}

void mytcpsocket::ReadAndParseData()
{
    /*
     * 获取信号发射者->对应的通讯套接字
     * 接收字段,拆包分析包头关键字,Sort排序,Get获取
     * 返回,包头SortOK排序完成,Data返回数据
     * 以#拆包,超界无效
     */
    mytcpsocket *Socket = static_cast<mytcpsocket*>(sender());
    QString Recv = Socket->readAll();
    cout << QThread::currentThread();
    static QVector<int> dataList;
    static int NumCount = 0;
    QString Flag = (Recv.section('#',0,0));
    if(Flag == "Sort")
    {
        QElapsedTimer Timer;
        Timer.start();
        NumCount = (Recv.section('#',1,1)).toInt();
        dataList.resize(NumCount);
        for(int i = 0;i < NumCount  ;i++)
        {
            //cout << (Recv.section('#',i+2,i+2)).toInt();
            //cout << i;
            //从0开始存,100个,即0~99
            dataList.replace(i,(Recv.section('#',i+2,i+2)).toInt());
        }
        //升序
        /*
         *降序
         *std::sort(dataList.begin(),dataList.end(),std::greater<int>());
         *cout<<dataList;
         */
        std::sort(dataList.begin(),dataList.end());        
        Socket->write("SortOK#" + QString::number(Timer.elapsed()).toUtf8());
    }else if(Flag == "Get")
    {
        QString Serial = (Recv.section('#',1,1));
        cout << Serial;
        cout << NumCount;
        if(Serial == "Max")
        {
            Socket->write("Data#" + QString::number(dataList.at(NumCount-1)).toUtf8());
        }else if(Serial == "Min")
        {
            Socket->write("Data#" + QString::number(dataList.at(0)).toUtf8());
        }else
        {
            //正则替换,去掉"英文字母"
            int i = Serial.replace(QRegExp("[a-zA-Z]+"), "").toInt();
            if(i > NumCount-1 || i < 0)
            {
                //超界
                cout << "超界";
                return;
            }else
            {
                Socket->write("Data#" + QString::number(dataList.at(i-1)).toUtf8());
            }
        }
    }
}
