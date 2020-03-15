#ifndef COMMON_H
#define COMMON_H
#include "strings.h"
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
