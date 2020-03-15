#include "mainwindow.h"
#include "ui_mainwindow.h"


Network network;
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
    if (connectivityManager(network)){
         pthread_create(&thread1, nullptr, connectClientServer, (void *)this);
    }
}

void MainWindow::disconnectPress(){
    ui->connectPB->setEnabled(true);
    ui->statusQL->setText("Disconnect");
    ui->statusQL->setStyleSheet(QStringLiteral("QLabel{color: rgb(170, 0, 0);}"));
    QString XMAX=ui->portLE->text();
}

void MainWindow::setStatus(string value){
    ui->statusQL->setText(QString::fromStdString(value));
}

void MainWindow::getParameters(){

}
