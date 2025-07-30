#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "account.h"
#include "customer.h"
#include "manager.h"

#define ACCOUNTS_FILE "accounts.dat"

struct Account {
    int id;
    char username[30];
    char password[30];
};

int getLastID() {
    FILE *fp = fopen(ACCOUNTS_FILE, "rb");
    struct Account acc;
    int lastID = 1000;

    if (fp != NULL) {
        while (fread(&acc, sizeof(struct Account), 1, fp) == 1) {
            lastID = acc.id;
        }
        fclose(fp);
    }
    return lastID;
}

void createAccount() {
    FILE *fp = fopen(ACCOUNTS_FILE, "ab");
    if (!fp) {
        printf("Error opening file.\n");
        return;
    }

    struct Account acc;
    printf("Enter username: ");
    scanf("%s", acc.username);
    printf("Enter password: ");
    scanf("%s", acc.password);

    acc.id = getLastID() + 1;
    fwrite(&acc, sizeof(struct Account), 1, fp);
    fclose(fp);

    updateBalance(acc.id, 0);
    printf("Account created successfully!\n");
    printf("Your Account ID is: %d\n", acc.id);
}

void deleteAccount() {
    int delID;
    printf("Enter Account ID to delete: ");
    scanf("%d", &delID);

    FILE *fp = fopen(ACCOUNTS_FILE, "rb");
    FILE *temp = fopen("temp_accounts.dat", "wb");
    struct Account acc;
    int found = 0;

    if (!fp || !temp) {
        printf("Error opening file.\n");
        return;
    }

    while (fread(&acc, sizeof(struct Account), 1, fp) == 1) {
        if (acc.id == delID) {
            found = 1;
        } else {
            fwrite(&acc, sizeof(struct Account), 1, temp);
        }
    }

    fclose(fp);
    fclose(temp);
    remove(ACCOUNTS_FILE);
    rename("temp_accounts.dat", ACCOUNTS_FILE);

    if (found) {
        printf("Account %d deleted successfully.\n", delID);
        deleteTransactionHistory(delID);
    } else {
        printf("Account ID %d not found.\n", delID);
    }
}

void login() {
    int inputID;
    char inputPass[30];
    struct Account acc;
    int found = 0;

    printf("Enter Account ID: ");
    scanf("%d", &inputID);
    printf("Enter Password: ");
    scanf("%s", inputPass);

    if (inputID == 1000 && strcmp(inputPass, "admin123") == 0) {
        printf("Manager login successful!\n");
        managerMenu();
        return;
    }

    FILE *fp = fopen(ACCOUNTS_FILE, "rb");
    if (!fp) {
        printf("No accounts found. Please create an account first.\n");
        return;
    }

    while (fread(&acc, sizeof(struct Account), 1, fp) == 1) {
        if (acc.id == inputID && strcmp(acc.password, inputPass) == 0) {
            found = 1;
            break;
        }
    }
    fclose(fp);

    if (found) {
        printf("Login successful! Welcome, %s.\n", acc.username);
        customerMenu(inputID);
    } else {
        printf("Invalid Account ID or Password.\n");
    }
}
