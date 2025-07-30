#include <stdio.h>
#include "account.h"
#include "transaction.h"

void managerMenu() {
    int choice;
    while (1) {
        printf("\n===== Manager Menu =====\n");
        printf("1. Add User\n");
        printf("2. Delete User\n");
        printf("3. View Transactions of Specific User\n");
        printf("4. Approve Pending Transactions\n");
        printf("5. Logout\n");
        printf("Choose an option: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                createAccount();
                break;
            case 2:
                deleteAccount();
                break;
            case 3:
                viewUserTransactions();
                break;
            case 4:
                approvePendingTransactions();
                break;
            case 5:
                return;
            default:
                printf("Invalid choice.\n");
        }
    }
}
