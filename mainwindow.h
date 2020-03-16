/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: mainwindow.h - contains the declaration functions for QT GUI.
--
-- PROGRAM: Linux Chat Application
--
-- DATE: March 15, 2020
--
-- REVISIONS:
--
-- DESIGNER: Jameson Cheong
--
-- PROGRAMMER: Jameson Cheong
--
-- NOTES:
-- This is a header file contains declaration functions for QT GUI objects.
----------------------------------------------------------------------------------------------------------------------*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "common.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    static void * connectClientServer(void*);
    Networks* getNet(){
        return &network;
    }
    void displayMessages(string mesgServ);

private:
    Networks network;
    Ui::MainWindow *ui;
    string status;
    QTextBrowser* qtTB;
    void getParameters(Network* net);
    void saveToFile();

private slots:
    void disconnectPress();
    void connectPress();
    void sendPress();
    void setStatus(string value, int error);
};

void serverReceiving(MainWindow * win);
void clientReceiving(MainWindow * win);
#endif // MAINWINDOW_H
