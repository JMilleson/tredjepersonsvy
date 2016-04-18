#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);

    connect(timer, SIGNAL(timeout()), this, SLOT(sendSensorData()));
    connect(client->getSocket(), SIGNAL(connected()),this, SLOT(connected()));
    connect(client->getSocket(), SIGNAL(disconnected()),this, SLOT(disconnected()));
    connect(client,SIGNAL(notifySentData(QString)),this,SLOT(sentData(QString)));
    connect(client,SIGNAL(notifyReceivedData(QString)),this,SLOT(receivedData(QString)));

    this->on_initSerial_clicked();
    //this->on_sendSignal_clicked();


    connect(serCom,SIGNAL( serialDataConfirmed()),this,SLOT(receivedSerialConfirmation()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_initSerial_clicked(){
    if(serCom->init()){
        ui->initSerial->setStyleSheet("background-color: green");
        ui->initSerial->setText("initzialised");
    } else {
        ui->initSerial->setStyleSheet("background-color: red");
        ui->initSerial->setText("failed");
    }
}

void MainWindow::on_sendSignal_clicked(){
    this->serialTimer.restart();
    serCom->sendSignal();
}

void MainWindow::on_requestVideo_clicked(){
    QJsonObject videoOptions ={
        {"port", ui->videoPort->text()},
        {"timeout",ui->videoTimeout->text()},
        {"width", ui->videoWidth->text()},
        {"height", ui->videoHeight->text()},
        {"bitrate",ui->videoBitrate->text()},
        {"QP",ui->videoQP->text()}
    };
    QJsonObject requestVideo = {
        {"requestVideo", videoOptions}
    };

    QJsonDocument data(requestVideo);
    client->send(data.toJson());
}

void MainWindow::on_viewVideo_clicked(){
    qDebug() << "attempting to open sink";
    QProcess process;
    process.start("gst-launch-1.0", QStringList() << "udpsrc port=1337 ! application/x-rtp,encoding-name=H264,payload=96 ! rtph264depay ! avdec_h264 ! videoconvert ! autovideosink");
}


void MainWindow::on_pushSendSettings_clicked(){
    QJsonObject trottlePid ={
        {"p", ui->trottleP->text()},
        {"i", ui->trottleI->text()},
        {"d", ui->trottleD->text()}
    };

    QJsonObject yawPid ={
        {"p", ui->yawP->text()},
        {"i", ui->yawI->text()},
        {"d", ui->yawD->text()}
    };

    QJsonObject rollPid ={
        {"p", ui->rollP->text()},
        {"i", ui->rollI->text()},
        {"d", ui->rollD->text()}
    };

    QJsonObject pitchPid ={
        {"p", ui->pitchP->text()},
        {"i", ui->pitchI->text()},
        {"d", ui->pitchD->text()}
    };

    QJsonObject target = {
        {"targetHeight", ui->targetHeight->text()},
        {"targetDistance", ui->targetDistance->text()}
    };

    QJsonObject settings = {
        {"trottle", trottlePid},
        {"yaw", yawPid},
        {"roll", rollPid},
        {"pitch", pitchPid},
        {"target", target}
    };

    QJsonObject sendData = {
        {"settings", settings}
    };

    QJsonDocument data(sendData);
    client->send(data.toJson());

    /**/
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

void MainWindow::receivedSerialConfirmation(){
    qDebug() << this->serialTimer.nsecsElapsed();
    QJsonObject sendData = {
        {"sensorConfirmation", 0}
    };

    QJsonDocument data(sendData);
    client->send(data.toJson());
}

void MainWindow::sentData(QString s){
    if(!ui->pauseSendLog->isChecked())
        ui->networkSendLog->setText(QTime::currentTime().toString()+"-> "+s+'\n'+ui->networkSendLog->toPlainText());
}


void MainWindow::receivedData(QString s){
    if(s.contains("pingme")){
        this->serialTimer.start();
        serCom->sendSignal();
        return;
    }
    if(!ui->pauseReceiveLog->isChecked())
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
