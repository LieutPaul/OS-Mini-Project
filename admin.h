#include "structs.h"
#ifndef ADMIN_H
#define ADMIN_H

int setup_admin_connection();
void admin_options(int);
void print_products(struct Product[], int);

#endif