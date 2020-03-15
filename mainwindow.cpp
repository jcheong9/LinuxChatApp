#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
//Networks network;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->connectPB,SIGNAL(released()),this,SLOT(connectPress()));
    connect(ui->disconnectPB,SIGNAL(released()),this,SLOT(disconnectPress()));
    connect(ui->sendPB,SIGNAL(released()),this,SLOT(sendPress()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::connectPress(){

    getParameters(&network);
    if (connectivityManager(&network)){
        ui->connectPB->setEnabled(false);
        network.connected = 1;
        setStatus("Connected",0);
        pthread_create(&network.thread1, nullptr, connectClientServer, (void *) this);
    }else{
        setStatus("Failed to Connected",1);
    }
}

void MainWindow::disconnectPress(){
    network.connected = 0;
    ::close(network.sd);
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

void MainWindow::getParameters(Network* net){
    net->port= atoi(ui->portLE->text().toUtf8().constData());
    net->address=ui->ipaddressLE->text().toUtf8().constData();
    if(ui->clientRB->isChecked()){
        net->clientMode = 1;
    }else{
        net->clientMode = 0;
    }
}

void* MainWindow::connectClientServer(void* network){
    //start receiveing thread
    MainWindow * win = (MainWindow *)network;
    if(win->network.clientMode){
        while(win->network.connected){
            clientReceiving(&win->network);
            win->displayMessages(win->network.message);
        }
    }else{
        serverReceiving(&win->network);
    }
    return &win->network;
}

void MainWindow::sendPress(){
    string message = ui->inputLE->text().toUtf8().constData();
    if(network.clientMode && network.connected){
        char sbuf[BUFLEN];
        strncpy(sbuf, message.c_str(),BUFLEN);
        send (network.sd, sbuf, BUFLEN, 0);
    }

}

void MainWindow::displayMessages(string mesgServ){
    ui->messagesTB->insertPlainText("hello\r\nhhe");
    ui->messagesTB->append("hello\r\nhhe");
     ui->messagesTB->insertPlainText("hello\r\nhhe");
     ui->messagesTB->append("hello\r\nhhe");
     ui->messagesTB->append("hello\r\nhhe");
     ui->messagesTB->append("hello\r\nhhe");
//    ui->messagesTB->setText( QString::fromStdString(mesgServ));
}

