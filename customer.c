#include <stdio.h>
#include "customer.h"
#include "transaction.h"

#define BALANCE_FILE "balances.dat"

struct Balance {
    int id;
    int amount;
};

int getBalance(int accountId) {
    FILE *fp = fopen(BALANCE_FILE, "rb");
    struct Balance bal;
    if (!fp) return 0;

    while (fread(&bal, sizeof(struct Balance), 1, fp) == 1) {
        if (bal.id == accountId) {
            fclose(fp);
            return bal.amount;
        }
    }
    fclose(fp);
    return 0;
}

void updateBalance(int accountId, int balance) {
    FILE *fp = fopen(BALANCE_FILE, "rb");
    FILE *temp = fopen("temp_balance.dat", "wb");
    struct Balance bal;
    int found = 0;

    if (fp) {
        while (fread(&bal, sizeof(struct Balance), 1, fp) == 1) {
            if (bal.id == accountId) {
                bal.amount = balance;
                found = 1;
            }
            fwrite(&bal, sizeof(struct Balance), 1, temp);
        }
        fclose(fp);
    }

    if (!found) {
        bal.id = accountId;
        bal.amount = balance;
        fwrite(&bal, sizeof(struct Balance), 1, temp);
    }

    fclose(temp);
    remove(BALANCE_FILE);
    rename("temp_balance.dat", BALANCE_FILE);
}

void customerMenu(int accountId) {
    int balance = getBalance(accountId);
    int choice;

    while (1) {
        printf("\n===== Customer Menu =====\n");
        printf("Current Balance: %d\n", balance);
        printf("1. Deposit Money\n");
        printf("2. Withdraw Money\n");
        printf("3. View Transaction History\n");
        printf("4. Logout\n");
        printf("Choose an option: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                deposit_money(&balance, accountId);
                break;
            case 2:
                withdraw_money(&balance, accountId);
                break;
            case 3:
                viewTransactionHistory(accountId);
                break;
            case 4:
                updateBalance(accountId, balance);
                return;
            default:
                printf("Invalid choice. Try again.\n");
        }
    }
}
