#ifndef PRODUCT_H
#define PRODUCT_H

#include "inventory.h"

void productMenu(Inventory *);
void addProduct(Inventory *);
void viewProducts(Inventory *);
void searchProduct(Inventory *);
void updateProduct(Inventory *);
void deleteProduct(Inventory *);

#endif