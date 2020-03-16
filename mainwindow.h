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
    QTextBrowser* getTB(){
        return qtTB;
    }
    void displayMessages(string mesgServ);

private:
    Networks network;
    Ui::MainWindow *ui;
    string status;
    QTextBrowser* qtTB;
    void getParameters(Network* net);
    void saveToFile();


signals:
    //void valueChanage(string newVal);

private slots:
    void disconnectPress();
    void connectPress();
    void sendPress();
    void setStatus(string value, int error);
};

void serverReceiving(MainWindow * win);
void clientReceiving(MainWindow * win);
#endif // MAINWINDOW_H
