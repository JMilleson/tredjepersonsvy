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

void MainWindow::on_pushButton_clicked()
{
    qDebug() << "Attempting to connect to" << ui->ip->text();
    if(!client.doConnect(ui->ip->text(), ui->port->text().toInt()))
    {
        ui->pushButton->setStyleSheet("background-color: green");
    } else {
        ui->pushButton->setStyleSheet("background-color: red");
    }
}
