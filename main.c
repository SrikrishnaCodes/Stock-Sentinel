#include <stdbool.h>
#include <stdio.h>

#include "inventory.h"
#include "menu.h"
#include "product.h"
#include "batch.h"

int main(void){
    Inventory inventory = {0};
    Inventory *inv = &inventory;

    loadInventory(inv);

    int menu_option;

    do{
        printf("\n");
        printf("==========================================\n");
        printf(BLUE "              STOCK SENTINEL" RESET "\n");
        printf(BLUE "           SUPERMARKET INVENTORY" RESET "\n");
        printf(BLUE "        & EXPIRY MONITORING SYSTEM" RESET "\n");
        printf("==========================================\n");
        printf("1. Product Management\n");
        printf("2. Batch & Expiry Tracking\n");
        printf("3. View Expiry Alerts\n");
        printf("4. Stock Management\n");
        printf("5. Low-Stock Alerts\n");
        printf("6. FEFO Recommendations\n");
        printf("7. Record Sales\n");
        printf("8. Stock Movement History\n");
        printf("9. Reports & Analytics\n");
        printf("10. Save and Exit\n");

        menu_option = get_choice(10);

        switch (menu_option){
            case 1:
                productMenu(inv);
                break;
            case 2:
                batchMenu(inv);
                break;
        }

    } while (menu_option != 10);

    saveInventory(inv);

    printf("\nThank You for using STOCK SENTINEL\n");

    return 0;
}

// Gets integer input between 1 and n
int get_choice(int n){
    int choice = 0;

    // User input Validation Loop
    while (true) {
        printf("------------------------------------------\n");
        printf("Enter your Choice (1-%d): ", n);

        // Check if input is a valid integer
        if (scanf("%d", &choice) != 1) {
            printf(RED "[ERROR] Invalid Choice!\nPlease enter a number between 1 and %d" RESET "\n", n);

            // Clear invalid input buffer
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);

            continue;
        }

        // Check for trailing characters (e.g. 5abc or 5 d)
        int next_char = getchar();
        if (next_char != '\n' && next_char != EOF) {
            while ((next_char = getchar()) != '\n' && next_char != EOF);

            printf(RED "[ERROR] Invalid Choice!\nPlease enter a number between 1 and %d" RESET "\n", n);
            continue;
        }

        // Validate integer range
        if (choice >= 1 && choice <= n) {
            printf(GREEN "[SUCCESS] You entered %d" RESET "\n", choice);
            printf("------------------------------------------\n");
            break;
        }
        else
            printf(RED "[ERROR] Invalid Choice!\nPlease enter a number between 1 and %d" RESET "\n", n);
    }

    return choice;
}

// only gets integer input without range validation
int get_int(void){
    int input = 0;

    // User input Validation Loop
    while (true) {
        if (scanf("%d", &input) != 1) {
            printf(RED "[ERROR] Invalid Input!\nPlease enter an Integer" RESET "\n");

            // Clear invalid input buffer
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);

            continue;
        }

        // Check for trailing characters (e.g. 5abc or 5 d)
        int next_char = getchar();
        if (next_char != '\n' && next_char != EOF) {
            while ((next_char = getchar()) != '\n' && next_char != EOF);

            printf(RED "[ERROR] Invalid Input!\nPlease enter an Integer" RESET "\n");
            continue;
        }

        // if program reaches here, input is valid
        break;
    }

    return input;
}

// only gets float input without range validation
float get_float(void){
    float input = 0.0f;

    while (true) {
        if (scanf("%f", &input) != 1) {
            printf(RED "[ERROR] Invalid Input!\nPlease enter a number" RESET "\n");

            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);

            continue;
        }

        int next_char = getchar();
        if (next_char != '\n' && next_char != EOF) {
            while ((next_char = getchar()) != '\n' && next_char != EOF);

            printf(RED "[ERROR] Invalid Input!\nPlease enter a number" RESET "\n");
            continue;
        }

        break;
    }

    return input;
}

void loadInventory(Inventory *inv){
    FILE *prodfile = fopen("products.dat", "rb");
    FILE *batchfile = fopen("batches.dat", "rb");

    if (prodfile == NULL){
        printf(RED "[ERROR] Unable to open products.dat." RESET "\n");
        if (batchfile != NULL) {
            fclose(batchfile);
        }
        return;
    }
    if (batchfile == NULL){
        printf(RED "[ERROR] Unable to open batches.dat." RESET "\n");
        fclose(prodfile);
        return;
    }

    fread(&inv->productCount, sizeof(inv->productCount), 1, prodfile);
    fread(&inv->batchCount, sizeof(inv->batchCount), 1, batchfile);

    for(int i = 0; i < inv->productCount; i++){
        fread(&inv->products[i].id, sizeof(int), 1, prodfile);
        fread(inv->products[i].name, sizeof(inv->products[i].name), 1, prodfile);
        fread(inv->products[i].category, sizeof(inv->products[i].category), 1, prodfile);
        fread(&inv->products[i].purchasePrice, sizeof(float), 1, prodfile);
        fread(&inv->products[i].sellingPrice, sizeof(float), 1, prodfile);
        fread(&inv->products[i].minStockLevel, sizeof(int), 1, prodfile);
    }

    for(int i = 0; i < inv->batchCount; i++){
        fread(&inv->batches[i].batchId, sizeof(int), 1, batchfile);
        fread(&inv->batches[i].productId, sizeof(int), 1, batchfile);
        fread(&inv->batches[i].quantity, sizeof(int), 1, batchfile);
        fread(&inv->batches[i].day, sizeof(int), 1, batchfile);
        fread(&inv->batches[i].month, sizeof(int), 1, batchfile);
        fread(&inv->batches[i].year, sizeof(int), 1, batchfile);
    }

    fclose(prodfile);
    fclose(batchfile);
}

void saveInventory(Inventory *inv){
    FILE *prodfile = fopen("products.dat", "wb");
    FILE *batchfile = fopen("batches.dat", "wb");

    if (prodfile == NULL){
        printf(RED "[ERROR] Unable to open products.dat." RESET "\n");
        return;
    }
    if (batchfile == NULL){
        printf(RED "[ERROR] Unable to open batches.dat." RESET "\n");
        fclose(prodfile);
        return;
    }

    fwrite(&inv->productCount, sizeof(inv->productCount), 1, prodfile);
    fwrite(&inv->batchCount, sizeof(inv->batchCount), 1, batchfile);

    for(int i = 0; i < inv->productCount; i++){
        fwrite(&inv->products[i].id, sizeof(int), 1, prodfile);
        fwrite(inv->products[i].name, sizeof(inv->products[i].name), 1, prodfile);
        fwrite(inv->products[i].category, sizeof(inv->products[i].category), 1, prodfile);
        fwrite(&inv->products[i].purchasePrice, sizeof(float), 1, prodfile);
        fwrite(&inv->products[i].sellingPrice, sizeof(float), 1, prodfile);
        fwrite(&inv->products[i].minStockLevel, sizeof(int), 1, prodfile);
    }

    for(int i = 0; i < inv->batchCount; i++){
        fwrite(&inv->batches[i].batchId, sizeof(int), 1, batchfile);
        fwrite(&inv->batches[i].productId, sizeof(int), 1, batchfile);
        fwrite(&inv->batches[i].quantity, sizeof(int), 1, batchfile);
        fwrite(&inv->batches[i].day, sizeof(int), 1, batchfile);
        fwrite(&inv->batches[i].month, sizeof(int), 1, batchfile);
        fwrite(&inv->batches[i].year, sizeof(int), 1, batchfile);
    }

    fclose(prodfile);
    fclose(batchfile);
}

