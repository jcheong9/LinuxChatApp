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

private:
    Ui::MainWindow *ui;
    string status;
    void getParameters();

signals:
    void valueChanage(string newVal);

private slots:
    void disconnectPress();
    void connectPress();
    void setStatus(string value);
};
#endif // MAINWINDOW_H
