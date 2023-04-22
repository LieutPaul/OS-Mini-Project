#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "admin.h"

int setup_admin_connection(){
    int type = 0;

    struct sockaddr_in serv;

    char buff[1000];

    int sd = socket(AF_INET, SOCK_STREAM, 0); 

    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv.sin_port = htons(5001); 

    connect(sd, (struct sockaddr *) &serv, sizeof(serv)); 

    write(sd,&type,sizeof(int));
    return sd;
}

void print_products(struct Product admin_products[], int length){
    printf("\n");
    for(int i=0;i<length;i++){
        if(admin_products[i].id!=-1){
            printf("ID: %d\n",admin_products[i].id);
            printf("Name: %s\n",admin_products[i].name);
            printf("Price: %d\n",admin_products[i].price);
            printf("Quantity: %d\n\n",admin_products[i].quantity);
        }
    }
}

void admin_options(int sd){
    int choice=0;
    struct Product products[MAX_PRODUCTS];

    while(1){

        printf("Enter: \n");
        printf("1 to add a new Product.\n");
        printf("2 to delete a product.\n");
        printf("3 to update the price of a product.\n");
        printf("4 to update the quantity of a product.\n");
        printf("5 to see all products.\n");
        printf("6 to exit.\n");
        scanf("%d",&choice);
        
        if(choice == 1 || choice==2 || choice==3 || choice==4 || choice==5 || choice==6){
            write(sd,&choice,sizeof(int));
        }else{
            printf("\nInvalid Choice\n");
            continue;
        }

        if(choice==1){

            struct Product product;
            printf("Enter id of product (>0): ");
            scanf(" %d",&product.id);
            printf("Enter name of product: ");
            scanf(" %[^\n]s",product.name);
            printf("Enter cost of product: ");
            scanf(" %d",&product.price);
            printf("Enter quantity of product: ");
            scanf(" %d",&product.quantity);
            write(sd, &product, sizeof(struct Product));

        }else if(choice==2){
            
            int id=0;
            printf("Enter id of product to delete (>0): ");
            scanf("%d",&id);
            write(sd,&id,sizeof(int));

        }else if(choice==3){
            
            int id,price;
            printf("Enter id of product to update price of: ");
            scanf("%d",&id);
            printf("Enter new price of item: ");
            scanf("%d",&price);
            write(sd,&id,sizeof(int));
            write(sd,&price,sizeof(int));

        }else if(choice==4){

            int id,quantity;
            printf("Enter id of product to update quantity of: ");
            scanf("%d",&id);
            printf("Enter new quantity of item: ");
            scanf("%d",&quantity);
            write(sd,&id,sizeof(int));
            write(sd,&quantity,sizeof(int));

        }else if(choice==5){

            read(sd,products,sizeof(products));
            print_products(products,MAX_PRODUCTS);
        
        }else if(choice==6){

            break;

        }
    }
}