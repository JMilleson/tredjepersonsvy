#include "serialcommunication.h"

SerialCommunication::SerialCommunication(QObject *parent) : QObject(parent)
{

}

bool SerialCommunication::init(){
    try {
        qDebug () << "init serial ";
        QList<QSerialPortInfo> ports =  QSerialPortInfo::availablePorts();
        for(QSerialPortInfo p : ports){
            QString info = p.description();
            info += p.portName();
            info += p.systemLocation();
            info += p.serialNumber();
            qDebug () <<  info;
        }
        if(ports.size() == 0)
            return 0;
        this->port = new QSerialPort(ports.at(0));
        port->setBaudRate(57600);
        bool succes = port->open(QIODevice::ReadWrite);
        if(!succes)
            return 0;
        connect(port, SIGNAL(readyRead()), this, SLOT(SlotReadyRead()));

        qDebug() << " signal connected";

        return true;
    } catch (std::exception & e){
        qDebug() << "error occured";
        return 0;
    }
    catch (...){
    //something
        return 0;
    }
}
void SerialCommunication::sendSignal(){
    if (port != NULL && port->isWritable()){
        port->write("0");
    }
}

void SerialCommunication::SlotReadyRead() {
    QByteArray ReceivedDat = port->readAll();
    if(0 < ReceivedDat.size()){
        emit serialDataConfirmed();
    }else{
        qDebug() << "No data." << " Timestamp: " << QDateTime::currentMSecsSinceEpoch();
    }
}

