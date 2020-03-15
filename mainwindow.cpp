#include "mainwindow.h"
#include "ui_mainwindow.h"

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

void MainWindow::disconnectPress(){
    ui->connectPB->setEnabled(true);
    ui->statusQL->setText("Disconnect");
    ui->statusQL->setStyleSheet(QStringLiteral("QLabel{color: rgb(170, 0, 0);}"));
    QString XMAX=ui->portLE->text();
}

void MainWindow::setStatus(string value){
    ui->statusQL->setText(QString::fromStdString(value));
}
