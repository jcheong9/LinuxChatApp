#ifndef COMMON_H
#define COMMON_H
#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdlib.h>
#include <strings.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include "connectivitymanager.h"
#include "connect.h"
using namespace std;

typedef struct Network{
    int port;
    int sdClient;
    int clientMode;
    const char* address;
    int connected;
    int msgid;
}Network;

#endif // COMMON_H
