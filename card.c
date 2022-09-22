#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include "card.h"

//#pragma warning(disable : 4996)


EN_cardError_t getCardHolderName(ST_cardData_t* cardData)
{
	uint8_t name[100];
	uint8_t counter = 0;

	printf("Enter your name (Maximum 24 Characters And Minimum 20 Characters): ");
	gets(name);


	for (int32_t i = 0; name[i] != '\0'; i++)
	{
		counter = counter + 1;
	}

	// checking the number of the characters
	if (counter > 24 || counter < 20)
	{
		return WRONG_NAME;
	}
	else
	{
		strcpy(cardData->cardHolderName, name);
		return CARD_DONE;
	}
	
}


EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData)
{
	uint8_t date[20];
	uint8_t counter = 0;

	printf("Enter the expiry date: ");
	gets(date);

	// counting the number of the characters
	for (int32_t i = 0; date[i] != '\0'; i++)
	{
		counter = counter + 1;
	}

	// checking the number of the characters
	if (counter < 5)
	{
		return WRONG_EXP_DATE;
	}

	int32_t months, year;

	months = ((date[0] - '0') * 10) + (date[1] - '0');
	year = ((date[3] - '0') * 10) + (date[4] - '0');


	// checking the format
	if (months > 12 || months < 1 || date[2] != '/' || year > 100 || year < 0)
	{
		return WRONG_EXP_DATE;
	}
	else
	{
		strcpy(cardData->cardExpirationDate, date);
		return CARD_DONE;
	}

}


EN_cardError_t getCardPAN(ST_cardData_t* cardData)
{
	uint8_t pan[50];
	uint8_t counter = 0;

	printf("Enter the primary account number: ");
	gets(pan);

	for (int32_t i = 0; pan[i] != '\0'; i++)
	{
		counter = counter + 1;
	}

	// checking the number of the characters
	if (counter > 19 || counter < 16)
	{
		return WRONG_PAN;
	}
	else
	{
		strcpy(cardData->primaryAccountNumber, pan);
		return CARD_DONE;
	}
}