#include "clientwidget.h"
#include <QLabel>
#include <QFileDialog>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QComboBox>
#include <QTcpSocket>
#include <QFile>
#include <QTextStream>
#include <QDataStream>
#include <QTimer>
#include <QThread>
#include <QDebug>
#include <QElapsedTimer>

#define cout qDebug() << "[" << __FILE__ << ":" << __LINE__ << "]"

ClientWidget::ClientWidget(QWidget *parent) : QWidget(parent)
{
    //如果再this->SetLayout 会警告
    //Attempting to add QLayout "" to ClientWidget "", which already has a layout
    VBoxLayout = new QVBoxLayout(this);//指定父对象 == setLayout

    //第一行
    CreateHBoxLayout1();

    //第二行
    CreateHBoxLayout2();

    //第三行
    CreateHBoxLayout3();

    //第四行
    CreateHBoxLayout4();

    VBoxLayout->addItem(HBoxLayout1);
    VBoxLayout->addItem(HBoxLayout2);
    VBoxLayout->addItem(HBoxLayout3);
    VBoxLayout->addItem(HBoxLayout4);

    TcpSocket = new QTcpSocket(this);

    //计时器
    Timer1 = new QElapsedTimer;

    connect(PB_GetFilePath,&QPushButton::clicked,this,&ClientWidget::GetFileInfo);
    connect(TcpSocket,&QTcpSocket::connected,
            [=]()
            {
                //链接上后,Sort有效
                connect(PB_Sort,&QPushButton::clicked,this,&ClientWidget::FileOperation);
            });
    connect(TcpSocket,&QTcpSocket::readyRead,this,&ClientWidget::GetServerResult);
    connect(PB_GetResult,&QPushButton::clicked,this,&ClientWidget::GetDataSend);
    connect(TcpSocket,&QTcpSocket::disconnected,this,&ClientWidget::DisConnectedDeal);
}

void ClientWidget::CreateHBoxLayout1()
{
    HBoxLayout1 = new QHBoxLayout;
    LB_Title = new QLabel("dataset",this);
    LB_Title->setFixedSize(40,40);

    PB_GetFilePath = new QPushButton(tr("获取文件路径"),this);
    PB_GetFilePath->setFixedSize(80,30);

    PB_Sort = new QPushButton(tr("Sort"),this);
    PB_Sort->setFixedSize(60,30);

    HBoxLayout1->addWidget(LB_Title);
    HBoxLayout1->addWidget(PB_GetFilePath);
    HBoxLayout1->addWidget(PB_Sort);
}

void ClientWidget::CreateHBoxLayout2()
{
    HBoxLayout2 =new QHBoxLayout;
    TE_ShowTimeCount = new QTextEdit(this);
    TE_ShowTimeCount->setFixedSize(260,40);

    HBoxLayout2->addWidget(TE_ShowTimeCount);
}

void ClientWidget::CreateHBoxLayout3()
{
    HBoxLayout3 = new QHBoxLayout;
    LB_Operator = new QLabel(tr("Operator"),this);
    CB_Box      = new QComboBox(this);
    PB_GetResult= new QPushButton(tr("Get"),this);

    LB_Operator->setFixedSize(60,40);
    CB_Box->setFixedSize(60,20);
    PB_GetResult->setFixedSize(60,30);
    PB_GetResult->setEnabled(false);

    CB_Box->addItem(tr("Min"));
    CB_Box->addItem(tr("Max"));
    CB_Box->addItem(tr("10th"));
    CB_Box->addItem(tr("20th"));
    CB_Box->addItem(tr("100th"));

    HBoxLayout3->addWidget(LB_Operator);
    HBoxLayout3->addWidget(CB_Box);
    HBoxLayout3->addWidget(PB_GetResult);
}

void ClientWidget::CreateHBoxLayout4()
{
    HBoxLayout4 = new QHBoxLayout;
    TE_ShowOperate = new QTextEdit(this);
    TE_ShowOperate->setFixedSize(260,40);

    HBoxLayout4->addWidget(TE_ShowOperate);
}

void ClientWidget::GetFileInfo()
{
    FilePath = QFileDialog::getOpenFileName(this,tr("Open"),"/","Word(*.txt)");
    if(FilePath.isNull())
    {
        cout << "文件路径获取失败";
        return;
    }
    else
    {
       PB_Sort->setEnabled(true);

       if(TcpSocket->state() == QAbstractSocket::ConnectedState)
       {
           return;
       }else
       {
           emit ClientWidget::ConnectServer();
       }
    }
}

void ClientWidget::ConnectServer()
{
    QString IP = QString("127.0.0.1");
    //TcpSocket->abort();
    //TcpSocket->reset();
    TcpSocket->connectToHost(IP,555);
}

void ClientWidget::FileOperation()
{
    Timer1->restart();

    cout << "连接成功";
    PB_Sort->setEnabled(false);
    QFile File(FilePath);
    cout << FilePath;
    if(File.open(QIODevice::Text | QIODevice::ReadOnly))
    {
        Count = 0;
        QString FileData;
        QString str;
        QTextStream Stream(&File);
        while(!Stream.atEnd())
        {
            Count += 1;
            Stream >> str;
            FileData += str + "#";
        }
        cout << Count;
        //用"#"拆包,第0号表示类型,Sort(排序),第1号是数据总数
        //第0号 == Get(取值),第一号表示有序队列下标
        TcpSocket->write(QString("Sort").toUtf8() + "#" + QString::number(Count).toUtf8() + "#" + FileData.toUtf8());
        emit TimeStop();
        File.close();
    }
    else
    {
        cout << "文件打开失败";
        return;
    }
}

void ClientWidget::GetDataSend()
{
    TcpSocket->write(QString("Get").toUtf8() + "#" + CB_Box->currentText().toUtf8());
}

void ClientWidget::GetServerResult()
{
    QString Recv = TcpSocket->readAll();
    QString Flag = Recv.section("#",0,0);
    cout << Recv;
    cout << Flag;
    //排序OK
    if(Flag == "SortOK")
    {
        PB_GetResult->setEnabled(true);
        TE_ShowTimeCount->append("排序时间(ms):"+Recv.section("#",1,1));
    }else if(Flag == "Data")//Get数据
    {
        TE_ShowOperate->setText(QString::number(Recv.section("#",1,1).toInt()));
        cout << (QString::number(Recv.section("#",1,1).toInt()));
    }else
    {
        cout << "返回值无效";
        return;
    }
}

void ClientWidget::TimeStop()
{
    if(Timer1->isValid())
    {
        TE_ShowTimeCount->setText(QString::number(Timer1->elapsed()) + "ms");
    }
}

void ClientWidget::DisConnectedDeal()
{
    cout << "链接中断";
    PB_GetResult->setEnabled(false);
    PB_Sort->disconnect();
}
