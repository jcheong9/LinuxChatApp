#include "mainwindow.h"
#include "ui_mainwindow.h"

Networks network;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->connectPB,SIGNAL(released()),this,SLOT(connectPress()));
    connect(ui->disconnectPB,SIGNAL(released()),this,SLOT(disconnectPress()));
//    connect(ui->sendPB,SIGNAL(released()),this,SLOT(sendPress()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::connectPress(){
    pthread_t thread1;
    getParameters();
    if (connectivityManager(&network)){
        ui->connectPB->setEnabled(false);
        setStatus("Connected",0);
        // pthread_create(&thread1, nullptr, connectClientServer, (void *)this);
    }else{
        setStatus("Failed to Connected",1);
    }
}

void MainWindow::disconnectPress(){
    ui->connectPB->setEnabled(true);
    ui->statusQL->setText("Disconnect");
    ui->statusQL->setStyleSheet(QStringLiteral("QLabel{color: rgb(170, 0, 0);}"));
    QString XMAX=ui->portLE->text();
}

void MainWindow::setStatus(string value, int error){
    if(error){
        ui->statusQL->setStyleSheet(QStringLiteral("QLabel{color: rgb(170, 0, 0);}"));
    }else{
        ui->statusQL->setStyleSheet(QStringLiteral("QLabel{color: rgb(124,252,0);}"));
    }

    ui->statusQL->setText(QString::fromStdString(value));
}

void MainWindow::getParameters(){
    network.port= atoi(ui->portLE->text().toUtf8().constData());
    network.address=ui->ipaddressLE->text().toUtf8().constData();
    if(ui->clientRB->isChecked()){
        network.clientMode = 1;
    }
}
