#ifndef CONNECTWORKER_H
#define CONNECTWORKER_H

#include <QObject>
#include <QTcpSocket>

class ConnectWorker : public QObject
{
    Q_OBJECT
public:
    ConnectWorker(QTcpSocket * socket, int timeoutms);

public slots:
    void process();

signals:
    void finished();
    void error(QString err);
private:
    QTcpSocket * socket;
    int timeoutms;
};

#endif // CONNECTWORKER_H
