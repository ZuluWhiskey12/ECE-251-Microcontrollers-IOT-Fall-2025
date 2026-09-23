
#ifndef __PROB3C_H__
#define __PROB3C_H__
#include "ECE251_PIO_util.h"

int isDigitChar(char ch);
int char2Int (char digit);
char getGrade(int index);
int getCredits(int index);
int gradeToPoints(char grade);
void getAllClasses(char grades[], int credits[]);
double computeGPA(char grades[], int credits[]);
void displayGPA(double gpa);

#endif /* __PROB3C_H__ */
