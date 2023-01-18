#ifndef required_h
#define required_h

#include <stdio.h>
#include <stdbool.h>
#include <string.h>


typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long int uint64_t;

void inputf(char* str,int strLength);
bool alldigit(char* digitStr);
bool isAlphabetic(char* name);

void flush_input(void);





#endif 
