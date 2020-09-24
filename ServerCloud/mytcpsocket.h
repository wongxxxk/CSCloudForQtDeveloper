#ifndef MYTCPSOCKET_H
#define MYTCPSOCKET_H

#include <QWidget>
#include <QTcpSocket>

class mytcpsocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit mytcpsocket(qintptr socketdesc,QTcpSocket *parent = nullptr);

signals:

public slots:
    void SocketErr(QAbstractSocket::SocketError socketError);
    void ReadAndParseData();
};

#endif // MYTCPSOCKET_H
