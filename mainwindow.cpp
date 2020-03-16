#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: main.cpp - A simple program for client to chat between each other. The server send any incoming
--                         messages to clients connects and displays client connected. The client-side displayed
--                         incoming messages from server.
--
--
-- PROGRAM: Linux Chat Application
--
-- FUNCTIONS:
--		int main(int argc, char *argv[])
--
--
-- DATE: March 15, 2020
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Jameson Cheong
--
-- PROGRAMMER: Jameson Cheong
--
-- NOTES:
-- This function is the starting point for this application. It initialized the QT GUI object and start the application.
--
----------------------------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: MainWindow
--
-- DATE: March 15, 2020
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Jameson Cheong
--
-- PROGRAMMER: Jameson Cheong
--
-- INTERFACE: MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) , ui(new Ui::MainWindow)
--
-- RETURNS: void
--
-- NOTES:
-- This constructor initalized an objects when the MainWindow is constructed. It links buttons to
-- functions and set up the UI.
--
----------------------------------------------------------------------------------------------------------------------*/
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    qtTB = ui->messagesTB;
    ui->setupUi(this);
    connect(ui->connectPB,SIGNAL(released()),this,SLOT(connectPress()));
    connect(ui->disconnectPB,SIGNAL(released()),this,SLOT(disconnectPress()));
    connect(ui->sendPB,SIGNAL(released()),this,SLOT(sendPress()));
    connect(ui->actionSave, &QAction::triggered, [=]() { saveToFile(); });
}
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: ~MainWindow
--
-- DATE: March 15, 2020
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Jameson Cheong
--
-- PROGRAMMER: Jameson Cheong
--
-- INTERFACE: MainWindow::~MainWindow()
--
-- RETURNS: void
--
-- NOTES:
-- This deconstructor destorys the MainWindow
--
----------------------------------------------------------------------------------------------------------------------*/
MainWindow::~MainWindow()
{
    delete ui;
}
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: connectPress
--
-- DATE: March 15, 2020
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Jameson Cheong
--
-- PROGRAMMER: Jameson Cheong
--
-- INTERFACE: void MainWindow::connectPress()
--
-- RETURNS: void
--
-- NOTES:
-- This function handles the connect button pressed. It will creates thread, disable button and change status to connect
-- when the checks are passed.
--
----------------------------------------------------------------------------------------------------------------------*/
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
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: disconnectPress
--
-- DATE: March 15, 2020
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Jameson Cheong
--
-- PROGRAMMER: Jameson Cheong
--
-- INTERFACE: void MainWindow::disconnectPress()
--
-- RETURNS: void
--
-- NOTES:
-- This function handles the connect button pressed. It will creates thread, disable button and change status to connect
-- when the checks are passed.
--
----------------------------------------------------------------------------------------------------------------------*/
void MainWindow::disconnectPress(){
    network.connected = 0;
    ::close(network.sd);
    ui->connectPB->setEnabled(true);
    ui->statusQL->setText("Disconnect");
    ui->statusQL->setStyleSheet(QStringLiteral("QLabel{color: rgb(170, 0, 0);}"));
}
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: setStatus
--
-- DATE: March 15, 2020
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Jameson Cheong
--
-- PROGRAMMER: Jameson Cheong
--
-- INTERFACE: void MainWindow::setStatus(string value, int error)
--
-- RETURNS: void
--
-- NOTES:
-- This function handles the status display. The color of display will changed based on error value (0: green, 1: red)
--
----------------------------------------------------------------------------------------------------------------------*/
void MainWindow::setStatus(string value, int error){
    if(error){
        ui->statusQL->setStyleSheet(QStringLiteral("QLabel{color: rgb(170, 0, 0);}"));
    }else{
        ui->statusQL->setStyleSheet(QStringLiteral("QLabel{color: rgb(124,252,0);}"));
    }
    ui->statusQL->setText(QString::fromStdString(value));
}
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: getParameters
--
-- DATE: March 15, 2020
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Jameson Cheong
--
-- PROGRAMMER: Jameson Cheong
--
-- INTERFACE: void MainWindow::getParameters(Network* net)
--
-- RETURNS: void
--
-- NOTES:
-- This function handles getting all the inputs(client mode selected, port and ip address) in GUI.
--
----------------------------------------------------------------------------------------------------------------------*/
void MainWindow::getParameters(Network* net){
    net->port= atoi(ui->portLE->text().toUtf8().constData());
    net->address=ui->ipaddressLE->text().toUtf8().constData();
    if(ui->clientRB->isChecked()){
        net->clientMode = 1;
    }else{
        net->clientMode = 0;
    }
}
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: connectClientServer
--
-- DATE: March 15, 2020
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Jameson Cheong
--
-- PROGRAMMER: Jameson Cheong
--
-- INTERFACE: void* MainWindow::connectClientServer(void* network)
--
-- RETURNS: void
--
-- NOTES:
-- This thread function manages client and server receiving.
--
----------------------------------------------------------------------------------------------------------------------*/
void* MainWindow::connectClientServer(void* network){
    //start receiveing thread
    MainWindow * win = (MainWindow *)network;
    if(win->network.clientMode){
        clientReceiving(win);
    }else{
        serverReceiving(win);
    }
    return &win->network;
}
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: sendPress
--
-- DATE: March 15, 2020
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Jameson Cheong
--
-- PROGRAMMER: Jameson Cheong
--
-- INTERFACE: void MainWindow::sendPress()
--
-- RETURNS: void
--
-- NOTES:
-- This function handles send button pressed. It parsed the input field for message and sends to server.
--
----------------------------------------------------------------------------------------------------------------------*/
void MainWindow::sendPress(){
    string message = ui->inputLE->text().toUtf8().constData();
    if(network.clientMode && network.connected){
        char sbuf[BUFLEN];
        strncpy(sbuf, message.c_str(),BUFLEN);
        send (network.sd, sbuf, BUFLEN, 0);
    }
}
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: displayMessages
--
-- DATE: March 15, 2020
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Jameson Cheong
--
-- PROGRAMMER: Jameson Cheong
--
-- INTERFACE: void MainWindow::displayMessages(string mesgServ)
--
-- RETURNS: void
--
-- NOTES:
-- This function handles displaying messages.
--
----------------------------------------------------------------------------------------------------------------------*/
void MainWindow::displayMessages(string mesgServ){
    QString qtstr = QString::fromStdString(mesgServ);
    QMetaObject::invokeMethod(ui->messagesTB, "append", Q_ARG(QString, qtstr));
}
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: saveToFile
--
-- DATE: March 15, 2020
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Jameson Cheong
--
-- PROGRAMMER: Jameson Cheong
--
-- INTERFACE: void MainWindow::saveToFile()
--
-- RETURNS: void
--
-- NOTES:
-- This function handles saving the file. It opens a dialog and prompts the user for file name.
-- the texts are saved into the selected file.
--
----------------------------------------------------------------------------------------------------------------------*/
void MainWindow::saveToFile()
{
    QString str = ui->messagesTB->toPlainText();
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Save Messages"), "",
        tr("All Files (*)"));
    if (fileName.isEmpty())
        return;
    else {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::information(this, tr("Unable to open file"),
                file.errorString());
            return;
        }

        QTextStream out(&file);
        out << ui->messagesTB->toPlainText();
    }
}
