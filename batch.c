#include <stdbool.h>
#include <stdio.h>

#include "inventory.h"
#include "menu.h"
#include "batch.h"

void batchMenu(Inventory *inv) {
    int choice;

    do {
        printf(BLUE "\n=========== BATCH MANAGEMENT ===========" RESET "\n");
        printf("1. Add New Batch\n");
        printf("2. View All Batches\n");
        printf("3. View Batches by Product\n");
        printf("4. Update Batch\n");
        printf("5. Delete Batch\n");
        printf("6. Back to Main Menu\n");

        choice = get_choice(6);

        switch (choice) {
            case 1:
                addBatch(inv);
                break;

            case 2:
                viewBatches(inv);
                break;

            case 3:
                viewBatchesByProduct(inv);
                break;

            case 4:
                updateBatch(inv);
                break;
            case 5:
                deleteBatch(inv);
                break;
        }

    } while (choice != 6);

    printf(YELLOW "[INFO] Returning to Main Menu..." RESET "\n");
}

static bool isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

static bool isValidDate(int day, int month, int year) {
    if (year < 1900 || month < 1 || month > 12 || day < 1) {
        return false;
    }

    int daysInMonth;
    switch (month) {
        case 2:
            daysInMonth = isLeapYear(year) ? 29 : 28;
            break;
        case 4:
        case 6:
        case 9:
        case 11:
            daysInMonth = 30;
            break;
        default:
            daysInMonth = 31;
            break;
    }

    return day <= daysInMonth;
}

void addBatch(Inventory *inv) {

    printf(BLUE "\n=========== ADD NEW BATCH ===========" RESET "\n");

    // Check whether batch storage is full
    if (inv->batchCount >= MAX_BATCHES) {
        printf(RED "[ERROR] Maximum batch limit reached." RESET "\n");
        return;
    }

    Batch newBatch = {0};

    // Get Product ID
    printf("Enter Product ID: ");
    newBatch.productId = get_int();

    // Check whether the product exists
    int productFound = 0;
    for (int i = 0; i < inv->productCount; i++) {
        if (inv->products[i].id == newBatch.productId) {
            productFound = 1;
            break;
        }
    }

    if (!productFound) {
        printf(RED "[ERROR] Product not found." RESET "\n");
        return;
    }

    // Get Batch ID
    while (true) {
        printf("Enter Batch ID: ");
        newBatch.batchId = get_int();

        bool isDuplicate = false;
        for (int i = 0; i < inv->batchCount; i++) {
            if (inv->batches[i].batchId == newBatch.batchId) {
                isDuplicate = true;
                break;
            }
        }

        if (newBatch.batchId <= 0) {
            printf(RED "[ERROR] Batch ID must be a positive number." RESET "\n");
            printf("------------------------------------------\n");
        } else if (isDuplicate) {
            printf(RED "[ERROR] Batch ID already exists.\nPlease use a unique ID." RESET "\n");
            printf("------------------------------------------\n");
        } else {
            break;
        }
    }

    // Get quantity
    while (true) {
        printf("Enter Quantity: ");
        newBatch.quantity = get_int();

        if (newBatch.quantity <= 0) {
            printf(RED "[ERROR] Quantity must be greater than 0." RESET "\n");
            printf("------------------------------------------\n");
        } else {
            break;
        }
    }

    // Get expiry date
    while (true) {
        printf("Enter Expiry Date (DD MM YYYY): ");
        newBatch.day = get_int();
        newBatch.month = get_int();
        newBatch.year = get_int();

        if (!isValidDate(newBatch.day, newBatch.month, newBatch.year)) {
            printf(RED "[ERROR] Invalid expiry date.\nPlease enter a valid date (DD MM YYYY)." RESET "\n");
            printf("------------------------------------------\n");
        } else {
            break;
        }
    }

    // Store batch
    inv->batches[inv->batchCount++] = newBatch;

    printf(GREEN "[SUCCESS] Batch added successfully!" RESET "\n");
}

void viewBatches(Inventory *inv) {
    printf(BLUE "\n=========== VIEW ALL BATCHES ===========" RESET "\n");

    if (inv->batchCount == 0) {
        printf(YELLOW "[INFO] No batches available in the inventory." RESET "\n");
        return;
    }

    for (int i = 0; i < inv->batchCount; i++) {
        printf("------------------------------------------\n");
        printf("Batch ID: %d\n", inv->batches[i].batchId);
        printf("Product ID: %d\n", inv->batches[i].productId);
        printf("Quantity: %d\n", inv->batches[i].quantity);
        printf("Expiry Date: %02d/%02d/%04d\n", inv->batches[i].day, inv->batches[i].month, inv->batches[i].year);
    }
    printf("------------------------------------------\n");
}

void viewBatchesByProduct(Inventory *inv) {
    printf(BLUE "\n=========== VIEW BATCHES BY PRODUCT ===========" RESET "\n");

    if (inv->batchCount == 0) {
        printf(YELLOW "[INFO] No batches available in the inventory." RESET "\n");
        return;
    }

    int productId;
    printf("Enter Product ID to view batches: ");
    productId = get_int();

    int found = 0;
    for (int i = 0; i < inv->batchCount; i++) {
        if (inv->batches[i].productId == productId) {
            if (!found) {
                printf(GREEN "[INFO] Batches for Product ID %d:" RESET "\n", productId);
            }
            found = 1;
            printf("------------------------------------------\n");
            printf("Batch ID: %d\n", inv->batches[i].batchId);
            printf("Quantity: %d\n", inv->batches[i].quantity);
            printf("Expiry Date: %02d/%02d/%04d\n", inv->batches[i].day, inv->batches[i].month, inv->batches[i].year);
        }
    }

    if (!found) {
        printf(YELLOW "[INFO] No batches found for Product ID %d." RESET "\n", productId);
    }
}

void updateBatch(Inventory *inv) {
    int searchId;
    int choice;

    printf(BLUE "\n=========== UPDATE BATCH ===========" RESET "\n");

    if (inv->batchCount == 0) {
        printf(YELLOW "[INFO] No batches available in the inventory." RESET "\n");
        return;
    }

    printf("Enter Batch ID to update: ");
    searchId = get_int();

    for (int i = 0; i < inv->batchCount; i++) {
        if (inv->batches[i].batchId == searchId) {
            printf("1. Update Product ID\n");
            printf("2. Update Quantity\n");
            printf("3. Update Expiry Date\n");
            choice = get_choice(3);

            switch (choice) {
                case 1:
                    while (true) {
                        int newProductId;
                        printf("Enter new Product ID: ");
                        newProductId = get_int();

                        bool productFound = false;
                        for (int j = 0; j < inv->productCount; j++) {
                            if (inv->products[j].id == newProductId) {
                                productFound = true;
                                break;
                            }
                        }

                        if (!productFound) {
                            printf(RED "[ERROR] Product not found." RESET "\n");
                            printf("------------------------------------------\n");
                        } else {
                            inv->batches[i].productId = newProductId;
                            break;
                        }
                    }
                    break;
                case 2:
                    while (true) {
                        int newQuantity;
                        printf("Enter new Quantity: ");
                        newQuantity = get_int();

                        if (newQuantity <= 0) {
                            printf(RED "[ERROR] Quantity must be greater than 0." RESET "\n");
                            printf("------------------------------------------\n");
                        } else {
                            inv->batches[i].quantity = newQuantity;
                            break;
                        }
                    }
                    break;
                case 3:
                    while (true) {
                        int day;
                        int month;
                        int year;
                        printf("Enter new Expiry Date (DD MM YYYY): ");
                        day = get_int();
                        month = get_int();
                        year = get_int();

                        if (!isValidDate(day, month, year)) {
                            printf(RED "[ERROR] Invalid expiry date.\nPlease enter a valid date (DD MM YYYY)." RESET "\n");
                            printf("------------------------------------------\n");
                        } else {
                            inv->batches[i].day = day;
                            inv->batches[i].month = month;
                            inv->batches[i].year = year;
                            break;
                        }
                    }
                    break;
            }

            printf(GREEN "[SUCCESS] Batch updated successfully!" RESET "\n");
            return;
        }
    }

    printf(RED "[ERROR] Batch with ID %d not found." RESET "\n", searchId);
}

void deleteBatch(Inventory *inv) {
    printf(BLUE "\n=========== DELETE BATCH ===========" RESET "\n");

    if (inv->batchCount == 0) {
        printf(YELLOW "[INFO] No batches available in the inventory." RESET "\n");
        return;
    }

    int batchId;
    printf("Enter Batch ID to delete: ");
    batchId = get_int();

    for (int i = 0; i < inv->batchCount; i++) {
        if (inv->batches[i].batchId == batchId) {
            printf(YELLOW "[INFO] Batch Found: %d" RESET "\n", inv->batches[i].batchId);
            printf("Are you sure you want to delete this batch?\n");
            printf("1. Yes\n");
            printf("2. No\n");

            if (get_choice(2) != 1) {
                printf(YELLOW "[INFO] Delete cancelled." RESET "\n");
                return;
            }
            // Shift all subsequent batches left to overwrite the deleted batch
            for (int j = i; j < inv->batchCount - 1; j++) {
                inv->batches[j] = inv->batches[j + 1];
            }
            inv->batchCount--;
            printf(GREEN "[SUCCESS] Batch ID %d deleted successfully!" RESET "\n", batchId);
            return;
        }
    }

    printf(RED "[ERROR] Batch ID %d not found." RESET "\n", batchId);
}