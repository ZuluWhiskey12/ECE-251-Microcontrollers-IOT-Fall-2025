
/*****************************************************************************
* Programmer: Ryan G. Kim
* Class: ECE 251; Lab Section 0
* Programming Assignment: Lab 4a - Student records
*
* Date:
*
* Description: 
*
* Relevant Formulas:
*****************************************************************************/

#include "ECE251_PIO_util.h"
#include "student.h"

/*************************************************************
* Function: createStudent                                   *
* Date Created:                                             *
* Date Last Modified:                                       *
* Description:                                              *
* Input parameters:                                         *
* Returns: None                                             *
* Preconditions:                                            *
* Postconditions:                                           *
*************************************************************/
Student createStudent(char inputName[10], int inputId) {
    Student student;

    int i = 0;
    while (inputName[i] != '\0' && i < 10) {
        student.firstName[i] = inputName[i];
        i++;
    }
    student.firstName[i] = '\0';
    student.id = inputId;
    student.numCoursesTaken = 0;

    return student;
}

/*************************************************************
* Function: addCourse                                       *
* Date Created:                                             *
* Date Last Modified:                                       *
* Description:                                              *
* Input parameters:                                         *
* Returns: None                                             *
* Preconditions:                                            *
* Postconditions:                                           *
*************************************************************/
void addCourse (Student *inputStudent, char courseName[30], int courseId, char courseGrade, int courseCredits) {
    int index = inputStudent -> numCoursesTaken;

    int i = 0;
    while (courseName[i] != '\0' && i < 30) {
        inputStudent -> courseHistory[index].name[i] = courseName[i];
        i++;
    }
    inputStudent -> courseHistory[index].name[i] = '\0';

    inputStudent -> courseHistory[index].id = courseId;
    inputStudent -> courseHistory[index].grade = courseGrade;
    inputStudent -> courseHistory[index].credits = courseCredits;

    inputStudent -> numCoursesTaken++;
}

void printStudent (Student *inputStudent) {

    UARTprintf("Student Name: %s\n", inputStudent -> firstName);
    UARTprintf("Student ID: %d\n", inputStudent -> id);

    if (inputStudent->numCoursesTaken == 0) {
        UARTprintf("    (No classes)\n");
    } 
    else { 
        for (int i = 0; i < inputStudent -> numCoursesTaken; i++) {

        UARTprintf("    Class: %s\n", inputStudent -> courseHistory[i].name);
        UARTprintf("        CRN: %d", inputStudent -> courseHistory[i].id);
        UARTprintf("    Grade: %c", inputStudent -> courseHistory[i].grade);
        UARTprintf("    Credits: %d\n\n", inputStudent -> courseHistory[i].credits);
        }
    }
}

int gradeToPoints(char grade) {
    switch (grade) {
        case 'A': return 4;
        case 'B': return 3;
        case 'C': return 2;
        case 'D': return 1;
        default:  return 0;
    }
}

float calcGPA (Student *inputStudent) {
    int totalGradePoints = 0;
    int totalCredits = 0;

    for (int i = 0; i < inputStudent -> numCoursesTaken; i++) {
        totalGradePoints += gradeToPoints(inputStudent -> courseHistory[i].grade) * (inputStudent -> courseHistory[i].credits);
        totalCredits += inputStudent -> courseHistory[i].credits;
    }

    if (totalCredits == 0) {
        return 0.0;   
    }

    return (float)totalGradePoints / totalCredits;
}
