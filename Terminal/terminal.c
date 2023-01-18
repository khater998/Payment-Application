#include "terminal.h"

char* termError[] = {
	 "TERMINAL_OK", "WRONG_DATE", "EXPIRED_CARD", "INVALID_CARD", "INVALID_AMOUNT", "EXCEED_MAX_AMOUNT", "INVALID_MAX_AMOUNT"
};

/****************************************************  getTransactionDate Function *************************************************************************************/
EN_terminalError_t getTransactionDate(ST_terminalData_t* termData)
{
	time_t currentTime;      // the variable to the calendar time
	currentTime = time(NULL);  // calculate the current time
	struct tm tm = *localtime(&currentTime); //The value of currentTime is broken up into the structure tm and expressed in the local time zone.
	
	char day[3] = { '\0' };
	char month[3] = { '\0' };
	char year[5] = { '\0' };

	itoa(tm.tm_mday, day, 10);        // convert number to string
	itoa(tm.tm_mon + 1, month, 10);
	itoa(tm.tm_year + 1900, year, 10);

	char date[11] = { day[0], day[1], '/', month[0], month[1], '/', year[0], year[1] ,year[2] ,year[3] };  // combine into the required string format 
	strcpy(termData->transactionDate, date);

	return TERMINAL_OK;
	
}
/**********************************************************************************************************************************************************************/

/****************************************************  getTransactionDateTest Function ********************************************************************************/
void getTransactionDateTest(void)
{
	ST_terminalData_t termData;
	ST_terminalData_t* termData_ptr = &termData;
	EN_terminalError_t x;

	printf("Tester Name: Mohamed Ibrahim\n");
	printf("Function Name: getTransactionDate\n\n");
	printf("Current Date: ");
	x = getTransactionDate(termData_ptr);
	printf("%s\n", termData_ptr->transactionDate);
	printf("Result: %s", termError[x]);

}
/**********************************************************************************************************************************************************************/

/****************************************************  isCardExpired Function *****************************************************************************************/
EN_terminalError_t isCardExpired(ST_cardData_t* cardData, ST_terminalData_t* termData)
{
	char card_month[2] = { cardData->cardExpirationDate[0], cardData->cardExpirationDate[0] };  // exlcuding month from cardData
	char card_year[2] = { cardData->cardExpirationDate[3], cardData->cardExpirationDate[4] };	// exlcuding year from cardData

	char term_month[2] = { termData->transactionDate[3], termData->transactionDate[4] };		// exlcuding month from termData
	char term_year[2] = { termData->transactionDate[8], termData->transactionDate[9] };			// exlcuding year from termData

	int cardMonth = atoi(card_month);    // convert numeric string to its digit value
	int cardYear = atoi(card_year);
	int termMonth = atoi(term_month);
	int termYear = atoi(term_year);

	if (cardYear > termYear)             // checking if valid
		return TERMINAL_OK;
	else if (cardYear == termYear)
	{
		if (cardMonth > termMonth)
			return TERMINAL_OK;
		else
			return EXPIRED_CARD;
	}
	else
		return EXPIRED_CARD;
}
/**********************************************************************************************************************************************************************/
/****************************************************  isCardExpiredTest Function *************************************************************************************/
void isCardExpriedTest(void)
{
	EN_terminalError_t x = 0;
	ST_cardData_t cardData;
	ST_cardData_t* cardData_ptr = &cardData;

	ST_terminalData_t termData;
	ST_terminalData_t* termData_ptr = &termData;

	printf("Tester Name: Mohamed Ibrahim\n");
	printf("Function Name: isCardExpired\n\n");

	// test 1 with valid card
	printf("Test Case 1: \n");
	printf("Input Data: valid date: 05/25 \n");
	getCardExpiryDate(cardData_ptr);
	getTransactionDate(termData_ptr);
	printf("Transaction date: %s\n", termData_ptr->transactionDate);
	x = isCardExpired(cardData_ptr, termData_ptr);
	printf("Expected Result: %s\n", termError[TERMINAL_OK]);
	printf("Actual Result: %s\n\n", termError[x]);

	printf("Test Case 2: \n");
	printf("Input Data: 11/19\n");
	// test 2 with expired card
	getCardExpiryDate(cardData_ptr);
	getTransactionDate(termData_ptr);
	printf("Transaction date: %s\n", termData_ptr->transactionDate);
	x = isCardExpired(cardData_ptr, termData_ptr);
	printf("Expected Result: %s\n", termError[EXPIRED_CARD]);
	printf("Actual Result: %s\n\n", termError[x]);
}
/**********************************************************************************************************************************************************************/

/****************************************************  setMaxAmount Function ******************************************************************************************/
EN_terminalError_t setMaxAmount(ST_terminalData_t* termData, float maxAmount)
{
	if (maxAmount <= 0)
		return INVALID_MAX_AMOUNT;
	termData->maxTransAmount = maxAmount;
	return TERMINAL_OK;
}
/**********************************************************************************************************************************************************************/

/****************************************************  setMaxAmountTest Function **************************************************************************************/
void setMaxAmountTest(void)
{
	EN_terminalError_t x;
	ST_terminalData_t termData;
	ST_terminalData_t* termData_ptr = &termData;

	printf("Tester Name: Mohamed Ibrahim\n");
	printf("Function Name: setMaxAmount\n\n");

	// test 1 with a valid maxAmount
	printf("Test Case 1: \n");
	printf("Input Data: 10000\n");
	x = setMaxAmount(termData_ptr, 10000.0);
	printf("Expected Result: %s\n", termError[TERMINAL_OK]);
	printf("Actual Result: %s\n\n", termError[x]);

	// test 1 with a 0
	printf("Test Case 2: \n");
	printf("Input Data: 0\n");
	x = setMaxAmount(termData_ptr, 0);
	printf("Expected Result: %s\n", termError[INVALID_MAX_AMOUNT]);
	printf("Actual Result: %s\n\n", termError[x]);

	// test 1 with a valid maxAmount
	printf("Test Case 3: \n");
	printf("Input Data: -5000.0\n");
	x = setMaxAmount(termData_ptr, -5000.0);
	printf("Expected Result: %s\n", termError[INVALID_MAX_AMOUNT]);
	printf("Actual Result: %s\n\n", termError[x]);
}
/**********************************************************************************************************************************************************************/

/****************************************************  getTransactionAmount Function **********************************************************************************/
EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData)
{
	float amount = 0;
	printf("Enter transaction amount: ");
	scanf("%f", &amount);
	flush_input();                   // created function in required.c to flush the input buffer
	if (amount <= 0)
		return INVALID_AMOUNT;
	termData->transAmount = amount;
	return TERMINAL_OK;
}
/**********************************************************************************************************************************************************************/

/****************************************************  getTransactionAmountTest Function ******************************************************************************/
void getTransactionAmountTest(void)
{
	EN_terminalError_t x;
	ST_terminalData_t termData;
	ST_terminalData_t* termData_ptr = &termData;

	printf("Tester Name: Mohamed Ibrahim\n");
	printf("Function Name: getTransactionAmount\n\n");

	// test 1 with valid amount
	printf("Test Case 1: \n");
	printf("Input Data: any valid amount\n");
	x = getTransactionAmount(termData_ptr);
	printf("Expected Result: %s\n", termError[TERMINAL_OK]);
	printf("Actual Result: %s\n\n", termError[x]);

	// test 2 with 0
	printf("Test Case 2: \n");
	printf("Input Data: 0\n");
	x = getTransactionAmount(termData_ptr);
	printf("Expected Result: %s\n", termError[INVALID_AMOUNT]);
	printf("Actual Result: %s\n\n", termError[x]);

	// test 2 with negative numbers
	printf("Test Case 2: \n");
	printf("Input Data: negative numbers\n");
	x = getTransactionAmount(termData_ptr);
	printf("Expected Result: %s\n", termError[INVALID_AMOUNT]);
	printf("Actual Result: %s\n", termError[x]);
}
/**********************************************************************************************************************************************************************/


/****************************************************  isBelowMaxAmount Function **************************************************************************************/
EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData)
{
	setMaxAmount(termData, MAXAMOUNT); // MAXAMOUNT is a Macro to set the terminal max amount

	if (termData->transAmount > termData->maxTransAmount)
		return EXCEED_MAX_AMOUNT;
	return TERMINAL_OK;
}
/**********************************************************************************************************************************************************************/

/****************************************************  isBelowMaxAmount Function **************************************************************************************/
void isBelowMaxAmountTest(void)
{
	EN_terminalError_t x;
	ST_terminalData_t termData;
	ST_terminalData_t* termData_ptr = &termData;

	printf("Tester Name: Mohamed Ibrahim\n");
	printf("Function Name: isBelowMaxAmountTest\n\n");

	// test 1 transaction amount <= maxAmount
	printf("Test Case 1: \n");
	printf("Input Data: 0<input<10000   // maxAmount: 10000.0\n");
	getTransactionAmount(termData_ptr);
	x = isBelowMaxAmount(termData_ptr);
	printf("Expected Result: %s\n", termError[TERMINAL_OK]);
	printf("Actual Result: %s\n\n", termError[x]);

	// test 2 transaction amount > maxAmount
	printf("Test Case 2: \n");
	printf("Input Data: input>10000     //  maxAmount: 10000.0\n");
	getTransactionAmount(termData_ptr);
	x = isBelowMaxAmount(termData_ptr);
	printf("Expected Result: %s\n", termError[EXCEED_MAX_AMOUNT]);
	printf("Actual Result: %s\n\n", termError[x]);
}
/**********************************************************************************************************************************************************************/

/****************************************************  isValidCardPAN Function ****************************************************************************************/
EN_terminalError_t isValidCardPAN(ST_cardData_t* cardData)
{
	char PAN[20] = {'\0'};
	strcpy(PAN, cardData->primaryAccountNumber);
	uint8_t length = strlen(PAN);
	uint16_t sum = 0;     

	for (uint8_t i = 0; i < length; i++)
	{
		uint8_t num = PAN[i] - '0'; // converting a char digit to its integer value

		if (i % 2 == 0)
		{
			uint8_t product = 2 * num;                 // even index elements are multiplied by 2
			if (product < 9)
				sum += 2 * num;
			else
			{
				sum += ((product / 10) + (product % 10));
			}
		}
		else
		{
			sum += num;      // odd index elements are multiplied by 1
		}
	}

	if (sum % 10 == 0)
		return TERMINAL_OK;
	else
		return INVALID_CARD;
}
/**********************************************************************************************************************************************************************/

/****************************************************  isValidCardPANTest Function ************************************************************************************/
void isValidCardPANTest(void)
{
	ST_cardData_t cardData;
	ST_cardData_t* cardData_ptr = &cardData;
	EN_terminalError_t x;

	printf("Tester Name: Mohamed Ibrahim\n");
	printf("Function Name: isValidCardPAN\n\n");

	printf("Test Case 1: \n");
	printf("Input Data: 4681560145317153   // LUHN number\n");
	getCardPAN(cardData_ptr);
	x = isValidCardPAN(cardData_ptr);
	printf("Expected Result: %s\n", termError[TERMINAL_OK]);
	printf("Actual Result: %s\n\n", termError[x]);

	printf("Test Case 1: \n");
	printf("Input Data: 1234567890123456   // not a LUHN number\n");
	getCardPAN(cardData_ptr);
	x = isValidCardPAN(cardData_ptr);
	printf("Expected Result: %s\n", termError[INVALID_CARD]);
	printf("Actual Result: %s\n\n", termError[x]);
}
/**********************************************************************************************************************************************************************/
