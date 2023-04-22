#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main(){
    struct sockaddr_in serv;
    int sd,ret;
    char buff[1000];

    sd = socket(AF_INET, SOCK_STREAM, 0); // 0 -> IP

    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv.sin_port = htons(5001); // Port Number

    connect(sd, (struct sockaddr *) &serv, sizeof(serv)); // connect will setup connection establishment: initiate 3 way hand shake

    printf("Press enter to send message to server.\n");
    getchar();
    write(sd, "Message From client to server.\n", sizeof("Message From client to server\n"));
    
    close(sd);

    return (0);
}


