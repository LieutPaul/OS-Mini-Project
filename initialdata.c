#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>

#include "structs.h"

void clear_products(struct Product products[],  int length){
    for(int i=0;i<length;i++){
        products[i].id = -1;
    }
}

void clear_customers(struct Customer customers[], int length){
    for(int i=0; i<length;i++){
        customers[i].assigned = 0;
        customers[i].customer_id = i+1;
        for(int j=0;j<MAX_CART_ITEMS;j++){
            customers[i].cart_items[j].id = -1;
        }
    }
}

int main(){
    
    struct Product products[MAX_PRODUCTS];
    struct Customer customers[MAX_CUSTOMERS];

    clear_products(products, MAX_PRODUCTS);
    clear_customers(customers, MAX_CUSTOMERS);

    int fd1 = open("products.dat", O_CREAT | O_WRONLY, 0777);
    int fd2 = open("customers.dat", O_CREAT | O_WRONLY, 0777);

    for(int i=0;i<MAX_PRODUCTS;i++){
        write(fd1,&products[i],sizeof(struct Product));
    }

    for(int i=0;i<MAX_CUSTOMERS;i++){
        write(fd2,&customers[i],sizeof(struct Customer));
    }

    int fd3 = open("transaction_logs.txt",O_CREAT,0777);
    
}