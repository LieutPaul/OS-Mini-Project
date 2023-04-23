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
        
        int new=0,cust_id=0;
        printf("Are you a new cutomer(1) or existing customer(0): ");
        scanf("%d",&new);
        
        if(new == 1){
            cust_id=-1;
        }else{  
            printf("Please enter your customer id: ");
            scanf("%d",&cust_id);
        }
        
        sd = setup_user_connection(cust_id);

        if(new == 1){
            read(sd,&cust_id,sizeof(int));
            printf("\nYour new customer id is : %d\n",cust_id);
        }

        user_options(sd);
    
    }else if(choice==2){
        
        sd = setup_admin_connection();
        admin_options(sd);
    
    }
}