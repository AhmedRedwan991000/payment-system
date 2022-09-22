#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <time.h>
#include "terminal.h"



EN_terminalError_t getTransactionDate(ST_terminalData_t* termData)
{
	uint8_t date[11];
	uint8_t counter = 0;

	time_t t = time(NULL);
	struct tm* tm = localtime(&t);
	snprintf(date, 11, "%02d/%02d/%02d", tm->tm_mday, tm->tm_mon + 1, tm->tm_year + 1900);

	strcpy(termData->transactionDate, date);
}


EN_terminalError_t isCardExpired(ST_cardData_t* cardData, ST_terminalData_t* termData)
{
	uint8_t ExDate[6];
	uint8_t TrDate[11];

	strcpy(ExDate, cardData->cardExpirationDate);
	strcpy(TrDate, termData->transactionDate);

	int32_t ExMonth = ((ExDate[0] - '0') * 10) + (ExDate[1] - '0');
	int32_t ExYear = ((ExDate[3] - '0') * 10) + (ExDate[4] - '0');

	int32_t TrMonth = ((TrDate[3] - '0') * 10) + (TrDate[4] - '0');
	int32_t TrYear = ((TrDate[8] - '0') * 10) + (TrDate[9] - '0');

	if (ExYear < TrYear)
	{
		return EXPIRED_CARD;
	}
	else if (ExYear == TrYear)
	{
		if (ExMonth <= TrMonth)
		{
			return EXPIRED_CARD;
		}
	} 
	else
	{
		return TERMINAL_DONE;
	}
}



EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData)
{
	float TrAmount = 0;

	printf("Enter the transaction amount: ");
	scanf("%f", &TrAmount);

	if (TrAmount <= 0)
	{
		return INVALID_AMOUNT;
	}
	else
	{
		termData->transAmount = TrAmount;
		return TERMINAL_DONE;
	}
}


EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData)
{
	if (termData->transAmount > termData->maxTransAmount)
	{
		return EXCEED_MAX_AMOUNT;
	}
	else
	{
		return TERMINAL_DONE;
	}
}


EN_terminalError_t setMaxAmount(ST_terminalData_t* termData)
{
	float maxAmount;
	scanf("%f", &maxAmount);

	if (maxAmount <= 0)
	{
		return	INVALID_MAX_AMOUNT;
	}
	else
	{
		return TERMINAL_DONE;
	}
}
