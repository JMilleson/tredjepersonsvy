#include "tcpclient.h"

TcpClient::TcpClient(QObject *parent) : QObject(parent)
{

}

int TcpClient::doConnect(QString ip, quint16 port){
    this->socket = new QTcpSocket(this);
    socket->connectToHost(ip,port);

    if(socket->waitForConnected(5000))
     {
         qDebug() << "Connected!";
         return 0;
     }
     else
     {
         qDebug() << "Not connected!";
         return -1;
     }
}
