#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "user.h"

int setup_user_connection(){
    struct sockaddr_in serv;

    int sd = socket(AF_INET, SOCK_STREAM, 0); 

    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv.sin_port = htons(5001); 

    connect(sd, (struct sockaddr *) &serv, sizeof(serv)); 

    write(sd,"user",sizeof("user"));

    return sd;
}

void send_user_message(int sd){
    write(sd, "Message From user to server.\n", sizeof("Message From user to server.\n"));
}


