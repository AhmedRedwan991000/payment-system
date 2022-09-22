#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include "server.h"


int32_t transactionNumber;
int32_t accountNumber;
ST_accountsDB_t tempAccount;
ST_transaction_t tempTransaction;
FILE* fptr;


EN_transState_t recieveTransactionData(ST_transaction_t* transData)
{
	// First Check If The Account Is Found Or Not
	if (isValidAccount(&(transData->cardHolderData)) == ACCOUNT_NOT_FOUND)
	{
		printf("Declined, The Account Is Not Found Or Blocked\n\n");
		transData->transState = DECLINED_STOLEN_CARD;

		// Saving Transaction
		saveTransaction(transData);

		return DECLINED_STOLEN_CARD;
	}

	// If The Account Is Found, Check If The Transaction Amount Is Available In The Account Or Not
	if (isAmountAvailable(&(transData->terminalData)) == LOW_BALANCE)
	{
		printf("Declined Insuffecient Fund, Your Balance is %0.2f\n\n", tempAccount.balance);
		transData->transState = DECLINED_INSUFFECIENT_FUND;

		// Saving Transaction
		saveTransaction(transData);

		return DECLINED_INSUFFECIENT_FUND;
	}

	transData->transState = APPROVED;
	
	// Update The Customer's Account If The Account Is Found And The Amount Is Available
	tempAccount.balance = tempAccount.balance - transData->terminalData.transAmount;
	printf("Transaction Approved Your Balance Now is %0.2f\n\n", tempAccount.balance);

	// Open The DataBase File To Overwrite The Old Balance With The New Balance
	fptr = fopen("./DB/accounts.bin", "rb+");
	fseek(fptr, 4 + (accountNumber * sizeof(tempAccount)), SEEK_SET);
	fwrite(&tempAccount, sizeof(tempAccount), 1, fptr);
	fclose(fptr);

	// Saving Transaction
	saveTransaction(transData);

	return APPROVED;

}


EN_serverError_t isValidAccount(ST_cardData_t* cardData)
{
	int32_t i, noOfAccounts;
	fptr = fopen("./DB/accounts.bin", "rb");
	fread(&noOfAccounts, sizeof(noOfAccounts), 1, fptr);

	for (i = 0; i < noOfAccounts;  i++)
	{
		fread(&tempAccount, sizeof(tempAccount), 1, fptr);
		if (strcmp(tempAccount.primaryAccountNumber, cardData->primaryAccountNumber) == 0)
		{
			accountNumber = i;
			fclose(fptr);
			return SERVER_DONE;
		}
		else if (i == noOfAccounts - 1)
		{
			fclose(fptr);
			return ACCOUNT_NOT_FOUND;
		}
	}
}


EN_serverError_t isAmountAvailable(ST_terminalData_t* termData)
{
	if (termData->transAmount > tempAccount.balance)
	{
		return LOW_BALANCE;
	}
	else
	{
		return SERVER_DONE;
	}
}


EN_serverError_t saveTransaction(ST_transaction_t* transData)
{
	int32_t noOfTransactions;
	tempTransaction = *transData;
	fptr = fopen("./DB/transactions.bin", "rb+");
	fread(&noOfTransactions, sizeof(noOfTransactions), 1, fptr);
	noOfTransactions++;
	fseek(fptr, 0, SEEK_END);
	if (fwrite(&tempTransaction, sizeof(tempTransaction), 1, fptr) != 1)
	{
		return SAVING_FAILED;
	}
	else
	{
		fseek(fptr, 0, SEEK_SET);
		fwrite(&noOfTransactions, sizeof(noOfTransactions), 1, fptr);
	}
	fclose(fptr);

	// Updating The transactions_db.txt
	fptr = fopen("./DB/transactions_db.txt", "r+");
	fseek(fptr, 0, SEEK_END);
	fprintf(fptr, "Transaction No. %d\n", noOfTransactions);
	fprintf(fptr, "Transaction Date: %s\n", transData->terminalData.transactionDate);
	fprintf(fptr, "Customer's Name: %s\n", transData->cardHolderData.cardHolderName);
	fprintf(fptr, "Pinary Account Number: %s\n", transData->cardHolderData.primaryAccountNumber);
	fprintf(fptr, "Transaction Amount: %0.2f\n", transData->terminalData.transAmount);
	if (transData->transState == APPROVED)
	{
		fprintf(fptr, "Transaction State: Approved\n");
	}
	else if (transData->transState == DECLINED_INSUFFECIENT_FUND)
	{
		fprintf(fptr, "Transaction State: Declined Insuffecient Fund\n");
	}
	else if (transData->transState == DECLINED_STOLEN_CARD)
	{
		fprintf(fptr, "Transaction State: Declined, The Account Is Not Found Or Blocked\n");
	}
	fprintf(fptr, "------------------------------------------------------------------\n");

	fclose(fptr);

}


EN_serverError_t getTransaction(int transactionSequenceNumber)
{
	transactionSequenceNumber--;
	fptr = fopen("./DB/transactions.bin", "rb+");
	fseek(fptr, 4 + (transactionSequenceNumber * sizeof(tempTransaction)), SEEK_SET);
	if (fread(&tempTransaction, sizeof(tempTransaction), 1, fptr) != 1)
	{
		return TRANSACTION_NOT_FOUND;
	}
	else
	{
		printf("-------------------------------------------------------------------\n");
		printf("Transaction Data: %s\n", tempTransaction.terminalData.transactionDate);
		printf("Customer's Name: %s\n", tempTransaction.cardHolderData.cardHolderName);
		printf("Pinary Account Number: %s\n", tempTransaction.cardHolderData.primaryAccountNumber);
		printf("Transaction Amount: %0.2f\n", tempTransaction.terminalData.transAmount);
		if (tempTransaction.transState == APPROVED)
		{
			printf("Transaction State: Approved\n");
		}
		else if (tempTransaction.transState == DECLINED_INSUFFECIENT_FUND)
		{
			printf("Transaction State: Declined Insuffecient Fund\n");
		}
		else if (tempTransaction.transState == DECLINED_STOLEN_CARD)
		{
			printf("Transaction State: Declined, The Account Is Not Found Or Blocked\n");
		}
		printf("\n");
	}

	fclose(fptr);
}



EN_serverError_t getAccount(char pan[20])
{
	int32_t i, noOfAccounts;
	fptr = fopen("./DB/accounts.bin", "rb");
	fread(&noOfAccounts, sizeof(noOfAccounts), 1, fptr);

	for (i = 0; i < noOfAccounts; i++)
	{
		fread(&tempAccount, sizeof(tempAccount), 1, fptr);
		if (strcmp(tempAccount.primaryAccountNumber, pan) == 0)
		{
			printf("Your Balance is: %0.2f\n\n", tempAccount.balance);
			fclose(fptr);
			return SERVER_DONE;
		}
		else if (i == noOfAccounts - 1)
		{
			fclose(fptr);
			printf("Account Not Found !\n\n");
			return ACCOUNT_NOT_FOUND;
		}
	}
}
