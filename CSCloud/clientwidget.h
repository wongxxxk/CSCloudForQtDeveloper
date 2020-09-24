#ifndef CLIENTWIDGET_H
#define CLIENTWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
    class QTcpSocket;
    class QLabel;
    class QLineEdit;
    class QFileDialog;
    class QPushButton;
    class QHBoxLayout;
    class QVBoxLayout;
    class QTextEdit;
    class QComboBox;
    class QElapsedTimer;
QT_END_NAMESPACE

class ClientWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ClientWidget(QWidget *parent = nullptr);

private:
    void CreateHBoxLayout1();
    void CreateHBoxLayout2();
    void CreateHBoxLayout3();
    void CreateHBoxLayout4();

    QTcpSocket  *TcpSocket;
    QLabel      *LB_Title;
    QLabel      *LB_Operator;
    QFileDialog *FileDialog;
    QPushButton *PB_GetFilePath;
    QPushButton *PB_Sort;
    QPushButton *PB_GetResult;
    QHBoxLayout *HBoxLayout1;
    QHBoxLayout *HBoxLayout2;
    QHBoxLayout *HBoxLayout3;
    QHBoxLayout *HBoxLayout4;
    QVBoxLayout *VBoxLayout;
    QTextEdit   *TE_ShowTimeCount;
    QComboBox   *CB_Box;
    QTextEdit   *TE_ShowOperate;
    QString     FilePath;
    int         SendTimeCount = 0;
    int         Count = 0;//数据总数
    QElapsedTimer *Timer1;
signals:

public slots:
    void GetFileInfo();
    void ConnectServer();
    void FileOperation();
    void TimeStop();
    void GetDataSend();
    void GetServerResult();
    void DisConnectedDeal();
};

#endif // CLIENTWIDGET_H
