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
#include "connect.h"
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
    char message[BUFLEN];
    pthread_t thread1;
}Networks;

//Function protypes
int connectivityManager(Networks * net);
int initializedServer(Networks * net);
int initializedClient(Networks * net);
void* connectClientServer(void *);
static void SystemFatal(const char* message);
void clientReceiving(Networks * net);
void serverReceiving(Networks * net);


#endif // COMMON_H
