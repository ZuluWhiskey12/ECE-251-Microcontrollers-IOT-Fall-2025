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

#ifndef __STUDENT_H__
#define __STUDENT_H__

//Struct definition for course goes here. Prelab
struct course {
    char name[30];    
    int id;          
    char grade;     
    int credits;
};

typedef struct course Course;

struct student {
	char firstName[20];
	int id;
	Course courseHistory[20];
	int numCoursesTaken;
};

typedef struct student Student;

Student createStudent(char name[20], int id);
void addCourse (Student *inputStudent, char courseName[30], int courseId, char courseGrade, int courseCredits);
void printStudent (Student *inputStudent);
int gradeToPoints(char grade);
float calcGPA (Student *inputStudent);

#endif /* __STUDENT_H__ */
