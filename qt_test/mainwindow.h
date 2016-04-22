#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtDebug>
#include <QTimer>
#include <QTime>
#include <QProcess>
#include <QJsonDocument>
#include <QJsonObject>
#include <QElapsedTimer>
#include "tcpclient.h"
#include "occulussensorcontroller.h"
#include "serialcommunication.h"

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

    void on_pushCalibrateSensors_clicked();

    void on_send_clicked();

    void on_pushSendSettings_clicked();

    void on_requestVideo_clicked();

    void on_viewVideo_clicked();

    void on_stopVideo_clicked();

    //void on_initSerial_clicked();
    //void on_sendSignal_clicked();

    void on_requestSensorData_clicked();

    void sendSensorData();
    void requestSensorData();
    void connected();
    void disconnected();
    void connectSocket();
    void sentData(QString s);
    void receivedData(QString s);
    void enableSendSensors();
    //void receivedSerialConfirmation();

private:
    Ui::MainWindow *ui;
    TcpClient * client = new  TcpClient();
    OcculusSensorController oculus;
    QTimer *timer = new QTimer(this);
    QTimer * requestSensorDataTimer = new QTimer(this);
    SerialCommunication * serCom = new SerialCommunication();
    void stopSendData();
    void startSendData();
    void startRequestData();
    void stopRequestData();

    QProcess videoProcess;
    //QElapsedTimer serialTimer ;
};

#endif // MAINWINDOW_H
