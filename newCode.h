/********************************************
Jacob Young 0932387
Assignment One
This program is used to convert c++ to c code
*********************************************/
#include<stdlib.h>

#ifndef __NEWCODE_H__
#define __NEWCODE_H__

/*
createNewFunc
This will create the new function that is necessary (i.e. in struct A function void fn(int i) becomes void (*Afni)(int);)
It will also be stored for later use.
IN: char * functionCode - the array that is the function; char * structName - the actual name of the struct
OUT: char * - the new function name
*/
char * createNewFunc(char * functionCode, char * structName);

/*
changeFile
This changes the file name from .cc to .c for writing to a .c file
IN: char * fileName - the name of the .cc file
OUT: char * - the new file name which should have the extension .c
*/
char * changeFile(char * fileName);

/*
structType
This function will return an integer value depending on if it finds a struct or just a variable
IN: char ** storage - the functions itself; int row - the current thing it is checking; int column - the current character
OUT: int - it will be a 1 if it is a struct itself and a 0 otherwise (a variable)
*/
int structType(char ** storage, int row, int column);

/*
createConstruct
This will create the constructor needed for creation of reference to function pointers (constructorA(&myClass);)
IN: char * structName - the name of the struct to create it; char * variableName - the variable being referenced
OUT: char * - the written constructor creation
*/
char * createConstruct(char * structName, char * variableName);

/*
constructBody
This function will create the constructor itself for the class/struct
IN: char * functionNames - the function names of the current struct; char * structName - the name to actually work with the class
    int storedFuncs - the amount of functions that were saved from the struct
OUT: char * the code that is the constructor body and the function pointers now reference everything
*/
char * constructBody(char ** functionNames, char * structName, int storedFuncs);

/*
functionAdditions
This function retrieves the keywords from the parameters list to append for the new name
IN: char * functionCode - the code that gets the parameters (function itself)
OUT: char * - the new parameters (i.e. int i, char c == "ic" appended)
*/
char * functionAdditions(char * functionName);/*, int * foundStar);*/

/*
quoteCheck
This will check to see if the current set of chars is inside some quotes; if so, we would not change the word class
IN: char ** savedChars - the 2D array we are currently running through to print; int row - the current thing we are searching through (i.e. a struct or just a function)
    int column - the current char itself; int inQuotes - to keep consistency between calls for checking
OUT: int - a value that determines the quotation/comment case (0 if out of quote/comment; 1, 2, or 3 otherwise)
*/
int quoteCheck(char ** savedChars, int row, int column, int inQuotes);

#endif
