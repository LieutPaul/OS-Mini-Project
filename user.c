#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "user.h"

int setup_user_connection(int customer_id){
    int type=1;
    struct sockaddr_in serv;

    int sd = socket(AF_INET, SOCK_STREAM, 0); 

    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv.sin_port = htons(5001); 

    connect(sd, (struct sockaddr *) &serv, sizeof(serv)); 

    write(sd,&type,sizeof(int));
    write(sd,&customer_id,sizeof(int));

    return sd;
}

void display_products(struct Product products[], int length){
    for(int i=0;i<length;i++){
        if(products[i].id!=-1){
            printf("ID: %d\n",products[i].id);
            printf("Name: %s\n",products[i].name);
            printf("Price: %d\n",products[i].price);
            printf("Quantity: %d\n\n",products[i].quantity);
        }
    }
}

void user_options(int sd){
    
    int choice=0;
    struct Product products[1000];
    
    while(1){
        
        printf("Enter: \n");
        printf("1 to see all products.\n");
        printf("2 to add an item to your cart.\n");
        printf("3 to view your cart items.\n");
        printf("4 to update a cart item\n");
        printf("5 to delete an item in the cart\n");
        printf("6 to enter payment portal\n");
        printf("7 to exit.\n");
        scanf("%d",&choice);

        if(choice == 1 || choice==2 || choice==3 || choice==4 || choice==5 || choice==6 || choice==7){
            write(sd,&choice,sizeof(int));
        }else{
            printf("\nInvalid Choice\n");
            continue;
        }

        if(choice==1){
            
            printf("\nDisplaying Items: \n");
            read(sd,products,sizeof(products));
            display_products(products,MAX_PRODUCTS);

        }else if(choice==2){
            
            int id=0,quantity=0,return_status=0;
            printf("Enter id of product to add: ");
            scanf("%d",&id);
            printf("Enter quantity of product to add: ");
            scanf("%d",&quantity);
            write(sd,&id,sizeof(int));
            write(sd,&quantity,sizeof(int));
            read(sd,&return_status,sizeof(int));

            return_status == 1 ? printf("Added Successfully\n") : printf("Insufficient Quantity or Full Cart\n");

        }else if(choice==3){
            
            printf("\nDisplaying Cart Items:\n");
            read(sd,products,sizeof(products));
            display_products(products,MAX_CART_ITEMS);
            
        }else if(choice==4){

            int id=0,quantity=0;
            printf("Enter id of product to update quantity of: ");
            scanf("%d",&id);
            printf("Enter new quantity of product to add to cart: ");
            scanf("%d",&quantity);
            write(sd,&id,sizeof(int));
            write(sd,&quantity,sizeof(int));
            
        }else if(choice==5){

            int id=0;
            printf("Enter id of product to delete: ");
            scanf("%d",&id);
            write(sd,&id,sizeof(int));
            
        }else if(choice==6){
            
        }else if(choice==7){
            break;
        }
    }
}


