#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushConnect_clicked()
{
    qDebug() << "Attempting to connect to" << ui->ip->text();
    ui->pushConnect->setText("Attempting to connect...");
    ui->pushConnect->setStyleSheet("background-color: yellow");
    qApp->processEvents();

    if(client.doConnect(ui->ip->text(), ui->port->text().toInt())>=0)
    {
        ui->pushConnect->setStyleSheet("background-color: green");
        ui->pushConnect->setText("Connected!");
        ui->send->setEnabled(true);
    } else {
        ui->pushConnect->setStyleSheet("background-color: red");
        ui->pushConnect->setText("Connect failed.");
    }
}

void MainWindow::on_pushInitOculus_clicked()
{
    qDebug() << "Attempting to init occulus" << ui->ip->text();
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
    client.send(ui->sendMessage->text());
}
