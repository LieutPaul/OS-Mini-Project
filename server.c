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

int add_product(struct Product product, int nsd){
    
    int fd = open("products.dat", O_RDWR, 0777);
    int afd = open("admin_logs.txt", O_WRONLY | O_APPEND ,0777),b=0;
    char admin_log[1200];

    struct Product temp_product;
    while(read(fd,&temp_product,sizeof(struct Product)) != 0){
        if(temp_product.id == product.id){
            printf("Product with id exists\n");
            b = snprintf(admin_log,sizeof(admin_log),"Could not add product of id %d as it already exists.\n",product.id);
            write(afd,admin_log,b);
            close(fd);
            close(afd);
            return -1;
        }
    }
    
    lseek(fd,0,SEEK_SET);

    while(read(fd,&temp_product,sizeof(struct Product)) != 0){
        if(temp_product.id==-1){
            lseek(fd,-sizeof(struct Product),SEEK_CUR);
            write(fd,&product,sizeof(struct Product));
            printf("Added Product\n");
            b = snprintf(admin_log,sizeof(admin_log),"Added product of id %d.\n",product.id);
            write(afd,admin_log,b);
            close(fd);
            close(afd);
            return 1;
        }
    }
    printf("Could Not Add\n");
    b = snprintf(admin_log,sizeof(admin_log),"Could not add product of id %d.\n",product.id);
    write(afd,admin_log,b);
    close(fd);
    close(afd);
    return -2;

}

int delete_product(int id, int nsd){
    struct flock fl;
    int fd = open("products.dat",O_RDWR, 0777);
    int afd = open("admin_logs.txt", O_WRONLY | O_APPEND ,0777),b=0;
    char admin_log[1200];

    struct Product temp_product;

    while(read(fd,&temp_product,sizeof(struct Product)) != 0){
        if(temp_product.id==id){
            
            temp_product.id=-1;
            lseek(fd,-sizeof(struct Product),SEEK_CUR);

            fl.l_whence=SEEK_CUR; 
            fl.l_start=0;        
            fl.l_len=sizeof(struct Product);        
            fl.l_pid=getpid(); 
            fl.l_type=F_WRLCK;
            fcntl(fd, F_SETLKW, &fl);
            
            write(fd,&temp_product,sizeof(struct Product));

            fl.l_type=F_UNLCK;
            fcntl(fd, F_SETLKW, &fl); 

            printf("Deleted Product\n");
            
            b = snprintf(admin_log,sizeof(admin_log),"Deleted product of id %d.\n",id);
            write(afd,admin_log,b);
            close(fd);
            close(afd);
            return 1;
        }
    }

    printf("Could Not Find Product\n");
    b = snprintf(admin_log,sizeof(admin_log),"Could not delete product of id %d.\n",id);
    write(afd,admin_log,b);
    close(fd);
    close(afd);
    return -1;
}

int update_price(int id, int price, int nsd){
    struct flock fl;
    int fd = open("products.dat", O_RDWR, 0777);
    int afd = open("admin_logs.txt", O_WRONLY | O_APPEND ,0777),b=0;
    char admin_log[1200];

    struct Product temp_product;

    while(read(fd,&temp_product,sizeof(struct Product)) != 0){
        if(temp_product.id==id){

            temp_product.price=price;
            lseek(fd,-sizeof(struct Product),SEEK_CUR);

            fl.l_whence=SEEK_CUR; 
            fl.l_start=0;        
            fl.l_len=sizeof(struct Product);        
            fl.l_pid=getpid(); 
            fl.l_type=F_WRLCK;
            fcntl(fd, F_SETLKW, &fl);

            write(fd,&temp_product,sizeof(struct Product));

            fl.l_type=F_UNLCK;
            fcntl(fd, F_SETLKW, &fl);
            
            b = snprintf(admin_log,sizeof(admin_log),"Update price of product of id %d to %d.\n",id,price);
            write(afd,admin_log,b);
            
            printf("Updated Product\n");
            close(fd);
            close(afd);
            return 1;
        }
    }
    printf("Could Not Find Product\n");
    
    b = snprintf(admin_log,sizeof(admin_log),"Could not update price of product of id %d.\n",id);
    write(afd,admin_log,b);
    
    close(fd);
    close(afd);

    return -1;
}

int update_quantity(int id, int quantity, int nsd){
    struct flock fl;
    int fd = open("products.dat",O_RDWR, 0777);
    int afd = open("admin_logs.txt", O_WRONLY | O_APPEND ,0777),b=0;
    char admin_log[1200];

    struct Product temp_product;
    while(read(fd,&temp_product,sizeof(struct Product)) != 0){
        if(temp_product.id == id){

            temp_product.quantity = quantity;
            lseek(fd,-sizeof(struct Product),SEEK_CUR);

            fl.l_whence=SEEK_CUR; 
            fl.l_start=0;        
            fl.l_len=sizeof(struct Product);        
            fl.l_pid=getpid(); 
            fl.l_type=F_WRLCK;
            fcntl(fd, F_SETLKW, &fl);

            write(fd,&temp_product,sizeof(struct Product));

            fl.l_type=F_UNLCK;
            fcntl(fd, F_SETLKW, &fl);
            
            printf("Updated Product\n");

            b = snprintf(admin_log,sizeof(admin_log),"Updated quantity of product of id %d to %d.\n",id,quantity);
            write(afd,admin_log,b);

            close(fd);
            close(afd);

            return 1;
        }
    }
    
    printf("Could Not Find Product\n");
    b = snprintf(admin_log,sizeof(admin_log),"Could not update quantity of product of id %d.\n",id);
    write(afd,admin_log,b);
    
    close(fd);
    close(afd);
    
    return -1;
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
    close(fd);
}

int generate_new_customerid(){

    struct Customer temp_customer;
    int fd = open("customers.dat",O_RDWR, 0777);

    while(read(fd,&temp_customer,sizeof(struct Customer)) != 0){
        if(temp_customer.assigned == 0){
            temp_customer.assigned = 1;
            lseek(fd,-sizeof(struct Customer),SEEK_CUR);
            write(fd,&temp_customer,sizeof(struct Customer));
            close(fd);
            return temp_customer.customer_id;
        }
    }
    close(fd);
    return -1;
}

struct Product* find_product_by_id(int id){
    struct Product *product = (struct Product *) malloc(sizeof(struct Product));
    int fd = open("products.dat", O_RDONLY, 0777);
    while(read(fd,product,sizeof(struct Product)) != 0){
        if(product->id == id){
            close(fd);
            return product;
        }
    }
    close(fd);
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
                        close(fd);
                        return 1;

                    }else{
                        printf("Insufficient Quantity\n");
                        close(fd);
                        return -1;
                    }
                }
            }
            printf("Cart Limit Reached\n");
            close(fd);
            return -1;
        }
    }

    close(fd);
    printf("Could not find Customer\n");
    return -1;
}

int update_cart_item(int customer_id,int product_id,int quantity){
    
    int fd = open("customers.dat", O_RDWR, 0777);
    struct Customer temp_customer;

    struct Product *product = find_product_by_id(product_id);
    
    if(product == NULL){
        printf("Product does not exist.\n");
        close(fd);
        return -1;
    }

    if(quantity > product->quantity){
        printf("Insufficient Quantity\n");
        close(fd);
        return -1;
    }
    
    struct flock fl;

    while(read(fd,&temp_customer,sizeof(struct Customer)) != 0){

        if(temp_customer.customer_id==customer_id && temp_customer.assigned==1){

            for(int i = 0; i < MAX_CART_ITEMS; i++){

                if(temp_customer.cart_items[i].id == product_id){
                    
                    temp_customer.cart_items[i].quantity = quantity;
                    lseek(fd,-sizeof(struct Customer),SEEK_CUR);

                    fl.l_whence=SEEK_CUR; 
                    fl.l_start=0;        
                    fl.l_len=sizeof(struct Customer);        
                    fl.l_pid=getpid(); 
                    fl.l_type=F_WRLCK;
                    fcntl(fd, F_SETLKW, &fl);

                    write(fd,&temp_customer,sizeof(struct Customer));

                    fl.l_type=F_UNLCK;
                    fcntl(fd, F_SETLKW, &fl);

                    printf("Updated Cart Item\n");
                    close(fd);
                    return 1;
                }

            }
            printf("Item not found in cart\n");
            close(fd);
            return -1;
        }
    }

    printf("Could not find Customer\n");
    close(fd);
    return -1;

}


int delete_cart_item(int customer_id,int product_id){
    
    int fd = open("customers.dat", O_RDWR, 0777);
    struct Customer temp_customer;

    struct Product *product = find_product_by_id(product_id);
    
    if(product == NULL){
        printf("Product does not exist.\n");
        close(fd);
        return -1;
    }

    struct flock fl;
    
    while(read(fd,&temp_customer,sizeof(struct Customer)) != 0){

        if(temp_customer.customer_id==customer_id && temp_customer.assigned==1){

            for(int i = 0; i < MAX_CART_ITEMS; i++){

                if(temp_customer.cart_items[i].id == product_id){
                    
                    temp_customer.cart_items[i].id = -1;
                    lseek(fd,-sizeof(struct Customer),SEEK_CUR);
                    
                    fl.l_whence=SEEK_CUR; 
                    fl.l_start=0;        
                    fl.l_len=sizeof(struct Customer);        
                    fl.l_pid=getpid(); 
                    fl.l_type=F_WRLCK;
                    fcntl(fd, F_SETLKW, &fl);

                    write(fd,&temp_customer,sizeof(struct Customer));

                    fl.l_type=F_UNLCK;
                    fcntl(fd, F_SETLKW, &fl);
                    
                    printf("Deleted Cart Item\n");
                    close(fd);
                    return 1;
                }

            }
            printf("Item not found in cart\n");
            close(fd);
            return -1;
        }
    }

    printf("Could not find Customer\n");
    close(fd);
    return -1;

}

void send_cart_items(int customer_id, int nsd){
    
    struct Customer temp_customer;
    int fd = open("customers.dat",O_RDWR, 0777);

    while(read(fd,&temp_customer,sizeof(struct Customer)) != 0){
        
        if(temp_customer.customer_id==customer_id && temp_customer.assigned==1){
            
            write(nsd,temp_customer.cart_items,sizeof(temp_customer.cart_items));
            close(fd);
            return ;

        }
    }
}

void payment_portal(int user_id, int nsd){
    
    int cfd = open("customers.dat",O_RDWR,0777);
    int pfd = open("products.dat",O_RDWR,0777);
    int lfd = open("transaction_logs.txt",O_WRONLY | O_APPEND,0777);
    int b=0;

    struct Customer customer,temp_customer;
    struct Product temp_product;
    struct flock fl;
    char logtext[1200];

    while(read(cfd,&customer,sizeof(struct Customer)) != 0){
        if(customer.customer_id==user_id && customer.assigned==1){
            break;
        }
    }
    
    int found_product = 0;
    
    for(int i=0;i<MAX_CART_ITEMS;i++){
        if(customer.cart_items[i].id != -1){
            
            found_product = 0;
            lseek(pfd,0,SEEK_SET);

            while(read(pfd,&temp_product,sizeof(struct Product)) != 0){
                if(temp_product.id == customer.cart_items[i].id){
                    
                    found_product=1;
                    lseek(pfd,-sizeof(struct Product),SEEK_CUR);
                    fl.l_whence=SEEK_CUR; 
                    fl.l_start=0;        
                    fl.l_len=sizeof(struct Product);        
                    fl.l_pid=getpid(); 
                    fl.l_type=F_WRLCK;
                    fcntl(pfd, F_SETLKW, &fl);

                    if(temp_product.quantity >= customer.cart_items[i].quantity){
                        temp_product.quantity -= customer.cart_items[i].quantity;
                        write(pfd,&temp_product,sizeof(struct Product));
                        printf("Bought Item : %d.\n",temp_product.id);
                        b = snprintf(logtext,sizeof(logtext),"Customer of id %d bought %d amount of product of id %d.\n",user_id,customer.cart_items[i].quantity,customer.cart_items[i].id);
                        write(lfd,logtext,b);
                    }else{
                        printf("Insufficient Quantity : %d.\n",temp_product.id);
                        b = snprintf(logtext,sizeof(logtext),"Customer of id %d could not buy %d amount of product of id %d because of insufficient quantity.\n",user_id,customer.cart_items[i].quantity,customer.cart_items[i].id);
                        write(lfd,logtext,b);
                    }

                    fl.l_type=F_UNLCK;
                    fcntl(pfd, F_SETLKW, &fl);
                    break;
                }
            }
            
            if (found_product==0){
                b = snprintf(logtext,sizeof(logtext),"Customer of id %d could not buy %d amount of product of id %d because product does not exist.\n",user_id,customer.cart_items[i].quantity,customer.cart_items[i].id);
                write(lfd,logtext,b);
                printf("Product not found : %d.\n",customer.cart_items[i].id);
            }
        }
        customer.cart_items[i].id = -1;
    }

    lseek(cfd,0,SEEK_SET);

    while( read(cfd,&temp_customer,sizeof(struct Customer)) != 0 ){
        if(temp_customer.customer_id == customer.customer_id){
            lseek(cfd,-sizeof(struct Customer),SEEK_CUR);
            write(cfd,&customer,sizeof(struct Customer));
            break;
        }
    }
    
    close(cfd);
    close(pfd);
    close(lfd);

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
                    int choice=0,ret=0;
                    read(nsd, &choice, sizeof(int));
                    
                    if(choice==1){
                        struct Product product;
                        read(nsd,&product,sizeof(struct Product));
                        ret = add_product(product,nsd);
                        write(nsd,&ret,sizeof(int));
                    }else if(choice==2){
                        int id;
                        read(nsd,&id,sizeof(int));
                        ret = delete_product(id,nsd);
                        write(nsd,&ret,sizeof(int));
                    }else if(choice==3){
                        int id,price;
                        read(nsd,&id,sizeof(int));
                        read(nsd,&price,sizeof(int));
                        ret = update_price(id,price,nsd);
                        write(nsd,&ret,sizeof(int));
                    }else if(choice==4){
                        int id,quantity;
                        read(nsd,&id,sizeof(int));
                        read(nsd,&quantity,sizeof(int));
                        ret = update_quantity(id,quantity,nsd);
                        write(nsd,&ret,sizeof(int));
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

                    }else if(choice==6){

                        payment_portal(user_id,nsd);

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