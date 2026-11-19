#include <stdbool.h>
#include <stdio.h>

#include "inventory.h"
#include "menu.h"
#include "product.h"
#include "batch.h"

int main(void){
    Inventory inventory = {0};
    Inventory *inv = &inventory;

    int menu_option;

    do{
        printf("\n");
        printf("==========================================\n");
        printf("              STOCK SENTINEL\n");
        printf("           SUPERMARKET INVENTORY\n");
        printf("        & EXPIRY MONITORING SYSTEM\n");
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

    printf("Thank You for using STOCK SENTINEL\n");

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
            printf("%s[ERROR] Invalid Choice!\nPlease enter a number between 1 and %d%s\n", RED, n, RESET);

            // Clear invalid input buffer
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);

            continue;
        }

        // Check for trailing characters (e.g. 5abc or 5 d)
        int next_char = getchar();
        if (next_char != '\n' && next_char != EOF) {
            while ((next_char = getchar()) != '\n' && next_char != EOF);

            printf("%s[ERROR] Invalid Choice!\nPlease enter a number between 1 and %d%s\n", RED, n, RESET);
            continue;
        }

        // Validate integer range
        if (choice >= 1 && choice <= n) {
            printf("%s[SUCCESS] You entered %d%s\n", GREEN, choice, RESET);
            printf("------------------------------------------\n");
            break;
        }
        else
            printf("%s[ERROR] Invalid Choice!\nPlease enter a number between 1 and %d%s\n", RED, n, RESET);
    }

    return choice;
}

// only gets integer input without range validation
int get_int(void){
    int input = 0;

    // User input Validation Loop
    while (true) {
        if (scanf("%d", &input) != 1) {
            printf("%s[ERROR] Invalid Input!\nPlease enter an Integer%s\n", RED, RESET);

            // Clear invalid input buffer
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);

            continue;
        }

        // Check for trailing characters (e.g. 5abc or 5 d)
        int next_char = getchar();
        if (next_char != '\n' && next_char != EOF) {
            while ((next_char = getchar()) != '\n' && next_char != EOF);

            printf("%s[ERROR] Invalid Input!\nPlease enter an Integer%s\n", RED, RESET);
            continue;
        }

        // if program reaches here, input is valid
        break;
    }

    return input;
}
