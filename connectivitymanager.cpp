#include "connectivitymanager.h"
#include "common.h"

int connectivityManager(Network * net){
    if(net->port == 0){
        return 0;
    }
    if(net->clientMode){
        if(strcmp(net->address,"") <= 0){
            return 0;
        }
        if(initializedClient(net)){
            return 1;
        }
    }else{
        if(initializedServer(net)){
            return 1;
        }
    }
    return 0;
}

int initializedClient(Network * net){
    //client function
    return 1;
}

int initializedServer(Network * net){
    //server function
    struct sockaddr_in server;
    int  arg;

    int port = net->port;

    // Create a stream socket
    if ((net->sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        return 0;
        //SystemFatal("Cannot Create Socket!");

    // set SO_REUSEADDR so port can be resused imemediately after exit, i.e., after CTRL-c
    arg = 1;
    if (setsockopt (net->sd , SOL_SOCKET, SO_REUSEADDR, &arg, sizeof(arg)) == -1)
        return 0;
        //SystemFatal("setsockopt");

    // Bind an address to the socket
    bzero((char *)&server, sizeof(struct sockaddr_in));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = htonl(INADDR_ANY); // Accept connections from any client

    if (bind(net->sd , (struct sockaddr *)&server, sizeof(server)) == -1)
        return 0;
//        SystemFatal("bind error");

    // Listen for connections
    // queue up to LISTENQ connect requests
    listen(net->sd , LISTENQ);

    return 1;
}
