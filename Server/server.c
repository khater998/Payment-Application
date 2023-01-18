#include "server.h"

char* transStateError[] = {
	"APPROVED", "DECLINED_INSUFFECIENT_FUND", "DECLINED_STOLEN_CARD", "FRAUD_CARD", "INTERNAL_SERVER_ERROR"   // to print errors as a string
}; 

char* serError[] = {
	"SERVER_OK", "SAVING_FAILED", "TRANSACTION_NOT_FOUND", "ACCOUNT_NOT_FOUND", "LOW_BALANCE", "BLOCKED_ACCOUNT"  // to print errors as a string
};

ST_accountsDB_t accountsDB[255] = {													
									{2000.0,    RUNNING, "8989374615436851"},
									{100000.0,  BLOCKED, "5807007076043875"},
									{500000.0,  RUNNING, "5550790294712160"},
									{2000000.0, BLOCKED, "4313305323553963"},
									{175000,    RUNNING, "4650731245148528"},
									{30000,     RUNNING, "4508363596763285"},
									{100000.0,  BLOCKED, "6709893990261223"},
									{5000.0,    RUNNING, "4917617852195994"},
									{800000.0,  RUNNING, "4175002717566332"},
									{15000.0,   BLOCKED, "4175006510464850"} };

ST_transaction_t transactionsDB[255] = { 0 };

uint16_t index = 0;                  // to reference the matched account in the database
uint16_t transTracker = 0;           // to track the transactions operations


/**************************************************************    isValidAccount functin    **************************************************************************/

EN_serverError_t isValidAccount(ST_cardData_t* cardData, ST_accountsDB_t* accountRefrence)
{
	bool found = false;                          
	for (index = 0; index < 255; index++)       // search through all accountsDB 255 elements
	{
		if (strcmp(cardData->primaryAccountNumber, accountRefrence[index].primaryAccountNumber) == 0)   // check if entered PAN matches the PAN element of any account
		{
			found = true;                        
			break;                              // break out of the loop if account is found
		}
	}
	if (found == false)
		return ACCOUNT_NOT_FOUND;        
	return SERVER_OK;
}
/**********************************************************************************************************************************************************************/
/**************************************************************    isValidAccountTest functin    **********************************************************************/
void isValidAccountTest(void)
{
	ST_cardData_t cardData;
	ST_cardData_t* cardData_ptr = &cardData;
	EN_serverError_t x = 0;

	printf("Tester Name: Mohamed Ibrahim Ezzat\n");
	printf("Function Name: isValidAccountTest\n\n");

	// test 1: valid PAN number
	printf("Test Case 1: \n");
	printf("Input Data: 4650731245148528    // PAN in the data base\n");
	getCardPAN(cardData_ptr);
	x = isValidAccount(cardData_ptr, accountsDB);
	printf("Expected Result: %s\n", serError[SERVER_OK]);
	printf("Actual Result: %s\n\n", serError[x]);

	printf("Test Case 2: \n");
	printf("Input Data: 1234567890123456   // PAN that doesn't exist\n");
	getCardPAN(cardData_ptr);
	x = isValidAccount(cardData_ptr, accountsDB);
	printf("Expected Result: %s\n", serError[ACCOUNT_NOT_FOUND]);
	printf("Actual Result: %s\n\n", serError[x]);
}
/**********************************************************************************************************************************************************************/

/**************************************************************    isBlockedAccount functin    ************************************************************************/
EN_serverError_t isBlockedAccount(ST_accountsDB_t* accountRefrence)
{
	if (accountRefrence->state == RUNNING)
		return SERVER_OK;
	return BLOCKED_ACCOUNT;
}
/**********************************************************************************************************************************************************************/

/**************************************************************    isBlockedAccountTest functin    ********************************************************************/
void isBlockedAccountTest(void)
{
	ST_cardData_t cardData;
	ST_cardData_t* cardData_ptr = &cardData;
	EN_serverError_t x = 0;

	printf("Tester Name: Mohamed Ibrahim Ezzat\n");
	printf("Function Name: isBlockedAccountTest\n\n");

	printf("Test Case 1: \n");
	printf("Input Data: 4175002717566332     // Running account \n");
	getCardPAN(cardData_ptr);
	isValidAccount(cardData_ptr, accountsDB);
	x = isBlockedAccount(&accountsDB[index]);
	printf("Expected Result: %s\n", serError[SERVER_OK]);
	printf("Actual Result: %s\n\n", serError[x]);

	printf("Test Case 2: \n");
	printf("Input Data: 4175006510464850	 // Blocked account\n");
	getCardPAN(cardData_ptr);
	isValidAccount(cardData_ptr, accountsDB);
	x = isBlockedAccount(&accountsDB[index]);
	printf("Expected Result: %s\n", serError[BLOCKED_ACCOUNT]);
	printf("Actual Result: %s\n\n", serError[x]);
}
/**********************************************************************************************************************************************************************/


/**************************************************************    isAmountAvailable functin    ***********************************************************************/
EN_serverError_t isAmountAvailable(ST_terminalData_t* termData, ST_accountsDB_t* accountRefrence)
{
	if (termData->transAmount > accountRefrence->balance)
		return LOW_BALANCE;
	return SERVER_OK;
}
/**********************************************************************************************************************************************************************/

/**************************************************************    isAmountAvailableTest functin    ********************************************************************/
void isAmountAvailableTest(void)
{
	ST_cardData_t cardData;
	ST_cardData_t* cardData_ptr = &cardData;

	ST_terminalData_t termData;
	ST_terminalData_t* termData_ptr = &termData;
	EN_serverError_t x;

	printf("Tester Name: Mohamed Ibrahim Ezzat\n");
	printf("Function Name: isAmountAvailableTest\n\n");

	printf("Test Case 1: \n");
	printf("Input Data: 4508363596763285 && amount < 30000     // balance = 30000\n");
	getCardPAN(cardData_ptr);
	getTransactionAmount(termData_ptr);
	isValidAccount(cardData_ptr, accountsDB);
	x = isAmountAvailable(termData_ptr, &(accountsDB[index]));
	printf("Expected Result: %s\n", serError[SERVER_OK]);
	printf("Actual Result: %s\n\n", serError[x]);

	printf("Test Case 2: \n");
	printf("Input Data: 4508363596763285 && amount > 30000     // balance = 30000 \n");
	getCardPAN(cardData_ptr);
	getTransactionAmount(termData_ptr);
	isValidAccount(cardData_ptr, accountsDB);
	x = isAmountAvailable(termData_ptr, &(accountsDB[index]));
	printf("Expected Result: %s\n", serError[LOW_BALANCE]);
	printf("Actual Result: %s\n\n", serError[x]);

}
/**********************************************************************************************************************************************************************/
EN_transState_t recieveTransactionData(ST_transaction_t* transData)
{
	EN_transState_t errorState;

	/*###############################     getting card data   #####################################*/

	ST_cardData_t* cardHolderData_ptr = &(transData->cardHolderData);																	
	EN_cardError_t cardState;
	do
	{
		cardState = getCardHolderName(cardHolderData_ptr);          // From the flowchart, Card Data must be correct, there is no abort or ending option
		if (cardState != CARD_OK)									// Making sure that name follows the allowed format, if not, user enters the name again
			printf("%70s", "!!!! WRONG_NAME_FORMAT !!!!\n");
	} while (cardState != CARD_OK);

	do
	{
		cardState = getCardExpiryDate(cardHolderData_ptr);
		if (cardState != CARD_OK)									// Making sure that Card Expiry Date follows the allowed format, if not, user enters the name again
			printf("%70s", "!!!! WRONG_EXP_DATE_FORMAT !!!!\n");
	} while (cardState != CARD_OK);
	
	do
	{
		cardState = getCardPAN(cardHolderData_ptr);                 // // Making sure that PAN follows the allowed format, if not, user enters the name again
		if (cardState != CARD_OK)
			printf("%70s", "!!!! WRONG_PAN_FORAMAT !!!!\n");
	} while (cardState != CARD_OK);

	EN_terminalError_t terminalState = isValidCardPAN(cardHolderData_ptr);
	if (terminalState == INVALID_CARD)
		return printf("%70s", "#### DECLINED, failed in Luhun check ####\n");

	/*############################################################################################*/


	/*###############################     getting terminal data   ################################*/

	ST_terminalData_t* terminalData_ptr = &(transData->terminalData);

	getTransactionDate(terminalData_ptr);                           // take date from the machine

	terminalState = isCardExpired(cardHolderData_ptr, terminalData_ptr);   
	if (terminalState == EXPIRED_CARD)								
		return printf("%70s", "#### DECLINED EXPIRED CARD ####\n");  // checking if card is expired, if so, Abort the operation

	setMaxAmount(terminalData_ptr, MAXAMOUNT);                       // setting the terminal max amount, "currently 10000"

	do																
	{
		terminalState = getTransactionAmount(terminalData_ptr);      // keep asking the user for a vaild withdrawal amount
		if (terminalState != TERMINAL_OK)
			printf("%70s", "!!!! INVALID_AMOUNT !!!!\n");
	} while (terminalState != TERMINAL_OK);

	terminalState = isBelowMaxAmount(terminalData_ptr);				 // If user's entered mount > Terminal  max amount,   Abort the operation
	if (terminalState == EXCEED_MAX_AMOUNT)
		return printf("%70s", "#### DECLINED AMOUNT EXCEEDING LIMIT ####\n");
	/*############################################################################################*/

	/*###############################    Checking if Account is Valid   ##########################*/
	
	EN_serverError_t isValid;
	EN_serverError_t isAvailable;
	EN_serverError_t isBlocked;
	EN_serverError_t isSaved;

	isValid = isValidAccount(cardHolderData_ptr, accountsDB);  // check if account exisits
	if (isValid == SERVER_OK)
	{
		isBlocked = isBlockedAccount(&(accountsDB[index]));    // if acoounts exists, check if it's not blocked
		if (isBlocked == SERVER_OK)
		{
			isAvailable = isAmountAvailable(terminalData_ptr, &(accountsDB[index]));  // if account exists && running, check if it has sufficient funds
			if (isAvailable == SERVER_OK)
			{
				errorState = APPROVED;       
			}
			else
			{
				errorState = DECLINED_INSUFFECIENT_FUND;
			}
		}
		else
		{
			errorState = DECLINED_STOLEN_CARD;
		}
	}
	else
	{
		errorState = FRAUD_CARD;
	}

	transData->transState = errorState;
	
	isSaved = saveTransaction(transData);
	if (isSaved == SAVING_FAILED)
		errorState = INTERNAL_SERVER_ERROR;
	else
	{
		printf("Old Balance: %.2f\n", accountsDB[index].balance);
		accountsDB[index].balance -= terminalData_ptr->transAmount;
		printf("New Balance: %.2f\n", accountsDB[index].balance);
		transTracker++;
	}
	
	return errorState;
}
/**********************************************************************************************************************************************************************/

/**************************************************************    saveTransaction functin    *************************************************************************/

EN_serverError_t saveTransaction(ST_transaction_t* transData)
{
	static uint16_t sequenceNumber = 0;
	if (transTracker < 255)
	{
		sequenceNumber++;
		transData->transactionSequenceNumber = sequenceNumber;     // assign a sequence number to the transaction
		transactionsDB[transTracker] = *transData;				   // add the transaction to the Transactions Data Base
		listSavedTransactions();                                   // print all the transactions on the output
		return SERVER_OK;
	}
	return SAVING_FAILED;      

}
/**********************************************************************************************************************************************************************/

/**************************************************************    saveTransactiontest functin    *************************************************************************/
void saveTransactionTest(void)
{
	ST_transaction_t transData;
	ST_transaction_t* transData_ptr = &transData;
	EN_serverError_t x;

	printf("Tester Name: Mohamed Ibrahim Ezzat\n");
	printf("Function Name: saveTransactionTest\n\n");
	printf("Test Case 1: \n");
	printf("Input Data: PAN: 5550790294712160 amount<10000 to avoid quitting before testing save function\n");
	recieveTransactionData(transData_ptr);
	x = saveTransaction(transData_ptr);
	printf("Expected Result: %s\n", serError[SERVER_OK]);
	printf("Actual Result: %s\n\n", serError[x]);

	transTracker = 255;
	printf("Test Case 2: \n");
	printf("Input Data: index to transactionsDB is set to 255 // saving fail\n");
	recieveTransactionData(transData_ptr);
	x = saveTransaction(transData_ptr);
	printf("Expected Result: %s\n", serError[SAVING_FAILED]);
	printf("Actual Result: %s\n\n", serError[x]);
}

/**********************************************************************************************************************************************************************/
/**************************************************************    listSavedTransactions functin    *******************************************************************/

void listSavedTransactions(void)
{
	for (int i = 0; i <= transTracker; i++)
	{
		printf("#########################\n");
		printf("Transaction Sequence Number: %d\n", transactionsDB[i].transactionSequenceNumber);
		printf("Transaction Date: %s\n", (transactionsDB[i].terminalData).transactionDate);
		printf("Transaction Amount: %.2f\n", (transactionsDB[i].terminalData).transAmount);
		printf("Transaction State: %s\n", transStateError[(transactionsDB[i].transState)]);
		printf("Terminal Max Amount: %.2f\n", (transactionsDB[i].terminalData).maxTransAmount);
		printf("Cardholder Name: %s\n", (transactionsDB[i].cardHolderData).cardHolderName);
		printf("PAN: %s\n", (transactionsDB[i].cardHolderData).primaryAccountNumber);
		printf("Card Expiration Date: %s\n", (transactionsDB[i].cardHolderData).cardExpirationDate);
		printf("#########################\n\n");
	}	
}
/**********************************************************************************************************************************************************************/

/**************************************************************    listSavedTransactionsTest functin    *******************************************************************/

void listSavedTransactionsTest(void)
{
	ST_transaction_t transData;
	ST_transaction_t* transData_ptr = &transData;
	printf("Tester Name: Mohamed Ibrahim Ezzat\n");
	printf("Function Name: listSavedTransactions\n\n");

	printf("Test Case 1: \n");
	printf("Input Data: amount<10000 && valid expiry date\n");
	recieveTransactionData(transData_ptr);
	listSavedTransactions();
	transTracker++;
	
	printf("Test Case 2: \n");
	printf("Input Data: amount<10000 && valid expiry date\n");
	recieveTransactionData(transData_ptr);
	listSavedTransactions();
	transTracker++;

	printf("Test Case 3: \n");
	printf("Input Data: amount<10000 && valid expiry date\n");
	recieveTransactionData(transData_ptr);
	listSavedTransactions();
}
/**************************************************************    recieveTransactionDataTest functin    **************************************************************/

void recieveTransactionDataTest(void)
{
	ST_transaction_t transData;
	ST_transaction_t* transData_ptr = &transData;
	EN_transState_t x;

	printf("Tester Name: Mohamed Ibrahim Ezzat\n");
	printf("Function Name: recieveTransactionData\n\n");

	// test 1: valid PAN number
	printf("Test Case 1: \n");
	printf("Input Data: PAN: 4508363596763285 && amount<10000 && valid expiry date \n");
	x = recieveTransactionData(transData_ptr);
	printf("Expected Result: %s\n", transStateError[APPROVED]);
	printf("Actual Result: %s\n\n", transStateError[x]);

	// test 2: not valid PAN number
	printf("Test Case 2: \n");
	printf("Input Data: PAN: 5181975718047403 && amount<10000 && valid expiry date \n");
	x = recieveTransactionData(transData_ptr);
	printf("Expected Result: %s\n", transStateError[FRAUD_CARD]);
	printf("Actual Result: %s\n\n", transStateError[x]);

	// test 3: withdrawal amount > account balance
	printf("Test Case 3: \n");
	printf("Input Data: PAN: PAN: 8989374615436851 && Amount>2000 && valid expiry date \n");
	x = recieveTransactionData(transData_ptr);
	printf("Expected Result: %s\n", transStateError[DECLINED_INSUFFECIENT_FUND]);
	printf("Actual Result: %s\n\n", transStateError[x]);

	// test 4: Blocked Account
	printf("Test Case 4: \n");
	printf("Input Data: PAN: 4175006510464850 && Amount<100000 && valid expiry date\n");
	x = recieveTransactionData(transData_ptr);
	printf("Expected Result: %s\n", transStateError[DECLINED_STOLEN_CARD]);
	printf("Actual Result: %s\n\n", transStateError[x]);
}
/**********************************************************************************************************************************************************************/
