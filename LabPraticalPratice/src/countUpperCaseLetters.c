/*
* Write a program to count upper case letters in a string created from user input
*   1. Prompt user for string input
*   2. Store input into an Array
*   3. Count uppercase letter in the Array
*   4. Print out the count
*/
#define maxLine 128;

int position = 0;

char inStr[maxLine];
char inChar;

UARTPrintf("Enter a String\n");

do
{
    /* code */
    inChar = UARTgetc_echo();
    inStr = inChar;
    position++

} while ((in_char != '\r' || '\n') && (position < maxLine));

inStr[--position] = '\0';

int count = 0;
for (int i = 0; i < position; i++) { //Could also do for (int i = 0; inStr[i] != '\0'; i++)
    char c = inStr[i];

    if ((c >= 'A') && (c <= 'Z')) {
        count ++
    }
}

UARTPrintf("count = %d\n", count);
