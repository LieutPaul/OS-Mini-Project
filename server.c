#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/file.h>
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
    struct flock fl;
    int fd = open("products.dat",O_RDWR, 0777);
    struct Product temp_product;
    while(read(fd,&temp_product,sizeof(struct Product)) != 0){
        if(temp_product.id==id){

            fl.l_whence=SEEK_CUR; 
            fl.l_start=0;        
            fl.l_len=sizeof(struct Product);        
            fl.l_pid=getpid(); 
            fl.l_type=F_WRLCK;
            fcntl(fd, F_SETLKW, &fl);
            
            temp_product.id=-1;
            lseek(fd,-sizeof(struct Product),SEEK_CUR);
            write(fd,&temp_product,sizeof(struct Product));

            fl.l_type=F_UNLCK;
            fcntl(fd, F_SETLKW, &fl); 

            printf("Deleted Product\n");
            return ;
        }
    }
    printf("Could Not Find Product\n");
}

void update_price(int id, int price){
    struct flock fl;
    int fd = open("products.dat", O_RDWR, 0777);
    struct Product temp_product;
    while(read(fd,&temp_product,sizeof(struct Product)) != 0){
        if(temp_product.id==id){

            fl.l_whence=SEEK_CUR; 
            fl.l_start=0;        
            fl.l_len=sizeof(struct Product);        
            fl.l_pid=getpid(); 
            fl.l_type=F_WRLCK;
            fcntl(fd, F_SETLKW, &fl);

            temp_product.price=price;
            lseek(fd,-sizeof(struct Product),SEEK_CUR);
            write(fd,&temp_product,sizeof(struct Product));

            fl.l_type=F_UNLCK;
            fcntl(fd, F_SETLKW, &fl);

            printf("Updated Product\n");
            return ;
        }
    }
    printf("Could Not Find Product\n");
}

void update_quantity(int id, int quantity){
    struct flock fl;
    int fd = open("products.dat",O_RDWR, 0777);
    struct Product temp_product;
    while(read(fd,&temp_product,sizeof(struct Product)) != 0){
        if(temp_product.id == id){

            fl.l_whence=SEEK_CUR; 
            fl.l_start=0;        
            fl.l_len=sizeof(struct Product);        
            fl.l_pid=getpid(); 
            fl.l_type=F_WRLCK;
            fcntl(fd, F_SETLKW, &fl);

            temp_product.quantity = quantity;
            lseek(fd,-sizeof(struct Product),SEEK_CUR);
            write(fd,&temp_product,sizeof(struct Product));

            fl.l_type=F_UNLCK;
            fcntl(fd, F_SETLKW, &fl);
            
            printf("Updated Product\n");
            return ;
        }
    }
    printf("Could Not Find Product\n");
}

void send_products(int nsd){
    struct Product products[MAX_PRODUCTS];
    int i = 0, fd = open("products.dat", O_RDONLY, 0777);
    struct Product temp_product;
    while(read(fd,&temp_product,sizeof(struct Product)) != 0){
        products[i] = temp_product;
        i++;
    }
    write(nsd,products,sizeof(products));
}

int generate_new_customerid(){

    struct Customer temp_customer;
    int fd = open("customers.dat",O_RDWR, 0777);

    while(read(fd,&temp_customer,sizeof(struct Customer)) != 0){
        if(temp_customer.assigned == 0){
            temp_customer.assigned = 1;
            lseek(fd,-sizeof(struct Customer),SEEK_CUR);
            write(fd,&temp_customer,sizeof(struct Customer));
            return temp_customer.customer_id;
        }
    }
    return -1;
}

struct Product* find_product_by_id(int id){
    struct Product *product = (struct Product *) malloc(sizeof(struct Product));
    int fd = open("products.dat", O_RDONLY, 0777);
    while(read(fd,product,sizeof(struct Product)) != 0){
        if(product->id == id){
            return product;
        }
    }
    return NULL;
}

int add_cart_item(int customer_id,int product_id,int quantity){
    
    struct Customer temp_customer;
    struct Product *product = find_product_by_id(product_id);
    
    if(product == NULL){
        printf("Product not found\n");
        return -1;
    }
    
    int fd = open("customers.dat", O_RDWR, 0777);

    while(read(fd,&temp_customer,sizeof(struct Customer)) != 0){
        
        if(temp_customer.customer_id==customer_id  && temp_customer.assigned==1){

            for(int j=0;j<MAX_CART_ITEMS;j++){
                
                if(temp_customer.cart_items[j].id == -1){
                    
                    if(quantity <= product->quantity){
                        
                        product->quantity=quantity;
                        temp_customer.cart_items[j] = *product;
                        lseek(fd,-sizeof(struct Customer),SEEK_CUR);
                        write(fd,&temp_customer,sizeof(struct Customer));
                        printf("Added to Cart\n");
                        return 1;

                    }else{
                        printf("Insufficient Quantity\n");
                        return -1;
                    }
                }
            }
            printf("Cart Limit Reached\n");
            return -1;
        }
    }

    printf("Could not find Customer\n");
    return -1;
}

int update_cart_item(int customer_id,int product_id,int quantity){
    
    int fd = open("customers.dat", O_RDWR, 0777);
    struct Customer temp_customer;

    struct Product *product = find_product_by_id(product_id);
    
    if(product == NULL){
        printf("Product does not exist.\n");
        return -1;
    }

    if(quantity > product->quantity){
        printf("Insufficient Quantity\n");
        return -1;
    }

    while(read(fd,&temp_customer,sizeof(struct Customer)) != 0){

        if(temp_customer.customer_id==customer_id && temp_customer.assigned==1){

            for(int i = 0; i < MAX_CART_ITEMS; i++){

                if(temp_customer.cart_items[i].id == product_id){
                    temp_customer.cart_items[i].quantity = quantity;
                    lseek(fd,-sizeof(struct Customer),SEEK_CUR);
                    write(fd,&temp_customer,sizeof(struct Customer));
                    printf("Updated Cart Item\n");
                    return 1;
                }

            }
            printf("Item not found in cart\n");
            return -1;
        }
    }

    printf("Could not find Customer\n");
    return -1;

}


int delete_cart_item(int customer_id,int product_id){
    
    int fd = open("customers.dat", O_RDWR, 0777);
    struct Customer temp_customer;

    struct Product *product = find_product_by_id(product_id);
    
    if(product == NULL){
        printf("Product does not exist.\n");
        return -1;
    }

    while(read(fd,&temp_customer,sizeof(struct Customer)) != 0){

        if(temp_customer.customer_id==customer_id && temp_customer.assigned==1){

            for(int i = 0; i < MAX_CART_ITEMS; i++){

                if(temp_customer.cart_items[i].id == product_id){
                    temp_customer.cart_items[i].id = -1;
                    lseek(fd,-sizeof(struct Customer),SEEK_CUR);
                    write(fd,&temp_customer,sizeof(struct Customer));
                    printf("Deleted Cart Item\n");
                    return 1;
                }

            }
            printf("Item not found in cart\n");
            return -1;
        }
    }

    printf("Could not find Customer\n");
    return -1;

}

void send_cart_items(int customer_id, int nsd){
    
    struct Customer temp_customer;
    int fd = open("customers.dat",O_RDWR, 0777);

    while(read(fd,&temp_customer,sizeof(struct Customer)) != 0){
        
        if(temp_customer.customer_id==customer_id){
            
            write(nsd,temp_customer.cart_items,sizeof(temp_customer.cart_items));
            return ;

        }
    }
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
                        printf("Sending Products\n");
                        send_products(nsd);
                    }else if(choice==6){
                        break;
                    }
                }

            }else if(type==1){

                int user_id;
                read(nsd,&user_id,sizeof(int));
                
                if(user_id == -1){
                    user_id=generate_new_customerid();
                    write(nsd,&user_id,sizeof(int));
                } // Check if that customer with user_id exists 
                
                printf("Connected to a user of id: %d\n",user_id);

                while(1){
                    int choice=0;
                    read(nsd, &choice, sizeof(int));
                    if(choice==1){
                        
                        printf("Sending Products\n");
                        send_products(nsd);
                   
                    }else if (choice==2){
                        
                        int product_id=0,quantity=0,ret=0;
                        read(nsd,&product_id,sizeof(int));
                        read(nsd,&quantity,sizeof(int));
                        ret = add_cart_item(user_id,product_id,quantity);
                        write(nsd,&ret,sizeof(int));

                    }else if (choice==3){

                        send_cart_items(user_id,nsd);

                    }else if (choice == 4){

                        int product_id=0,quantity=0,ret=0;
                        read(nsd,&product_id,sizeof(int));
                        read(nsd,&quantity,sizeof(int));
                        ret = update_cart_item(user_id,product_id,quantity);
                        write(nsd,&ret,sizeof(int));

                    }else if (choice == 5){

                        int product_id=0,ret=0;
                        read(nsd,&product_id,sizeof(int));
                        ret = delete_cart_item(user_id,product_id);
                        write(nsd,&ret,sizeof(int));

                    }else if (choice==7){
                        break;
                    }
                    
                }

            }
        }   
    }
    close(nsd);
    close(sd);
}