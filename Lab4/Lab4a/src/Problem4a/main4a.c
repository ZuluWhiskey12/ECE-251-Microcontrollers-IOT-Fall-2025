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

int main(void)
{
    // Initialize the UART.
    UARTsetup(9600);

    // Problem 3a!
    UARTprintf("Hello, Lab 4!\n");
		UARTprintf("Problem 4a\n\n");

    Student list[3];

    //Bob
    list[0] = createStudent("Bob", 1234);

    addCourse(&list[0], "Intro to Basketweaving", 101, 'A', 2);
    addCourse(&list[0], "Relativity and Quantum Mechan", 500, 'C', 3);
    addCourse(&list[0], "Intro to Microcontollers", 251, 'B', 3);

    //Fred
    list[1] = createStudent("Fred", 2345);
    // no courses added

    //Ann
    list[2] = createStudent("Ann", 5678);
    addCourse(&list[2], "Relativity and Quantum Mechan", 500, 'A', 3);
    addCourse(&list[2], "Intro to Microcontollers", 251, 'A', 3);
    addCourse(&list[2], "Suffering Fools Gladly", 102, 'D', 2);
    addCourse(&list[2], "Honors English", 300, 'A', 3);

    // Loop through all students in the list
    for (int i = 0; i < 3; i++) {
        printStudent(&list[i]);

        float gpa = calcGPA(&list[i]);
        UARTprintf("GPA: %f\n\n", gpa);
    }
	
    // We are finished. Since this is an embedded system, hang around
    while(1) { }
}
