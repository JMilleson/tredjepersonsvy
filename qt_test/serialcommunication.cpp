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
        return port->open(QIODevice::ReadWrite);
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

