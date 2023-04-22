#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>

#include "structs.h"
#include "admin_connections.h"

void clear_products(struct Product admin_products[],  int length){
    for(int i=0;i<length;i++){
        admin_products[i].id=-1;
    }
}

void add_product(struct Product admin_products[], struct Product product, int length){
    for(int i=0;i<length;i++){
        if(admin_products[i].id==-1){
            admin_products[i]=product;
            return ;
        }
    }
}

int delete_product(struct Product admin_products[], int id, int length){
    for(int i=0;i<length;i++){
        if(admin_products[i].id==id){
            admin_products[i].id=-1;
            return 1;
        }
    }
    return 0;
}

int update_price(struct Product admin_products[], int id, int price,int length){
    for(int i=0;i<length;i++){
        if(admin_products[i].id==id){
            admin_products[i].price=price;
            return 1;
        }
    }
    return 0;
}

int update_quantity(struct Product admin_products[], int id, int quantity,int length){
    for(int i=0;i<length;i++){
        if(admin_products[i].id==id){
            admin_products[i].quantity=quantity;
            return 1;
        }
    }
    return 0;
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

int main(){
    int choice=0;
    struct Product admin_products[1000];
    clear_products(admin_products,1000);

    while(1){
        printf("Enter: \n1 to add a new Product.\n2 to delete a product.\n3 to update the price of a product.\n4 to update the quantity of a product.\n5 to see all your products.\n6 to listen to client.\n7 to exit;");
        scanf("%d",&choice);
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
            add_product(admin_products,product,1000);

        }else if(choice==2){
            
            int id=0;
            printf("Enter id of product to delete (>0): ");
            scanf("%d",&id);
            if(delete_product(admin_products,id,1000)==1){
                printf("Deleted item with id : %d \n",id);
            }else{
                printf("Item with id : %d does not exist.\n",id);
            }

        }else if(choice==3){
            
            int id,price;
            printf("Enter id of product to update price of: ");
            scanf("%d",&id);
            printf("Enter new price of item: ");
            scanf("%d",&price);
            if(update_price(admin_products,id,price,1000)==1){
                printf("Updated price of item with id : %d.\n",id);
            }else{
                printf("Item with id : %d does not exist.\n",id);
            }

        }else if(choice==4){

            int id,quantity;
            printf("Enter id of product to update quantity of: ");
            scanf("%d",&id);
            printf("Enter new quantity of item: ");
            scanf("%d",&quantity);
            if(update_quantity(admin_products,id,quantity,1000)==1){
                printf("Updated quantity of item with id : %d.\n",id);
            }else{
                printf("Item with id : %d does not exist.\n",id);
            }

        }else if(choice==5){

            print_products(admin_products,1000);
        
        }else if(choice==6){

            accept_request();
            
        }else if(choice==7){

            break;

        }
    }
}