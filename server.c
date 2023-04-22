#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>

#include "structs.h"

void clear_products(struct Product admin_products[],  int length){
    for(int i=0;i<length;i++){
        admin_products[i].id=-1;
    }
}

void add_product(struct Product admin_products[], struct Product product, int length){
    for(int i=0;i<length;i++){
        if(admin_products[i].id==-1){
            admin_products[i]=product;
            printf("Added\n");
            return;
        }
    }
    printf("Could Not Add\n");
}

void delete_product(struct Product admin_products[], int id, int length){
    for(int i=0;i<length;i++){
        if(admin_products[i].id==id){
            admin_products[i].id=-1;
            printf("Deleted\n");
            return ;
        }
    }
    printf("Product not found\n");

}

void update_price(struct Product admin_products[], int id, int price,int length){
    for(int i=0;i<length;i++){
        if(admin_products[i].id==id){
            admin_products[i].price=price;
            printf("Updated\n");
            return ;
        }
    }
    printf("Product not found\n");
}

void update_quantity(struct Product admin_products[], int id, int quantity,int length){
    for(int i=0;i<length;i++){
        if(admin_products[i].id==id){
            admin_products[i].quantity=quantity;
            printf("Updated\n");
            return ;
        }
    }
    printf("Product not found\n");
}


int setup_connection(){
    struct sockaddr_in server;
    int sd;
    socklen_t clientLen;
    sd = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(5001);

    bind(sd, (struct sockaddr *)&server, sizeof(server));
    listen(sd, 50);
    return sd;
}

int main(){
    struct Product products[1000];
    clear_products(products,1000);
    int sd = setup_connection(),nsd=0,type;
    struct sockaddr_in client;
    printf("Listening to connections\n");
    while(1){
        socklen_t clientLen = sizeof(client);
        nsd = accept(sd, (struct sockaddr *)&client, &clientLen);
        if(!fork()){
            read(nsd, &type, sizeof(int));

            if(type==0){

                printf("\nConnected to an admin\n");
                while(1){
                    int choice=0;
                    read(nsd, &choice, sizeof(int));
                    if(choice==1){
                        struct Product product;
                        read(nsd,&product,sizeof(struct Product));
                        add_product(products,product,1000);
                    }else if(choice==2){
                        int id;
                        read(nsd,&id,sizeof(int));
                        delete_product(products,id,1000);
                    }else if(choice==3){
                        int id,price;
                        read(nsd,&id,sizeof(int));
                        read(nsd,&price,sizeof(int));
                        update_price(products,id,price,1000);
                    }else if(choice==4){
                        int id,quantity;
                        read(nsd,&id,sizeof(int));
                        read(nsd,&quantity,sizeof(int));
                        update_quantity(products,id,quantity,1000);
                    }else if(choice==5){
                        write(nsd,products,sizeof(products));
                    }
                }

            }else if(type==1){

                printf("Connected to a user\n");
                while(1){
                    // user functions
                    
                }

            }
        }   
    }
    close(nsd);
    close(sd);
}