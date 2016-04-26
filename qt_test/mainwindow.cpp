#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);

    connect(timer, SIGNAL(timeout()), this, SLOT(sendSensorData()));
    connect(this->requestSensorDataTimer, SIGNAL(timeout()), this, SLOT(requestSensorData()));
    connect(this->graphTimer, SIGNAL(timeout()), this, SLOT(updateGraph()));
    connect(client->getSocket(), SIGNAL(connected()),this, SLOT(connected()));
    connect(client->getSocket(), SIGNAL(disconnected()),this, SLOT(disconnected()));
    connect(client,SIGNAL(notifySentData(QString)),this,SLOT(sentData(QString)));
    connect(client,SIGNAL(notifyReceivedData(QString)),this,SLOT(receivedData(QString)));

    this->applicationRuntimeTimer->start();

//    this->receivedData(QString("{\"sensordata\": {\"distance\": 0, \"height\": 10, \"pitch\": 0,\"roll\": 0, \"uavangle\": 0,   \"yaw\": 0 } }"));
//    this->receivedData(QString("{\"sensordata\": {\"distance\": 0, \"height\": 30, \"pitch\": 0,\"roll\": 0, \"uavangle\": 0,   \"yaw\": 0 } }"));
//    this->receivedData(QString("{\"sensordata\": {\"distance\": 0, \"height\": 100, \"pitch\": 0,\"roll\": 0, \"uavangle\": 0,   \"yaw\": 0 } }"));
//    this->receivedData(QString("{\"sensordata\": {\"distance\": 0, \"height\": 150, \"pitch\": 0,\"roll\": 0, \"uavangle\": 0,   \"yaw\": 0 } }"));
//    this->receivedData(QString("{\"sensordata\": {\"distance\": 0, \"height\": 200, \"pitch\": 0,\"roll\": 0, \"uavangle\": 0,   \"yaw\": 0 } }"));

    this->graph();
    this->on_pushGraph1_clicked();
    //this->on_initSerial_clicked();
    //this->on_sendSignal_clicked();
    //connect(serCom,SIGNAL( serialDataConfirmed()),this,SLOT(receivedSerialConfirmation()));



}

MainWindow::~MainWindow()
{
    delete ui;
}


/*void MainWindow::on_initSerial_clicked(){
    if(serCom->init()){
        ui->initSerial->setStyleSheet("background-color: green");
        ui->initSerial->setText("initzialised");
    } else {
        ui->initSerial->setStyleSheet("background-color: red");
        ui->initSerial->setText("failed");
    }
}*/

/*void MainWindow::on_sendSignal_clicked(){
    this->serialTimer.restart();
    serCom->sendSignal();
}*/

void MainWindow::graph(){
    ui->graph1->addGraph();
    ui->graph1->xAxis->setLabel("time");
    ui->graph1->yAxis->setLabel("height");
    ui->graph1->graph(0)->setLineStyle(QCPGraph::LineStyle::lsLine);

    ui->graph2->addGraph();
    ui->graph2->xAxis->setLabel("x (uavangle)");
    ui->graph2->yAxis->setLabel("distance to user");
    ui->graph2->graph(0)->setLineStyle(QCPGraph::LineStyle::lsNone);
    ui->graph2->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDiamond, 10));
}

void MainWindow::updateGraph(){
    //qDebug() << "graphing stuff";
    ui->graph1->graph(0)->setData(this->heightData.first, this->heightData.second);
    double first = this->heightData.first.first();
    double last = this->heightData.first.last();
    ui->graph1->xAxis->setRange(last ,first+1);
    ui->graph1->yAxis->setRange(0, this->hMax+5);
    ui->graph1->replot();
    //qDebug() << this->heightData.first << this->heightData.second;


    ui->graph2->graph(0)->setData(this->uavangleData.second, this->distanceData.second);
    ui->graph2->xAxis->setRange(-50,50);
    ui->graph2->yAxis->setRange(0, 600);
    ui->graph2->replot();
}

void MainWindow::on_pushGraph1_clicked(){
    this->graphTimer->stop();
    this->graphTimer->start(ui->refreshRate1->text().toInt());
}

void MainWindow::requestSensorData(){
    if(client->isConnected()){
        QJsonObject requestSensorData = {
            {"requestSensorData", ""}
        };
        QJsonDocument data(requestSensorData);
        client->send(data.toJson());
    } else {
        this->stopRequestData();
    }
}

void MainWindow::on_requestSensorData_clicked(){
    if(!ui->requestSensorData->isChecked()){
        ui->requestSensorData->setChecked(true);
    } else {
        ui->requestSensorData->setChecked(false);
    }

    requestSensorData();
    /*if(!this->requestSensorDataTimer->isActive())
        if(client->isConnected())
            this->startRequestData();
        else qDebug() << "not connected" ;
    else
        this->stopRequestData();*/
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

void MainWindow::on_stopVideo_clicked(){
    QJsonObject stopVideo = {
        {"abortVideo", ""}
    };

    QJsonDocument data(stopVideo);
    client->send(data.toJson());
}

void MainWindow::on_viewVideo_clicked(){
    //qDebug() << "attempting to open sink at: " << QDir::currentPath() << "  "<< qApp->applicationDirPath()
    videoProcess.setWorkingDirectory(QDir::currentPath());
    videoProcess.start("cmd.exe" , QStringList() << "\\test.bat");
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

void MainWindow::on_pushCalibrateSensors_clicked(){
    QJsonObject sendData = {
        {"calibrateSensors", ""}
    };

    QJsonDocument data(sendData);
    client->send(data.toJson());

    QTimer::singleShot(2000, this, SLOT(enableSendSensors()));
}

void MainWindow::enableSendSensors(){
    ui->pushSendSensorData->setEnabled(true);

    ui->pushCalibrateSensors->setText("Probably calibrated......");
    ui->pushCalibrateSensors->setStyleSheet("background-color: yellow");
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

/*void MainWindow::
 * dSerialConfirmation(){
    qDebug() << this->serialTimer.nsecsElapsed();
    QJsonObject sendData = {
        {"sensorConfirmation", 0}
    };

    QJsonDocument data(sendData);
    client->send(data.toJson());
}*/

void MainWindow::sentData(QString s){
    if(!ui->pauseSendLog->isChecked())
        ui->networkSendLog->setText(QTime::currentTime().toString()+"-> "+s+'\n'+ui->networkSendLog->toPlainText());
}

void MainWindow::receivedData(QString s){
    /*if(s.contains("pingme")){
        this->serialTimer.start();
        serCom->sendSignal();
        return;
    }*/
    try {
        QJsonDocument d = QJsonDocument::fromJson(s.toUtf8());
        //qDebug() << d;
        //qDebug() << d.object() << d.object().contains("") << d.object().keys();


        QJsonObject robj = d.object();

        QJsonObject::iterator itr = robj.find("sensordata");
        if(itr != robj.end()){

            double time = this->applicationRuntimeTimer->elapsed()/1000;


            QJsonObject obj = itr.value().toObject();

            //qDebug () <<"obj: "<< obj;

            QJsonValue val = obj.value("height");
            double dVal = val.toString().toDouble();

            //qDebug () <<"height: "<< val << "doubl: " << dVal;
            if ( heightData.first.size()>50){
                heightData.first.removeLast();
                double last = heightData.second.last();
                heightData.second.removeLast();
                if (last == hMax){
                    double t  = 0;
                    for(double d : heightData.second){
                        t = std::max(t,d);
                    }
                    hMax = t;
                }
            }
            this->heightData.second.push_front(dVal);
            this->heightData.first.push_front(time);
            this->hMax = std::max(this->hMax, dVal);

            val = obj.value("distance");
            dVal = val.toString().toDouble();
            if ( distanceData.first.size()>5){
                distanceData.first.removeLast();
                distanceData.second.removeLast();
            }
            this->distanceData.second.push_front(dVal+300);
            this->distanceData.first.push_front(time);
            this->dMax = std::max(this->dMax, dVal+300);


            if ( uavangleData.first.size()>5){
                uavangleData.first.removeLast();
                uavangleData.second.removeLast();
            }
            val = obj.value("uavangle");
            dVal = val.toString().toDouble();
            this->uavangleData.second.push_front(dVal);
            this->uavangleData.first.push_front(time);
        }


        //QJsonObject sensordata = data.find("sensordata");
        //qDebug()<<sensordata.find("height");
        //this->heightData.push_back("height");
        //if(d.object().contains("sensordata")){
        //    qDebug() << "sensor data rec.: " << s;
        //ui->sensorDataPi->setText(d.toJson(QJsonDocument::Indented));
        return;
        //}
    } catch (...){
        qDebug() << "error while parsing json ....";
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

void MainWindow::stopRequestData()
{
    ui->requestSensorData->setStyleSheet("background-color: grey");
    ui->requestSensorData->setText("Request data");
    qDebug() << "Stop requesting sensor data";
    this->requestSensorDataTimer->stop();
}

void MainWindow::startRequestData()
{
    ui->requestSensorData->setStyleSheet("background-color: green");
    ui->requestSensorData->setText("Stop request");
    qDebug() << "Requesting sensor data with the interval of " << ui->requestSensorDataIntervall->text();
    this->requestSensorDataTimer->start(ui->requestSensorDataIntervall->text().toInt());
}
