#include "tcpclient.h"

TcpClient::TcpClient(QObject *parent) : QObject(parent)
{

}

int TcpClient::doConnect(QString ip, quint16 port){
    return this->doConnect(ip,port,getDefaultTimeoutMS());
}

int TcpClient::doConnect(QString ip, quint16 port, int timeOutMs){
    this->socket = new QTcpSocket(this);
    socket->connectToHost(ip,port);

    if(socket->waitForConnected(timeOutMs))
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

int TcpClient::send(QString s){
    if(socket->state() == socket->ConnectedState){
        if(socket->write(s.toStdString().c_str()) &&  socket->waitForBytesWritten(1000)){
            qDebug() << "Wrote: " +s +" to socket";
            return 0;
        } else {
            return -2;
        }

    } else {
        qDebug() << "Not connected!";
        return -1;
    }
}
