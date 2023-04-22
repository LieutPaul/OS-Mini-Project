#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "admin.h"
#include "user.h"

int main(){
    int choice=0,sd=0;
    printf("Enter 1 to login as user and 2 to login as admin\n");
    scanf("%d",&choice);
    if(choice==1){
        sd = setup_user_connection();
        while(1){
            printf("Press enter to send message\n");
            getchar();
            send_user_message(sd);
        }        
    }else if(choice==2){
        sd = setup_admin_connection();
        while(1){
            printf("Press enter to send message\n");
            getchar();
            send_admin_message(sd);
        } 
    }
}