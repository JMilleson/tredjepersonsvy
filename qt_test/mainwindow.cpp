#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(sendSensorData()));

    connect(client->getSocket(), SIGNAL(connected()),this, SLOT(connected()));
    connect(client->getSocket(), SIGNAL(disconnected()),this, SLOT(disconnected()));
    connect(client,SIGNAL(notifySentData(QString)),this,SLOT(sentData(QString)));
    connect(client,SIGNAL(notifyReceivedData(QString)),this,SLOT(receivedData(QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushSendSettings_clicked(){
    QJsonObject settings
    {
        {"trottlep", ui->trottleP->text()},
        {"trottlei", ui->trottleI->text()},
        {"trottled", ui->trottleD->text()},
        {"yawp", ui->yawP->text()},
        {"yawi", ui->yawI->text()},
        {"yawd", ui->yawD->text()},
        {"rollp", ui->rollP->text()},
        {"rolli", ui->rollI->text()},
        {"rolld", ui->rollD->text()},
        {"pitchp", ui->pitchP->text()},
        {"pitchi", ui->pitchI->text()},
        {"pitchd", ui->pitchD->text()},
        {"targetHeight", ui->targetHeight->text()},
        {"targetDistance", ui->targetDistance->text()}
    };

    QJsonDocument jsonDoc = QJsonDocument(settings);
    client->send(jsonDoc.toJson());
}

void MainWindow::on_pushGetSensorData_clicked(){
    qDebug() << "Attempting to get sensor data...";
    oculus.updateTracking();
    QJsonObject data =oculus.getSensorDataAsJSON();
    QJsonDocument jsonDoc = QJsonDocument(data);
    ui->sensorOutput->setText(jsonDoc.toJson());
}

void MainWindow::on_pushSendSensorData_clicked()
{
    if(!timer->isActive())
        if(client->isConnected())
            this->startSendData();
        else qDebug() << "not connected" ;
    else
        this->stopSendData();
}

void MainWindow::on_pushConnect_clicked()
{
    if(client->isConnected()){
        qDebug() << "Disconencting from " << ui->ip->text();
        if (client->disconnectFromHost() >= 0){
            ui->pushConnect->setText("Connect");
            ui->pushConnect->setStyleSheet("background-color: gray");
        }
    } else {
        qDebug() << "Attempting to connect to" << ui->ip->text();
        ui->pushConnect->setText("Attempting to connect...");
        ui->pushConnect->setStyleSheet("background-color: yellow");
        qApp->processEvents();

        connectSocket();
        //client.doConnectASYNC(ui->ip->text(), ui->port->text().toInt());
    }
}

void MainWindow::on_pushInitOculus_clicked()
{
    qDebug() << "Attempting to init occulus";
    if(oculus.init()>=0)
    {
        ui->pushInitOculus->setStyleSheet("background-color: green");
        ui->pushInitOculus->setText("Oculus initiated!");
    } else {
        ui->pushInitOculus->setStyleSheet("background-color: red");
        ui->pushInitOculus->setText("Oculus initiation failed.");
    }
    qApp->processEvents();
}

void MainWindow::on_send_clicked()
{
    qDebug() << "Sending message";
    client->send(ui->sendMessage->text());
}

void MainWindow::sendSensorData()
{
    qDebug() << "Sending sensor data";
    oculus.updateTracking();

    QJsonObject data =oculus.getSensorDataAsJSON();
    QJsonDocument jsonDoc = QJsonDocument(data);
    client->send(jsonDoc.toJson());
}

void MainWindow::sentData(QString s){
    if(!ui->pauseSendLog->isChecked())
        ui->networkSendLog->setText(QTime::currentTime().toString()+"-> "+s+'\n'+ui->networkSendLog->toPlainText());
}


void MainWindow::receivedData(QString s){
    if(!ui->pauseSendLog->isChecked())
        ui->networkReceiveLog->setText(QTime::currentTime().toString()+"-> "+s+'\n'+ui->networkReceiveLog->toPlainText());
}


void MainWindow::connected()
{
    ui->pushConnect->setStyleSheet("background-color: green");
    ui->pushConnect->setText("Disconnect");
    ui->send->setEnabled(true);
}

void MainWindow::disconnected()
{
    ui->pushConnect->setStyleSheet("background-color: grey");
    ui->pushConnect->setText("Connect");
    ui->send->setEnabled(false);
    this->stopSendData();
}

void MainWindow::connectSocket()
{
    if(client->doConnect(ui->ip->text(), ui->port->text().toInt())<0){
        ui->pushConnect->setStyleSheet("background-color: red");
        ui->pushConnect->setText("Connect failed.");
    }
}

void MainWindow::stopSendData()
{
    ui->pushSendSensorData->setStyleSheet("background-color: grey");
    ui->pushSendSensorData->setText("Send sensor data");
    qDebug() << "Stop sending sensor data";
    timer->stop();
}

void MainWindow::startSendData()
{
    ui->pushSendSensorData->setStyleSheet("background-color: green");
    ui->pushSendSensorData->setText("Stop sending data");
    qDebug() << "Sending sensor data with the interval of " << ui->interval->text();
    timer->start(ui->interval->text().toInt());
}
