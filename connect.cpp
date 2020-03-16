/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: connect.cpp - A file contains receiving process of client and server.
--
--
-- PROGRAM: Linux Chat Application
--
-- FUNCTIONS:
--		void clientReceiving(MainWindow * win)
--		void serverReceiving(MainWindow * win)
--      bool isclosed(int sock)
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
-- This file contains functions will manage the receiving process for client and server.
--
----------------------------------------------------------------------------------------------------------------------*/
#include "common.h"
#include "mainwindow.h"
#include <QDebug>

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: clientReceiving
--
-- DATE: March 15, 2020
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Jameson Cheong
--
-- PROGRAMMER: Jameson Cheong
--
-- INTERFACE: void clientReceiving(MainWindow * win)
--
-- RETURNS: void
--
-- NOTES:
-- This function is client-side receiving. It will process any incoming messsages from the server and
-- display on the GUI.
--
----------------------------------------------------------------------------------------------------------------------*/

void clientReceiving(MainWindow * win){
    Networks* net = win->getNet();
    int n, bytes_to_read;
    char rbuf[BUFLEN];
    char *bp;
    memset (rbuf,0,BUFLEN);
    bp = rbuf;

    bytes_to_read = BUFLEN;

    // client makes repeated calls to recv until no more data is expected to arrive.
    while(net->connected){
        n = 0;
        while ((n = recv (net->sd, bp, bytes_to_read, 0)) < BUFLEN)
        {

            bp += n;
            bytes_to_read -= n;
        }
        if(!net->connected){
            return;
        }else{
            win->displayMessages(rbuf);
        }
    }
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: serverReceiving
--
-- DATE: March 15, 2020
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Jameson Cheong
--
-- PROGRAMMER: Jameson Cheong
--
-- INTERFACE: void serverReceiving(MainWindow * win)
--
-- RETURNS: void
--
-- NOTES:
-- This function is server-side receiving. It processes any incoming messsages from the client and
-- send messages to other clients connected. If new client connected, the server displays new connected
-- client on the GUI.
--
----------------------------------------------------------------------------------------------------------------------*/
void serverReceiving(MainWindow * win){
    string msg;
    int nready, bytes_to_read;
    int new_sd, sockfd, client_len,sockfdEcho;
    struct sockaddr_in client_addr;
    char *bp, buf[BUFLEN];
    int i, maxi;
    int maxfd, client[FD_SETSIZE];
    ssize_t n, k;
    fd_set rset, allset;
    Networks* net = win->getNet();

    maxfd	= net->sd;	// initialize
    maxi	= -1;		// index into client[] array

    for (i = 0; i < FD_SETSIZE; i++)
            client[i] = -1;             // -1 indicates available entry

    FD_ZERO(&allset);
    FD_SET(net->sd, &allset);
    while (net->connected)
    {
        rset = allset;               // structure assignment
        nready = select(maxfd + 1, &rset, NULL, NULL, NULL);

            if (FD_ISSET(net->sd, &rset)) // new client connection
            {
                client_len = sizeof(client_addr);
                if ((new_sd = accept(net->sd, (struct sockaddr *) &client_addr, (socklen_t *) &client_len)) == -1)
                   return;

                printf(" Remote Address:  %s\n", inet_ntoa(client_addr.sin_addr));
                msg = "Client: " + to_string(new_sd);
                msg = msg + " Remote Address: ";
                msg = msg + inet_ntoa(client_addr.sin_addr);
                win->displayMessages(msg);
                for (i = 0; i < FD_SETSIZE; i++)
                if (client[i] < 0)
                {
                    client[i] = new_sd;	// save descriptor
                    break;
                }
                if (i == FD_SETSIZE)
                {
                    printf ("Too many clients\n");
                    exit(1);
                }

                FD_SET (new_sd, &allset);     // add new descriptor to set
                if (new_sd > maxfd)
                    maxfd = new_sd;	// for select

                if (i > maxi)
                    maxi = i;	// new max index in client[] array

                if (--nready <= 0)
                    continue;	// no more readable descriptors
             }

        for (i = 0; i <= maxi; i++)	// check all clients for data
        {
            if ((sockfd = client[i]) < 0)
                continue;

            if (FD_ISSET(sockfd, &rset))
            {
                bp = buf;
                bytes_to_read = BUFLEN;
                while ((n = read(sockfd, bp, bytes_to_read)) > 0)
                {
                    bp += n;
                    bytes_to_read -= n;
                }
                for (i = 0; i <= maxi; i++)	// echo to all clients
                {
                    if ((sockfdEcho = client[i]) < 0)
                        continue;
                    if(!isclosed(sockfd) && sockfdEcho != sockfd) {
                        msg = "Client " + to_string(sockfd);
                        msg = msg + " : ";
                        msg = msg + buf;
                        k = write(sockfdEcho, msg.c_str(), BUFLEN);   // echo to client
                        msg = msg + "; Sent To Client: " + to_string(sockfdEcho);
                    }
                }
                if(isclosed(sockfd)){
                    printf(" Remote Address:  %s closed connection\n", inet_ntoa(client_addr.sin_addr));
                    msg = "Client: " + to_string(sockfd) +" Remote Address: " ;
                    msg = msg + inet_ntoa(client_addr.sin_addr);
                    msg = msg + " closed connection";
                    win->displayMessages(msg);
                    close(sockfd);
                    FD_CLR(sockfd, &allset);
                        client[i] = -1;
                }

                if (--nready <= 0)
                    break;        // no more readable descriptors
            }

        }
    }
}
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: isclosed
--
-- DATE: March 15, 2020
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Jameson Cheong
--
-- PROGRAMMER: Jameson Cheong
--
-- INTERFACE: bool isclosed(int sock)
--
-- RETURNS: bool
--
-- NOTES:
-- This function is checks client's socket is closed.
--
----------------------------------------------------------------------------------------------------------------------*/
bool isclosed(int sock) {
  fd_set rfd;
  FD_ZERO(&rfd);
  FD_SET(sock, &rfd);
  timeval tv = { 0 };
  select(sock+1, &rfd, 0, 0, &tv);
  if (!FD_ISSET(sock, &rfd))
    return false;
  int n = 0;
  ioctl(sock, FIONREAD, &n);
  return n == 0;
}



