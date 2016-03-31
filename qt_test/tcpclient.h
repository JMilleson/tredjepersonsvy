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
    int getDefaultTimeoutMS (){return defaultTimeoutMS;};
    int send(QString s);
    bool isConnected(){return socket->state()==QTcpSocket::ConnectedState;};
    int disconnectFromHost();
    QTcpSocket * getSocket(){return socket;};

signals:

public slots:
    void connected();
    void disconnected();
    void bytesWritten(qint64 bytes);
    void readyRead();

private:
    QTcpSocket *socket = new QTcpSocket();
    int defaultTimeoutMS = 5000;
};

#endif // TCPCLIENT_H
