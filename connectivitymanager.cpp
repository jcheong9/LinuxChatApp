/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: connectivitymanager.cpp - A file contains server and client connection.
--
--
-- PROGRAM: Linux Chat Application
--
-- FUNCTIONS:
--		int connectivityManager(Network * net)
--		int initializedClient(Network * net)
--      int initializedServer(Network * net)
--      static void SystemFatal(const char* message)
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
-- This file contains functions manage connections for client and server.
--
----------------------------------------------------------------------------------------------------------------------*/
#include "connectivitymanager.h"
#include "common.h"
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: connectivityManager
--
-- DATE: March 15, 2020
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Jameson Cheong
--
-- PROGRAMMER: Jameson Cheong
--
-- INTERFACE: int connectivityManager(Network * net)
--
-- RETURNS: int
--
-- NOTES:
-- This function is checks client mode is selected and inputs are empty.
--
----------------------------------------------------------------------------------------------------------------------*/
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
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: initializedClient
--
-- DATE: March 15, 2020
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Jameson Cheong
--
-- PROGRAMMER: Jameson Cheong
--
-- INTERFACE: int initializedClient(Network * net)
--
-- RETURNS: int
--
-- NOTES:
-- This function is initialized connection for client to server. It returns 1 successfully and 0 failed.
--
----------------------------------------------------------------------------------------------------------------------*/
int initializedClient(Network * net){
    //client function
    struct hostent	*hp;
    struct sockaddr_in server;
    int port = net->port;
    const char* address = net->address;
    char **pptr;
    // Create the socket
    if ((net->sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Cannot create socket");
        return 0;
    }
    bzero((char *)&server, sizeof(struct sockaddr_in));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    if ((hp = gethostbyname(address)) == NULL)
    {
        fprintf(stderr, "Unknown server address\n");
        return 0;
    }
    bcopy(hp->h_addr, (char *)&server.sin_addr, hp->h_length);

    // Connecting to the server
    if (connect (net->sd , (struct sockaddr *)&server, sizeof(server)) == -1)
    {
        fprintf(stderr, "Can't connect to server\n");
        perror("connect");
        return 0;
    }
    printf("Connected:    Server Name: %s\n", hp->h_name);
    pptr = hp->h_addr_list;
    return 1;
}
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: initializedServer
--
-- DATE: March 15, 2020
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Jameson Cheong
--
-- PROGRAMMER: Jameson Cheong
--
-- INTERFACE: int initializedServer(Network * net)
--
-- RETURNS: int
--
-- NOTES:
-- This function is initialized connection for server to client. It returns 1 successfully and 0 failed.
--
----------------------------------------------------------------------------------------------------------------------*/
int initializedServer(Network * net){
    //server function
    struct sockaddr_in server;
    int arg;
    int port = net->port;

    // Create a stream socket
    if ((net->sd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        SystemFatal("Cannot Create Socket!");
        return 0;
    }


    // set SO_REUSEADDR so port can be resused imemediately after exit, i.e., after CTRL-c
    arg = 1;
    if (setsockopt (net->sd , SOL_SOCKET, SO_REUSEADDR, &arg, sizeof(arg)) == -1){
        SystemFatal("setsockopt");
        return 0;
    }

    // Bind an address to the socket
    bzero((char *)&server, sizeof(struct sockaddr_in));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = htonl(INADDR_ANY); // Accept connections from any client

    if (bind(net->sd , (struct sockaddr *)&server, sizeof(server)) == -1){
        SystemFatal("bind error");
        return 0;
    }

    // Listen for connections
    // queue up to LISTENQ connect requests
    listen(net->sd , LISTENQ);

    return 1;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: SystemFatal
--
-- DATE: March 15, 2020
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Jameson Cheong
--
-- PROGRAMMER: Jameson Cheong
--
-- INTERFACE: static void SystemFatal(const char* message)
--
-- RETURNS: void
--
-- NOTES:
-- This function prints the error stored in errno and aborts the program.
--
----------------------------------------------------------------------------------------------------------------------*/
static void SystemFatal(const char* message)
{
    perror (message);
    //exit (EXIT_FAILURE);
}
