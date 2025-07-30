#ifndef TRANSACTION_H
#define TRANSACTION_H

void deposit_money(int *balance, int accountId);
void withdraw_money(int *balance, int accountId);
void viewTransactionHistory(int accountId);
void deleteTransactionHistory(int accountId);
void approvePendingTransactions();
void viewUserTransactions();

#endif
