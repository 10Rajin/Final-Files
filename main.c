#include "ui.h"
#include "account.h"
#include "customer.h"
#include "manager.h"

int main() {
    int choice;
    while (1) {
        welcome();

        printf("\n1. Login (Manager/Customer)\n");
        printf("2. Create Account\n");
        printf("3. Exit\n");
        printf("Choose an option: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                login();
                break;
            case 2:
                createAccount();
                break;
            case 3:
                printf("Exiting program. Goodbye!\n");
                exit(0);
            default:
                printf("Invalid choice. Try again.\n");
        }
    }
    return 0;
}
