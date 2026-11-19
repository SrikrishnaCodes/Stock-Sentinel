#ifndef INVENTORY_H
#define INVENTORY_H

#define MAX_PRODUCTS 200
#define MAX_BATCHES 500

typedef struct {
    int id;
    char name[50];
    char category[30];
    float purchasePrice;
    float sellingPrice;
    int minStockLevel;
} Product;

typedef struct {
    int batchId;
    int productId;
    int quantity;
    int day;
    int month;
    int year;
} Batch;

typedef struct {
    Product products[MAX_PRODUCTS];
    Batch batches[MAX_BATCHES];

    int productCount;
    int batchCount;
} Inventory;

#endif
