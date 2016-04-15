#ifndef SERIALCOMMUNICATION_H
#define SERIALCOMMUNICATION_H

#include <QObject>
#include <QtSerialPort/QtSerialPort>

class SerialCommunication : public QObject
{
    Q_OBJECT
public:
    explicit SerialCommunication(QObject *parent = 0);
    bool init();
    void sendSignal();

private:
    QSerialPort * port = NULL;

signals:

public slots:
};

#endif // SERIALCOMMUNICATION_H