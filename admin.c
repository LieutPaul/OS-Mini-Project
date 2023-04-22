#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "admin.h"

int setup_admin_connection(){
    struct sockaddr_in serv;

    char buff[1000];

    int sd = socket(AF_INET, SOCK_STREAM, 0); 

    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv.sin_port = htons(5001); 

    connect(sd, (struct sockaddr *) &serv, sizeof(serv)); 

    write(sd,"admin",sizeof("admin"));
    return sd;
}

void send_admin_message(int sd){
    write(sd, "Message From admin to server.\n", sizeof("Message From admin to server.\n"));
}