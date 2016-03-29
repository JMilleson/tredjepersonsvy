#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>

class TcpClient : public QObject
{
    Q_OBJECT
public:
    explicit TcpClient(QObject *parent = 0);
    int doConnect(QString ip, quint16 port);
signals:

public slots:

private:
    QTcpSocket *socket;
};

#endif // TCPCLIENT_H
