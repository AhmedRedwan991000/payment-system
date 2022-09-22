#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include "app.h"

void app(void)
{
	int32_t option;
	printf("To Make A Transaction Press 1\n");
	printf("To Get A Transaction Press 2\n");
	printf("To Check Your Balance Press 3\n");
	scanf("%d", &option);
	char c;
	scanf("%c", &c);

	if (option == 1)
	{
		ST_cardData_t* cardData, temp;
		ST_terminalData_t* termData, temp2;
		ST_transaction_t* transData, temp3;
		cardData = &temp;
		termData = &temp2;
		transData = &temp3;


		// Getting Customer's Name
		uint8_t name = getCardHolderName(cardData);
		while (name == WRONG_NAME)
		{
			printf("Wrong Name Try Again\n\n");
			name = getCardHolderName(cardData);
		}


		// Getting the Customer's Card PAN
		uint8_t pan = getCardPAN(cardData);
		while (pan == WRONG_PAN)
		{
			printf("Wrong PAN Number Try Again\n\n");
			pan = getCardPAN(cardData);
		}


		// Getting The Customer's Card Expiry Date
		uint8_t exDate = getCardExpiryDate(cardData);
		while (exDate == WRONG_EXP_DATE)
		{
			printf("Wrong Expiry Date Try again\n\n");
			exDate = getCardExpiryDate(cardData);
		}


		// Getting The Transaction Date
		getTransactionDate(termData);


		uint8_t isExpired = isCardExpired(cardData, termData);
		if (isExpired == EXPIRED_CARD)
		{
			printf("Expired Card\n\n");
			goto end;
		}

		// Setting Maximum Transaction Amount
		termData->maxTransAmount = 2500;


		uint8_t trAmount = getTransactionAmount(termData);
		while (trAmount == INVALID_AMOUNT)
		{
			printf("Invalid Amount Try Again\n\n");
			trAmount = getTransactionAmount(termData);
		}


		uint8_t isBelowMax = isBelowMaxAmount(termData);
		while (isBelowMax == EXCEED_MAX_AMOUNT)
		{
			printf("Exceed Max Transaction Amount (2500) Try Again\n\n");
			trAmount = getTransactionAmount(termData);
			isBelowMax = isBelowMaxAmount(termData);
		}


		// Saving Terminal Data And Card Data Into Transaction DataBase
		transData->cardHolderData = *cardData;
		transData->terminalData = *termData;

		recieveTransactionData(transData);

	end:
		;

	}
	else if (option == 2)
	{
		int32_t transNumber = 0;
		printf("Enter The Transaction Number: ");
		scanf("%d", &transNumber);
		if (getTransaction(transNumber) == TRANSACTION_NOT_FOUND)
		{
			printf("Transaction Not Found !\n\n");
		}
	}
	else
	{
		uint8_t pan[50];

		printf("Enter Your Primary Account Number: ");
		scanf("%s", pan);

		getAccount(pan);
	}
}