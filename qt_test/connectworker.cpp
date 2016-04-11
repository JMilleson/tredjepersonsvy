#include "connectworker.h"

ConnectWorker::ConnectWorker(QTcpSocket * socket, int timeoutms)
{
    this->socket = socket;
    this->timeoutms = timeoutms;
}

void ConnectWorker::process()
{
    //connect here
    socket->waitForConnected(timeoutms);
    emit finished();
}
