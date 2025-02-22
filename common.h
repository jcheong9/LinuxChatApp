/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: common.h - contains the declaration functions for this application.
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
-- This is a header file contains declaration functions client.
----------------------------------------------------------------------------------------------------------------------*/
#ifndef COMMON_H
#define COMMON_H
#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdlib.h>
#include <cstring>
#include <iostream>
#include <arpa/inet.h>

#include <unistd.h>
#include <pthread.h>
#include "connectivitymanager.h"
#include <QTextBrowser>
#include <QFileDialog>
#include <QMessageBox>
#include <sys/ioctl.h>

using namespace std;

#define SERVER_TCP_PORT 7000	// Default port
#define BUFLEN	255		//Buffer length
#define TRUE	1
#define LISTENQ	5
#define MAXLINE 4096
#define  MSGSIZE 2

typedef struct Network{
    int port;
    int sd;
    int clientMode;
    const char* address;
    int connected;
    pthread_t thread1;
}Networks;

//Function protypes
int connectivityManager(Networks * net);
int initializedServer(Networks * net);
int initializedClient(Networks * net);
void* connectClientServer(void *);
static void SystemFatal(const char* message);
bool isclosed(int sock);



#endif // COMMON_H
