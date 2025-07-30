#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "transaction.h"
#include "customer.h"

#define TRANSACTION_FILE "transactions.txt"
#define PENDING_FILE "pending.txt"

void deposit_money(int *balance, int accountId) {
    int amount;
    time_t tm;
    time(&tm);

    printf("Enter the amount you want to deposit: ");
    scanf("%d", &amount);

    if (amount > 50000) {
        FILE *pending = fopen(PENDING_FILE, "a");
        fprintf(pending, "DEPOSIT %d %d %s", accountId, amount, ctime(&tm));
        fclose(pending);
        printf("Deposit over 50,000 is pending manager approval.\n");
        return;
    }

    *balance += amount;
    FILE *ptr = fopen(TRANSACTION_FILE, "a");
    fprintf(ptr, "Account %d: Rs%d deposited on %s", accountId, amount, ctime(&tm));
    fclose(ptr);
    printf("Money Deposited. New balance: %d\n", *balance);
}

void withdraw_money(int *balance, int accountId) {
    int amount;
    time_t tm;
    time(&tm);

    printf("Enter the amount you want to withdraw: ");
    scanf("%d", &amount);

    if (*balance < amount) {
        printf("Insufficient balance. Withdrawal request denied.\n");
        return;
    }

    if (amount > 50000) {
        FILE *pending = fopen(PENDING_FILE, "a");
        fprintf(pending, "WITHDRAW %d %d %s", accountId, amount, ctime(&tm));
        fclose(pending);
        printf("Withdrawal over 50,000 is pending manager approval.\n");
        return;
    }

    *balance -= amount;
    FILE *ptr = fopen(TRANSACTION_FILE, "a");
    fprintf(ptr, "Account %d: Rs%d withdrawn on %s", accountId, amount, ctime(&tm));
    fclose(ptr);
    printf("Money Withdrawn. Current balance: %d\n", *balance);
}

void viewTransactionHistory(int accountId) {
    FILE *fp = fopen(TRANSACTION_FILE, "r");
    if (!fp) {
        printf("No transaction history found.\n");
        return;
    }

    char line[200];
    int found = 0;
    printf("\n--- Transaction History for Account %d ---\n", accountId);
    while (fgets(line, sizeof(line), fp) != NULL) {
        int id;
        if (sscanf(line, "Account %d:", &id) == 1 && id == accountId) {
            printf("%s", line);
            found = 1;
        }
    }
    if (!found) {
        printf("No transactions found for Account %d.\n", accountId);
    }
    fclose(fp);
}

void deleteTransactionHistory(int accountId) {
    FILE *fp = fopen(TRANSACTION_FILE, "r");
    FILE *temp = fopen("temp_transactions.txt", "w");
    if (!fp || !temp) return;

    char line[200];
    while (fgets(line, sizeof(line), fp)) {
        int id;
        if (sscanf(line, "Account %d:", &id) != 1 || id != accountId) {
            fputs(line, temp);
        }
    }
    fclose(fp);
    fclose(temp);
    remove(TRANSACTION_FILE);
    rename("temp_transactions.txt", TRANSACTION_FILE);
}

void approvePendingTransactions() {
    FILE *pending = fopen(PENDING_FILE, "r");
    if (!pending) {
        printf("No pending transactions.\n");
        return;
    }

    char type[10], timeStr[100];
    int accId, amt, choice;

    while (fscanf(pending, "%s %d %d %[^\n]s", type, &accId, &amt, timeStr) == 4) {
        printf("Pending Transaction: %s Rs%d for Account %d on %s\n", type, amt, accId, timeStr);
        printf("Approve (1) or Reject (0)? ");
        scanf("%d", &choice);

        if (choice == 1) {
            int balance = getBalance(accId);
            if (strcmp(type, "DEPOSIT") == 0) {
                balance += amt;
            } else if (strcmp(type, "WITHDRAW") == 0 && balance >= amt) {
                balance -= amt;
            } else {
                printf("Insufficient balance for withdrawal. Rejected.\n");
                continue;
            }
            updateBalance(accId, balance);
            FILE *trans = fopen(TRANSACTION_FILE, "a");
            fprintf(trans, "Account %d: Rs%d %s approved by manager on %s\n", accId, amt, type, timeStr);
            fclose(trans);
            printf("Approved and updated.\n");
        } else {
            printf("Transaction rejected.\n");
        }
    }

    fclose(pending);
    remove(PENDING_FILE);
}

void viewUserTransactions() {
    int accountId;
    printf("Enter Account ID to view transactions: ");
    scanf("%d", &accountId);
    viewTransactionHistory(accountId);
}
