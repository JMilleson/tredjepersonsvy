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
    int doConnect(QString ip, quint16 port, int timeOutMS);
    int getDefaultTimeoutMS () {return defaultTimeoutMS;};
    int send(QString s);
signals:

public slots:

private:
    QTcpSocket *socket;
    int defaultTimeoutMS = 5000;
};

#endif // TCPCLIENT_H
