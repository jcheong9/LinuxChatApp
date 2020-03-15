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
    void displayMessages(string mesgServ);

private:
    Networks network;
    Ui::MainWindow *ui;
    string status;
    void getParameters(Network* net);

signals:
    //void valueChanage(string newVal);

private slots:
    void disconnectPress();
    void connectPress();
    void sendPress();
    void setStatus(string value, int error);
};


#endif // MAINWINDOW_H
