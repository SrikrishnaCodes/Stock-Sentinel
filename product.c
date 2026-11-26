#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>

#include "inventory.h"
#include "menu.h"
#include "product.h"

// function to display the product management menu and handle user choices
void productMenu(Inventory *inv) {
    int choice;

    do {
        printf(BLUE "\n=========== PRODUCT MANAGEMENT ===========" RESET "\n");
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

    printf(YELLOW "[INFO] Returning to Main Menu..." RESET "\n");
}

void addProduct(Inventory *inv) {
    printf(BLUE "\n=========== ADD NEW PRODUCT ===========" RESET "\n");

    if (inv->productCount >= MAX_PRODUCTS) {
        printf(RED "[ERROR] Cannot add more products.\nMaximum limit reached." RESET "\n");
        return;
    }

    // Create a temporary product structure to hold the new product details
    Product newProduct = {0};

    while (true) {
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
            printf(RED "[ERROR] Product ID already exists.\nPlease use a unique ID." RESET "\n");
            printf("------------------------------------------\n");
        } 
        else
            break; // Exit the loop if the ID is unique
    }

    printf("Enter Product Name: ");
    fgets(newProduct.name, sizeof(newProduct.name), stdin);
    // Remove newline character that is at the end
    newProduct.name[strcspn(newProduct.name, "\n")] = '\0'; 

    printf("Enter Product Category: ");
    fgets(newProduct.category, sizeof(newProduct.category), stdin);
    // Remove newline character that is at the end
    newProduct.category[strcspn(newProduct.category, "\n")] = '\0';

    while (true) {
        printf("Enter Purchase Price: ");
        newProduct.purchasePrice = get_float();

        if (newProduct.purchasePrice < 0) {
            printf(RED "[ERROR] Purchase price cannot be negative." RESET "\n");
            printf("------------------------------------------\n");
        }
        else {
            break;
        }
    }

    // Only accept selling price if it's greater than or equal to purchase price
    while (true) {
        printf("Enter Selling Price: ");
        newProduct.sellingPrice = get_float();

        if (newProduct.sellingPrice < newProduct.purchasePrice) {
            printf(RED "[ERROR] Selling price cannot be less than purchase price." RESET "\n");
            printf("------------------------------------------\n");
        }
        else {
            break;
        }
    }

    while (true) {
        printf("Enter Minimum Stock Level: ");
        newProduct.minStockLevel = get_int();

        if (newProduct.minStockLevel < 0) {
            printf(RED "[ERROR] Minimum stock level cannot be negative." RESET "\n");
            printf("------------------------------------------\n");
        }
        else {
            break;
        }
    }

    // Add the new product to the inventory and increment the product count
    inv->products[inv->productCount++] = newProduct;

    printf("------------------------------------------\n");
    printf(GREEN "[SUCCESS] Product added successfully!" RESET "\n");
    printf("Product ID: %d\n", newProduct.id);
    printf("Product Name: %s\n", newProduct.name);
    printf("------------------------------------------\n");
}

void viewProducts(Inventory *inv) {
    printf(BLUE "\n=========== VIEW ALL PRODUCTS ===========" RESET "\n");

    if (inv->productCount == 0) {
        printf(YELLOW "[INFO] No products available in the inventory." RESET "\n");
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

void searchProduct(Inventory *inv) {
    int choice;

    printf(BLUE "\n=========== SEARCH PRODUCT ===========" RESET "\n");
    printf("1. Search by Product ID\n");
    printf("2. Search by Product Name\n");
    choice = get_choice(2);

    if (choice == 1) {
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
        printf(YELLOW "[INFO] Product with ID %d not found." RESET "\n", searchId);
    }

    else {
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
        printf(YELLOW "[INFO] Product with Name %s not found." RESET "\n", name);
    }
}

void updateProduct(Inventory *inv) {
    int searchId, choice;

    printf(BLUE "\n=========== UPDATE PRODUCT ===========" RESET "\n");

    if (inv->productCount == 0) {
        printf(YELLOW "[INFO] No products available in the inventory." RESET "\n");
        return;
    }

    printf("Enter Product ID to update: ");
    searchId = get_int();

    for (int i = 0; i < inv->productCount; i++) {
        if (inv->products[i].id == searchId) {
            printf("1. Update Product Name\n");
            printf("2. Update Product Category\n");
            printf("3. Update Purchase Price\n");
            printf("4. Update Selling Price\n");
            printf("5. Update Minimum Stock Level\n");
            choice = get_choice(5);

            switch (choice) {
                case 1:
                    printf("Enter new Product Name: ");
                    fgets(inv->products[i].name, sizeof(inv->products[i].name), stdin);
                    inv->products[i].name[strcspn(inv->products[i].name, "\n")] = '\0';
                    break;
                case 2:
                    printf("Enter new Product Category: ");
                    fgets(inv->products[i].category, sizeof(inv->products[i].category), stdin);
                    inv->products[i].category[strcspn(inv->products[i].category, "\n")] = '\0';
                    break;
                case 3:
                    while (true) {
                        float newPurchasePrice;
                        printf("Enter new Purchase Price: ");
                        newPurchasePrice = get_float();

                        if (newPurchasePrice < 0) {
                            printf(RED "[ERROR] Purchase price cannot be negative." RESET "\n");
                            printf("------------------------------------------\n");
                        }
                        else if (newPurchasePrice > inv->products[i].sellingPrice) {
                            printf(RED "[ERROR] Purchase price cannot be greater than selling price." RESET "\n");
                            printf("------------------------------------------\n");
                        }
                        else {
                            inv->products[i].purchasePrice = newPurchasePrice;
                            break;
                        }
                    }
                    break;
                case 4:
                    while (true) {
                        float newSellingPrice;
                        printf("Enter new Selling Price: ");
                        newSellingPrice = get_float();

                        if (newSellingPrice < inv->products[i].purchasePrice) {
                            printf(RED "[ERROR] Selling price cannot be less than purchase price." RESET "\n");
                            printf("------------------------------------------\n");
                        }
                        else {
                            inv->products[i].sellingPrice = newSellingPrice;
                            break;
                        }
                    }
                    break;
                case 5:
                    while (true) {
                        int newMinStockLevel;
                        printf("Enter new Minimum Stock Level: ");
                        newMinStockLevel = get_int();

                        if (newMinStockLevel < 0) {
                            printf(RED "[ERROR] Minimum stock level cannot be negative." RESET "\n");
                            printf("------------------------------------------\n");
                        }
                        else {
                            inv->products[i].minStockLevel = newMinStockLevel;
                            break;
                        }
                    }
                    break;
            }

            printf(GREEN "[SUCCESS] Product updated successfully!" RESET "\n");
            return;
        }
    }

    printf(RED "[ERROR] Product with ID %d not found." RESET "\n", searchId);
}

void deleteProduct(Inventory *inv) {
    int searchId;
    printf(BLUE "\n=========== DELETE PRODUCT ===========" RESET "\n");

    if (inv->productCount == 0) {
        printf(YELLOW "[INFO] No products available in the inventory." RESET "\n");
        return;
    }

    printf("Enter Product ID to delete: ");
    searchId = get_int();

    for (int i = 0; i < inv->productCount; i++) {
        if (inv->products[i].id == searchId) {
            printf(YELLOW "[INFO] Product Found: %s" RESET "\n", inv->products[i].name);
            printf("Are you sure you want to delete this product?\n");
            printf("1. Yes\n");
            printf("2. No\n");

            if (get_choice(2) != 1) {
                printf(YELLOW "[INFO] Delete cancelled." RESET "\n");
                return;
            }

            // Shift all products after the found product one position to the left
            for (int j = i; j < inv->productCount - 1; j++) {
                inv->products[j] = inv->products[j + 1];
            }
            inv->productCount--; // Decrease the product count
            printf(GREEN "[SUCCESS] Product with ID %d deleted successfully!" RESET "\n", searchId);
            return;
        }
    }
    printf(RED "[ERROR] Product with ID %d not found." RESET "\n", searchId);
}