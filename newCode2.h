/********************************************
Jacob Young 0932387
Assignment One
This program is used to convert c++ to c code
*********************************************/

#ifndef __NEWCODE2_H__
#define __NEWCODE2_H__

/*
localVar
This function will go through the function and the saved items to see if there is a variable that is needed to be replaced
IN: char * function - the current function that is being checked; char * saved - the information saved from the struct (variables)
OUT: int - a detection saying that a variable being used is for the struct (returns 1 if it finds one)
*/
int localVar(char * function, char * saved);

/*
returnType
This function will go through the function's beginning and determine if it is returning a struct
or if it is returning a pointer like a char *
IN: char * functionCode - this is the function itself
OUT: int - it is a -1 if a struct type, a 0 if it is nothing, and >= 1 for the amount of pointers
*/
int returnType(char * functionCode);

/*
fixingInput
This will check for if the input is weird and then correct it
IN: char * input - the string in question
OUT: char * - the fixed string
*/
char * fixingInput(char * input);

#endif
