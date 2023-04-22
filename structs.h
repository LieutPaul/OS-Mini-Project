#ifndef STRUCTS_H
#define STRUCTS_H

#define MAX_CART_ITEMS 100
#define MAX_PRODUCTS 100
#define MAX_CUSTOMERS 100

struct Product{
    int id;
    int quantity;
    int price;
    char name[100];
};

struct Customer{
    int customer_id;
    struct Product cart_items[100];
};

#endif