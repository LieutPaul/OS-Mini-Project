#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>

#include "admin_connections.h"

int setup_connection(){
    struct sockaddr_in server;
    int sd;
    socklen_t clientLen;
    sd = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(5001);

    bind(sd, (struct sockaddr *)&server, sizeof(server));
    listen(sd, 5);
    return sd;
}

void accept_request(){
    int sd = setup_connection();
    struct sockaddr_in client;
    while(1){
        socklen_t clientLen = sizeof(client);
        int nsd = accept(sd, (struct sockaddr *)&client, &clientLen);
        char buf[100];
        if(!fork()){
            read(nsd, buf, sizeof(buf));
            printf("%s\n",buf);
        }else{
            close(nsd);
        }   
    }
    close(sd);
}