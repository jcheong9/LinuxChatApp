#include "common.h"
#include "mainwindow.h"
#include <QDebug>

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
        win->displayMessages(rbuf);
    }

//    strncpy(net->message,rbuf,BUFLEN);
}


void serverReceiving(MainWindow * win){
    string msg;
    int nready, bytes_to_read;
    int new_sd, sockfd, client_len,sockfdEcho;
    struct sockaddr_in client_addr;
    char *bp, buf[BUFLEN];
    int i, maxi;
    int maxfd, client[FD_SETSIZE];
    ssize_t n;
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
//                SystemFatal("accept error");
            printf(" Remote Address:  %s\n", inet_ntoa(client_addr.sin_addr));
            msg = "ID: " + to_string(new_sd);
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
                    if(sockfdEcho != sockfd){
                        msg = "ID: " + to_string(sockfd);
                        msg = msg + " Message: ";
                        msg = msg + buf;
                        win->displayMessages(msg);
                        write(sockfdEcho, buf, BUFLEN);   // echo to client
                    }
                }
//                if (n == 0) // connection closed by client
//                {
//                    printf(" Remote Address:  %s closed connection\n", inet_ntoa(client_addr.sin_addr));
//                    msg = "ID: " + to_string(sockfd) +" Remote Address: " ;
//                    msg = msg + inet_ntoa(client_addr.sin_addr);
//                    msg = msg + " closed connection";
//                    win->displayMessages(msg);
//                    close(sockfd);
//                    FD_CLR(sockfd, &allset);
//                            client[i] = -1;
//                }

                if (--nready <= 0)
                            break;        // no more readable descriptors
            }
        }
    }
//    for(i = 0; i <= maxi; i++){
//        if ((sockfd = client[i]) < 0)
//            continue;

//        if (FD_ISSET(sockfd, &rset))
//        {
//            if (n == 0) // connection closed by client
//            {
//                printf(" Remote Address:  %s closed connection\n", inet_ntoa(client_addr.sin_addr));
//                msg = "ID: " + to_string(sockfd) +" Remote Address: " ;
//                msg = msg + inet_ntoa(client_addr.sin_addr);
//                msg = msg + " closed connection";
//                win->displayMessages(msg);
//                close(sockfd);
//                FD_CLR(sockfd, &allset);
//                        client[i] = -1;
//            }
//        }
//    }
}



