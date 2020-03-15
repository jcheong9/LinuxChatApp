#include "connectivitymanager.h"


int connectivityManager(Network * net){
    if(net->clientMode){
        if(initalizedClient(net)){
            return 1;
        }
    }else{
        if(initalizedServer(net)){
            return 1;
        }
    }
    return 0;
}

int initalizedClient(Network * net){
    //client function
    return 0;
}

int initalizedServer(Network * net){
    //server function
    return 0;
}
