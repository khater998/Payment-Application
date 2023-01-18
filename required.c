#include "required.h"

/****************************************************  inputf Function ********************************************************/
// inputf is a function that accepte a user input and store it in its string arguement.
// it keeps reading till THE NEWLINE Character is encountered.
// it can store up to strLength number of characters, any thing above that is flushed from the input buffer.
void inputf(char* str, int strLength)
{
	char c;
	int i = 0;
	bool x = 0;

	while ((c = getchar()) != '\n')
	{
		if (i < strLength - 1)    // last index of array = length - 1
		{
			str[i] = c;
			i++;
		}

	}
	str[i] = '\0';
}
/******************************************************************************************************************************/

/****************************************************  isAlphabetic Function **************************************************/
// isAlphabetic is a function to check if the passed string is all alphabetic characters or not
bool isAlphabetic(char* name)
{
	int i = 0;
	while ((isalpha(name[i]) || name[i] == ' ') && name[i] != '\0')
	{
		i++;
	}
	if (name[i] == '\0')
		return true;
	return false;
}
/******************************************************************************************************************************/

/****************************************************  alldigit Function ******************************************************/
// alldigit is a function to check if the passed string is all digit numbers
bool alldigit(char* digitStr)
{
	int length;
	length = strlen(digitStr);

	int i = 0;
	while (isdigit(digitStr[i]))
	{
		i++;
	}
	if (digitStr[i] == '\0')
		return true;
	return false;
}
/******************************************************************************************************************************/
// flush_input is a function to clear the input buffer
void flush_input(void)
{
	char c;
	while ((c = getchar()) != '\n' )
	{
		/*discard*/
	}
	return;
}