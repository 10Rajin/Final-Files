#ifndef CUSTOMER_H
#define CUSTOMER_H

void customerMenu(int accountId);
void updateBalance(int accountId, int balance);
int getBalance(int accountId);
void deleteTransactionHistory(int accountId);

#endif
