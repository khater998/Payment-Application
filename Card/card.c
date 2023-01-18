#include "card.h"

char* cardError[] = {											// combine with the enum cardError to print the error as a string
	"CARD_OK", "WRONG_NAME", "WRONG_EXP_DATE", "WRONG_PAN" 
};  

/****************************************************  getCardHolderName Function *************************************************************************************/
EN_cardError_t getCardHolderName(ST_cardData_t* cardData)
{
	char name[30];											 // to store the name entered by the user
								                 // to store the length of the names
	printf("Enter Card Holder Name: ");						 // ask the user to input the name
	inputf(name,30);											 // created function in required.h
	
	int length;
	length = strlen(name);

	if (length < 20 || length > 24 || !isAlphabetic(name))    // check if the name has the allowed format, isAlphabetic is a created funtction in required.h
		return WRONG_NAME;
	else {
		strcpy(cardData->cardHolderName, name);
		return CARD_OK;
	}
}
/**********************************************************************************************************************************************************************/

/************************************* getCardHolderNameTest Function *************************************************************************************************/
void getCardHolderNameTest(void)
{
	ST_cardData_t cardData;
	ST_cardData_t* cardData_ptr = &cardData;
	EN_cardError_t x;   // store the error type in x

	printf("Tester Name: Mohamed Ibrahim\n");
	printf("Function Name: getCardHolderName\n\n");
	// test 1 with a valid name
	printf("Test Case 1: \n");
	printf("Input Data: Mohamed Ibrahim Ezzat (acceptable)\n");
	x = getCardHolderName(cardData_ptr);
	printf("Expected Result: %s\n", cardError[CARD_OK]);
	printf("Actual Result: %s\n\n", cardError[x]);

	// test 2 with a shortname
	printf("Test Case 2: \n");
	printf("Input Data: Ali Ashrf (too short)\n");
	x = getCardHolderName(cardData_ptr);
	printf("Expected Result: %s\n", cardError[WRONG_NAME]);
	printf("Actual Result: %s\n\n", cardError[x]);

	// test 3 with a long name
	printf("Test Case 3: \n");
	printf("Input Data: Reda Elsayed Azzab Abdallah (too long)\n");
	x = getCardHolderName(cardData_ptr);
	printf("Expected Result: %s\n", cardError[WRONG_NAME]);
	printf("Actual Result: %s\n\n", cardError[x]);
	// test 4 name has special character or numbers
	printf("Test Case 4: \n");
	printf("Input Data: Mohamed Ibrahim 3zzat (has numbers)\n");
	x = getCardHolderName(cardData_ptr);
	printf("Expected Result: %s\n", cardError[WRONG_NAME]);
	printf("Actual Result: %s\n\n", cardError[x]);
	return;
}
/************************************************************************************************************************************************************************/

/****************************************************  getCardExpiryDate Function ***************************************************************************************/
EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData)
{
	char expDate[8];      // more than 5, to detect if the user entered more than 5 characters
	int length;

	printf("Enter Expiry Date as \"MM/YY\": ");
	inputf(expDate,8);
	length = strlen(expDate);
											// check if date is the allowed format
	if (length != 5 || expDate[2] != '/' || !isdigit(expDate[0]) || !isdigit(expDate[1]) || !isdigit(expDate[3]) || !isdigit(expDate[4]))
		return WRONG_EXP_DATE;
	else
	{
		char monthStr[2] = { expDate[0], expDate[1] };   // exclude the month part from the date string
		int month = atoi(monthStr);

		if (month > 12 || month < 1)            // montht can't be > 12 or < 1
			return WRONG_EXP_DATE;
		strcpy(cardData->cardExpirationDate, expDate);
		return CARD_OK;
	}
}
/************************************************************************************************************************************************************************/

/****************************************************  getCardExpiryDateTest Function ***********************************************************************************/
void getCardExpiryDateTest(void)
{
	ST_cardData_t cardData;
	ST_cardData_t* cardData_ptr = &cardData;
	EN_cardError_t x;

	printf("Tester Name: Mohamed Ibrahim\n");
	printf("Function Name: getCardExpiryDate\n\n");
	// test 1 with a valid date
	printf("Test Case 1: \n");
	printf("Input Data: any valid date\n");
	x = getCardExpiryDate(cardData_ptr);
	printf("Expected Result: %s\n", cardError[CARD_OK]);
	printf("Actual Result: %s\n\n", cardError[x]);
	// test 2 with more than 5 characters
	printf("Test Case 2: \n");
	printf("Input Data: 10/2022 (more than 5 characters)\n");
	x = getCardExpiryDate(cardData_ptr);
	printf("Expected Result: %s\n", cardError[WRONG_EXP_DATE]);
	printf("Actual Result: %s\n\n", cardError[x]);
	// test 3 with less than 5 characters
	printf("Test Case 3: \n");
	printf("Input Data: 5/22 (less than 5 characters)\n");
	x = getCardExpiryDate(cardData_ptr);
	printf("Expected Result: %s\n", cardError[WRONG_EXP_DATE]);
	printf("Actual Result: %s\n\n", cardError[x]);
	// test 4 changing the delimiter
	printf("Test Case 4: \n");
	printf("Input Data: 09-24 (changing the delimiter)\n");
	x = getCardExpiryDate(cardData_ptr);
	printf("Expected Result: %s\n", cardError[WRONG_EXP_DATE]);
	printf("Actual Result: %s\n\n", cardError[x]);
	// test 5 using non-numeric
	printf("Test Case 5: \n");
	printf("Input Data: 0@/24 (using non-numeric\n");
	x = getCardExpiryDate(cardData_ptr);
	printf("Expected Result: %s\n", cardError[WRONG_EXP_DATE]);
	printf("Actual Result: %s\n\n", cardError[x]);
	// test 6 with a non-valid month 
	printf("Test Case 6: \n");
	printf("Input Data: 13/27 (using non-valid month\n");
	x = getCardExpiryDate(cardData_ptr);
	printf("Expected Result: %s\n", cardError[WRONG_EXP_DATE]);
	printf("Actual Result: %s\n\n", cardError[x]);
}
/************************************************************************************************************************************************************************/

/****************************************************  getCardPAN Function **********************************************************************************************/
EN_cardError_t getCardPAN(ST_cardData_t* cardData)
{
	char panNum[22];
	int length;

	printf("Enter PAN Number: ");
	inputf(panNum,22);
	length = strlen(panNum);

	if (length > 19 || length < 16 || !alldigit(panNum)) // check PAN is the required format
		return WRONG_PAN;
	strcpy(cardData->primaryAccountNumber, panNum);
	return CARD_OK;
}
/************************************************************************************************************************************************************************/

/****************************************************  getCardPANTest Function ******************************************************************************************/
void getCardPANTest(void)
{
	ST_cardData_t cardData;
	ST_cardData_t* cardData_ptr = &cardData;
	EN_cardError_t x;

	printf("Tester Name: Mohamed Ibrahim\n");
	printf("Function Name: getCardPAN\n\n");

	// test 1 with a valid PAN number
	printf("Test Case 1: \n");
	printf("Input Data: 5550790294712160  // VALID PAN\n");
	x = getCardPAN(cardData_ptr);
	printf("Expected Result: %s\n", cardError[CARD_OK]);
	printf("Actual Result: %s\n\n", cardError[x]);

	// test 2 less than 16 characters
	printf("Test Case 2: \n");
	printf("Input Data: 123456789        // too short\n");
	x = getCardPAN(cardData_ptr);
	printf("Expected Result: %s\n", cardError[WRONG_PAN]);
	printf("Actual Result: %s\n\n", cardError[x]);

	// test 3 more than 19 characters
	printf("Test Case 3: \n");
	printf("Input Data: 5550790294712160123453 // too long \n");
	x = getCardPAN(cardData_ptr);
	printf("Expected Result: %s\n", cardError[WRONG_PAN]);
	printf("Actual Result: %s\n\n", cardError[x]);

	// test 4 not all numeric
	printf("Test Case 4: \n");
	printf("Input Data: 5550790@94@12160  // not all digits\n");
	x = getCardPAN(cardData_ptr);
	printf("Expected Result: %s\n", cardError[WRONG_PAN]);
	printf("Actual Result: %s\n\n", cardError[x]);
}
/************************************************************************************************************************************************************************/


