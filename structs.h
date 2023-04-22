#ifndef STRUCTS_H
#define STRUCTS_H

struct Product{
    int id;
    int quantity;
    int price;
    char name[100];
};

struct Cart{
    int customer_id;
    struct Product products[];
};

#endif