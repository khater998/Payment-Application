#include "app.h"

void appStart(void)
{
	ST_transaction_t transData;
	ST_transaction_t* transData_ptr = &transData;

	char again[8];
	do
	{
		recieveTransactionData(transData_ptr);
		printf("\nEnter END to end or anykey to continue: ");
		inputf(again,5);
	}while (strcmp(again, "END") != 0);
}