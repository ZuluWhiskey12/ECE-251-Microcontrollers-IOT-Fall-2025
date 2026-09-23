
#ifndef __PROB3A_H__
#define __PROB3A_H__
#include "ECE251_PIO_util.h"

//Function Prototypes
int isDigitChar(char ch);
char readDigitWithPrompt(char *prompt);
int char2int (char digit);
int getWeight (void);
int getHeightFeet (void);
int getHeightInches (void);
void getHeight(int *heightFeet, int *heightInches);
int convertHeightToInches(int heightFeet, int heightInches);
double calculateBmi (int weightInPounds, int heightFeet, int heightInches);
void displayBmi (double bmi);

#endif /* __PROB3A_H__ */
