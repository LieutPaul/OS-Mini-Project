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

void add_product(struct Product product){
    int fd = open("products.dat", O_RDWR, 0777);
    struct Product temp_product;
    while(read(fd,&temp_product,sizeof(struct Product)) != 0){
        if(temp_product.id==-1){
            lseek(fd,-sizeof(struct Product),SEEK_CUR);
            write(fd,&product,sizeof(struct Product));
            printf("Added Product\n");
            return ;
        }
    }
    printf("Could Not Add\n");
}

void delete_product(int id){
    int fd = open("products.dat",O_RDONLY, 0777);
    struct Product temp_product;
    while(read(fd,&temp_product,sizeof(struct Product)) != 0){
        if(temp_product.id==id){
            temp_product.id=-1;
            // lseek(fd,-sizeof(struct Product),SEEK_CUR);
            write(fd,&temp_product,sizeof(struct Product));
            printf("Deleted Product\n");
            return ;
        }
    }
    printf("Could Not Find Product\n");
}

void update_price(int id, int price){
    int fd = open("products.dat",O_RDONLY, 0777);
    struct Product temp_product;
    while(read(fd,&temp_product,sizeof(struct Product)) != 0){
        if(temp_product.id==id){
            temp_product.price=price;
            // lseek(fd,-sizeof(struct Product),SEEK_CUR);
            write(fd,&temp_product,sizeof(struct Product));
            printf("Updated Product\n");
            return ;
        }
    }
    printf("Could Not Find Product\n");
}

void update_quantity(int id, int quantity){
    int fd = open("products.dat",O_RDONLY, 0777);
    struct Product temp_product;
    while(read(fd,&temp_product,sizeof(struct Product)) != 0){
        if(temp_product.id == id){
            temp_product.quantity = quantity;
            // lseek(fd,-sizeof(struct Product),SEEK_CUR);
            write(fd,&temp_product,sizeof(struct Product));
            printf("Updated Product\n");
            return ;
        }
    }
    printf("Could Not Find Product\n");
}

void send_products(int nsd){
    struct Product products[MAX_PRODUCTS];
    int i = 0, fd = open("products.dat", O_RDONLY, 0777);;
    struct Product temp_product;
    while(read(fd,&temp_product,sizeof(struct Product)) != 0){
        products[i] = temp_product;
        i++;
    }
    write(nsd,products,sizeof(products));
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
                        add_product(product);
                    }else if(choice==2){
                        int id;
                        read(nsd,&id,sizeof(int));
                        delete_product(id);
                    }else if(choice==3){
                        int id,price;
                        read(nsd,&id,sizeof(int));
                        read(nsd,&price,sizeof(int));
                        update_price(id,price);
                    }else if(choice==4){
                        int id,quantity;
                        read(nsd,&id,sizeof(int));
                        read(nsd,&quantity,sizeof(int));
                        update_quantity(id,quantity);
                    }else if(choice==5){
                        send_products(nsd);
                    }
                }

            }else if(type==1){

                printf("Connected to a user\n");
                while(1){
                    int choice=0;
                    read(nsd, &choice, sizeof(int));
                    
                    if(choice==1){
                        
                    }
                    
                }

            }
        }   
    }
    close(nsd);
    close(sd);
}