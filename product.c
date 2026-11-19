#include <stdio.h>
#include <string.h>
#include <strings.h>

#include "inventory.h"
#include "menu.h"
#include "product.h"

// function to display the product management menu and handle user choices
void productMenu(Inventory *inv){
    int choice;

    do {
        printf("\n=========== PRODUCT MANAGEMENT ===========\n");
        printf("1. Add Product\n");
        printf("2. View All Products\n");
        printf("3. Search Product\n");
        printf("4. Update Product\n");
        printf("5. Delete Product\n");
        printf("6. Back to Main Menu\n");

        choice = get_choice(6);

        switch (choice) {
            case 1:
                addProduct(inv);
                break;
            case 2:
                viewProducts(inv);
                break;
            case 3:
                searchProduct(inv);
                break;
            case 4:
                updateProduct(inv);
                break;
            case 5:
                deleteProduct(inv);
                break;
        }
    } while (choice != 6);

    printf("%s[INFO] Returning to Main Menu...%s\n", YELLOW, RESET);
}

void addProduct(Inventory *inv){
    printf("\n  ========== ADD NEW PRODUCT ==========\n");

    if (inv->productCount >= MAX_PRODUCTS) {
        printf("%s[ERROR] Cannot add more products.\nMaximum limit reached.%s\n", RED, RESET);
        return;
    }

    // Create a temporary product structure to hold the new product details
    Product newProduct = {0};

    while (1) {
        printf("Enter Product ID: ");
        newProduct.id = get_int();

        // Check for duplicate product ID
        int isDuplicate = 0;
        for (int i = 0; i < inv->productCount; i++) {
            if (inv->products[i].id == newProduct.id) {
                isDuplicate = 1;
                break;
            }
        }

        if (isDuplicate) {
            printf("%s[ERROR] Product ID already exists.\nPlease use a unique ID.%s\n", RED, RESET);
            printf("------------------------------------------\n");
        } 
        else
            break; // Exit the loop if the ID is unique
    }

    printf("Enter Product Name: ");
    fgets(newProduct.name, sizeof(newProduct.name), stdin);
    // Remove newline character that is at the end
    newProduct.name[strcspn(newProduct.name, "\n")] = 0; 

    printf("Enter Product Category: ");
    fgets(newProduct.category, sizeof(newProduct.category), stdin);
    // Remove newline character that is at the end
    newProduct.category[strcspn(newProduct.category, "\n")] = 0;

    printf("Enter Purchase Price: ");
    scanf("%f", &newProduct.purchasePrice);

    // Only accept selling price if it's greater than or equal to purchase price
    do {
        printf("Enter Selling Price: ");
        scanf("%f", &newProduct.sellingPrice);

        if (newProduct.sellingPrice < newProduct.purchasePrice) {
            printf("%s[ERROR] Selling price cannot be less than purchase price.%s\n", RED, RESET);
            printf("------------------------------------------\n");
        }
    } while (newProduct.sellingPrice < newProduct.purchasePrice);

    printf("Enter Minimum Stock Level: ");
    scanf("%d", &newProduct.minStockLevel);

    // Add the new product to the inventory and increment the product count
    inv->products[inv->productCount++] = newProduct;

    printf("------------------------------------------\n");
    printf("%s[SUCCESS] Product added successfully!%s\n", GREEN, RESET);
    printf("Product ID: %d\n", newProduct.id);
    printf("Product Name: %s\n", newProduct.name);
    printf("------------------------------------------\n");
}

void viewProducts(Inventory *inv){
    printf("\n  ========== VIEW ALL PRODUCTS ==========\n");

    if (inv->productCount == 0) {
        printf("%s[INFO] No products available in the inventory.%s\n", YELLOW, RESET);
        return;
    }

    for (int i = 0; i < inv->productCount; i++) {
        printf("------------------------------------------\n");
        printf("Product ID: %d\n", inv->products[i].id);
        printf("Product Name: %s\n", inv->products[i].name);
        printf("Category: %s\n", inv->products[i].category);
        printf("Purchase Price: %.2f\n", inv->products[i].purchasePrice);
        printf("Selling Price: %.2f\n", inv->products[i].sellingPrice);
        printf("Minimum Stock Level: %d\n", inv->products[i].minStockLevel);
    }
    printf("------------------------------------------\n");
}

void searchProduct(Inventory *inv){
    int choice;

    printf("\n  ========== SEARCH PRODUCT ==========\n");
    printf("1. Search by Product ID\n");
    printf("2. Search by Product Name\n");
    choice = get_choice(2);

    if (choice == 1){
        // Search for the product by ID

        int searchId;
        printf("Enter Product ID to search for: ");
        searchId = get_int();

        for (int i = 0; i < inv->productCount; i++) {
            if (inv->products[i].id == searchId) {
                printf("------------------------------------------\n");
                printf("Product Found:\n");
                printf("ID: %d\n", inv->products[i].id);
                printf("Name: %s\n", inv->products[i].name);
                printf("Category: %s\n", inv->products[i].category);
                printf("Purchase Price: %.2f\n", inv->products[i].purchasePrice);
                printf("Selling Price: %.2f\n", inv->products[i].sellingPrice);
                printf("Minimum Stock Level: %d\n", inv->products[i].minStockLevel);
                printf("------------------------------------------\n");
                return;
            }
        }

        // If the product is not found
        printf("%s[INFO] Product with ID %d not found.%s\n", YELLOW, searchId, RESET);
    }

    else{
        // Search product by Name

        char name[50];
        printf("Enter name of product to search for: ");
        fgets(name, sizeof(name), stdin);
        name[strcspn(name, "\n")] = '\0';

        for (int i = 0; i < inv->productCount; i++) {
            if (strcasecmp(inv->products[i].name, name) == 0) {
                printf("------------------------------------------\n");
                printf("Product Found:\n");
                printf("ID: %d\n", inv->products[i].id);
                printf("Name: %s\n", inv->products[i].name);
                printf("Category: %s\n", inv->products[i].category);
                printf("Purchase Price: %.2f\n", inv->products[i].purchasePrice);
                printf("Selling Price: %.2f\n", inv->products[i].sellingPrice);
                printf("Minimum Stock Level: %d\n", inv->products[i].minStockLevel);
                printf("------------------------------------------\n");
                return;
            }
        }
        // If the product is not found
        printf("%s[INFO] Product with Name %s not found.%s\n", YELLOW, name, RESET);
    }
}

void updateProduct(Inventory *inv){
    int searchId, choice;
    printf("\n  ========== UPDATE PRODUCT ==========\n");
    printf("Enter Product ID to update: ");
    searchId = get_int();
    printf("%s[INFO] Searching for Product ID: %d%s\n", YELLOW, searchId, RESET);

    for (int i = 0; i < inv->productCount; i++){
        if (inv->products[i].id == searchId){
            printf("%s[INFO] Product Found.%s\n", YELLOW, RESET);
            printf("1. Update Product Name\n");
            printf("2. Update Product Category\n");
            printf("3. Update Purchase Price\n");
            printf("4. Update Selling Price\n");
            printf("5. Update Minimum Stock Level\n"); 
            choice = get_choice(5);

            switch (choice){
                case 1:
                    printf("Enter new Product Name: ");
                    fgets(inv->products[i].name, sizeof(inv->products[i].name), stdin);
                    inv->products[i].name[strcspn(inv->products[i].name, "\n")] = 0; 
                    break;
                case 2:
                    printf("Enter new Product Category: ");
                    fgets(inv->products[i].category, sizeof(inv->products[i].category), stdin);
                    inv->products[i].category[strcspn(inv->products[i].category, "\n")] = 0; 
                    break;
                case 3:
                    printf("Enter new Purchase Price: ");
                    scanf("%f", &inv->products[i].purchasePrice);
                    break;
                case 4:
                    while (1) {
                        printf("Enter new Selling Price: ");
                        scanf("%f", &inv->products[i].sellingPrice);
                        if (inv->products[i].sellingPrice < inv->products[i].purchasePrice) {
                            printf("%s[ERROR] Selling price cannot be less than purchase price.%s\n", RED, RESET);
                            printf("------------------------------------------\n");
                        } 
                        else
                            break; // Exit the loop if the selling price is valid
                    }
                    break;
                case 5:
                    printf("Enter new Minimum Stock Level: ");
                    scanf("%d", &inv->products[i].minStockLevel);
                    break;
            }
            printf("%s[SUCCESS] Product updated successfully!%s\n", GREEN, RESET);
            return;
        }
    }
    printf("%s[ERROR] Product with ID %d not found.%s\n", RED, searchId, RESET);
}

void deleteProduct(Inventory *inv){
    int searchId;
    printf("\n  ========== DELETE PRODUCT ==========\n");

    if (inv->productCount == 0) {
        printf("%s[INFO] No products available in the inventory.%s\n", YELLOW, RESET);
        return;
    }

    printf("Enter Product ID to delete: ");
    searchId = get_int();
    printf("%s[INFO] Searching for Product ID: %d%s\n", YELLOW, searchId, RESET);

    for (int i = 0; i < inv->productCount; i++){
        if (inv->products[i].id == searchId){
            // Shift all products after the found product one position to the left
            for (int j = i; j < inv->productCount - 1; j++){
                inv->products[j] = inv->products[j + 1];
            }
            inv->productCount--; // Decrease the product count
            printf("%s[SUCCESS] Product with ID %d deleted successfully!%s\n", GREEN, searchId, RESET);
            return;
        }
    }
    printf("%s[ERROR] Product with ID %d not found.%s\n", RED, searchId, RESET);
}