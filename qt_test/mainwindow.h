#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtDebug>
#include <QTimer>
#include <QTime>
#include <QProcess>
#include "tcpclient.h"
#include "occulussensorcontroller.h"
#include <QJsonDocument>
#include <QJsonObject>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushConnect_clicked();

    void on_pushInitOculus_clicked();

    void on_pushGetSensorData_clicked();

    void on_pushSendSensorData_clicked();

    void on_send_clicked();

    void on_pushSendSettings_clicked();

    void on_requestVideo_clicked();

    void on_viewVideo_clicked();

    void sendSensorData();
    void connected();
    void disconnected();
    void connectSocket();
    void sentData(QString s);
    void receivedData(QString s);

private:
    Ui::MainWindow *ui;
    TcpClient * client = new  TcpClient();
    OcculusSensorController oculus;
    QTimer *timer = new QTimer(this);
    void stopSendData();
    void startSendData();
};

#endif // MAINWINDOW_H
