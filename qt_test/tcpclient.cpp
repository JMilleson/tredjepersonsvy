#include "tcpclient.h"

TcpClient::TcpClient(QObject *parent) : QObject(parent)
{
    connect(socket, SIGNAL(connected()),this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
    connect(socket, SIGNAL(bytesWritten(qint64)),this, SLOT(bytesWritten(qint64)));
    connect(socket, SIGNAL(readyRead()),this, SLOT(readyRead()));

}

int TcpClient::doConnect(QString ip, quint16 port){
    return this->doConnect(ip,port,getDefaultTimeoutMS());
}

int TcpClient::doConnect(QString ip, quint16 port, int timeOutMS)
{
    socket->connectToHost(ip,port);

    if(socket->waitForConnected(timeOutMS))
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


int TcpClient::doConnectASYNC(QString ip, quint16 port){
    return this->doConnectASYNC(ip,port,getDefaultTimeoutMS());
}

int TcpClient::doConnectASYNC(QString ip, quint16 port, int timeOutMs){

    QThread* thread = new QThread();
    ConnectWorker* worker = new ConnectWorker(socket, timeOutMs);
    worker->moveToThread(thread);
    connect(thread, SIGNAL(started()), worker, SLOT(process()));
    connect(worker, SIGNAL(finished()), thread, SLOT(quit()));
    //connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
    //connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    socket->connectToHost(ip,port);
    thread->start();
    return 0;
}

int TcpClient::send(QString s){
    if(socket->state() == socket->ConnectedState){
        if(socket->write(s.toStdString().c_str()) &&  socket->waitForBytesWritten(10000)){
            return 0;
        } else {
            return -2;
        }

    } else {
        qDebug() << "Not connected!";
        return -1;
    }
}

int TcpClient::disconnectFromHost()
{
    socket->disconnectFromHost();
    if (socket->state() == QAbstractSocket::UnconnectedState ||
        socket->waitForDisconnected(1000)){
        qDebug("Disconnected!");
        return 0;
    } else {
        return -1;
    }
}

void TcpClient::connected()
{
    qDebug() << "connected...";

    // Hey server, tell me about you.
    socket->write("hej martin");
}

void TcpClient::disconnected()
{
    qDebug() << "disconnected...";
}

void TcpClient::bytesWritten(qint64 bytes)
{
    qDebug() << bytes << " bytes written...";
}

void TcpClient::readyRead()
{
    qDebug() << "reading...";

    // read the data from the socket
    qDebug() << socket->readAll();
}
