#include "../include/Helpers.h"
#include <stdlib.h>
#include <string.h>

/*
* Parsing to int value from hex value as string.
*/
int GetDecimalFromHex(char* hexValue)
{
	return (int)strtol(hexValue, NULL, 16);
}

/*
* Parsing to int value from hex value as string in 2s complement representation.
*/
int GetDecimalFromHex2Comp(char* hexValue)
{
	if (hexValue[0] == '0')
		return GetDecimalFromHex(hexValue);

	return ((GetDecimalFromHex(hexValue) & 0xFFFFF) << 12) >> 12;
}

/*
* Removing the last character of a string.
*/
void RemoveLastChar(char* str) {
	str[strlen(str) - 1] = '\0';
}
