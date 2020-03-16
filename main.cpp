#include "mainwindow.h"
#include <QApplication>
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
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
