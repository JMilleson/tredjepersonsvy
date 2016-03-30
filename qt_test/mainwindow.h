#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "tcpclient.h"
#include "occulussensorcontroller.h"

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

private:
    Ui::MainWindow *ui;
    TcpClient client;
    OcculusSensorController oculus;
};

#endif // MAINWINDOW_H
