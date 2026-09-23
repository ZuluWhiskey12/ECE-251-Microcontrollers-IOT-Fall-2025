
#ifndef __PROB3B_H__
#define __PROB3B_H__
#include "ECE251_PIO_util.h"

int isDigitChar(char ch);
int char2Int(char digit);
int getWeight(void);
int getHeightFeet(void);
int getHeightInches(void);
int convertHeightToInches(int heightFeet, int heightInches);
int getAge(void);
char getGender(void);
int getActivityLevel(void);
double calculateBMR(int weightLbs, int heightInches, int age, char gender);
double appliedActivity(double bmr, int level);
void displayCalories(double calories);
void displayBMR(double bmr);

#endif /* __PROB3B_H__ */
