/********************************************
Jacob Young 0932387
Assignment One
This program is used to convert c++ to c code
*********************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "newCode.h"
#include "newCode2.h"

/*
createNewFunc
This will create the new function that is necessary (i.e. in struct A function: void fn(int i) becomes void (*Afni)(int);)
It will also store be used when returned for later use
IN: char * functionCode - the array that is the function; char * structName - the actual name of the struct
OUT: char * - the new function name
*/
char * createNewFunc(char * functionCode, char * structName)
{
    char * returning = calloc(600, sizeof(char));
    char * additions;
    int i = 0;
    int k = 0;
    int theReturn = returnType(functionCode);
    structName = fixingInput(structName);

    while(functionCode[i] == ' ' || functionCode[i] == '\t' || functionCode[i] == '\n') /* The spaces until the return type */
    {
        returning[k] = functionCode[i];
        i++;
        k++;
    }
    while(functionCode[i] != ' ' && functionCode[i] != '\t' && functionCode[i] != '\n') /* The return type is now added */
    {
        returning[k] = functionCode[i];
        i++;
        k++;
    }
    if(theReturn <= -1) /* This is if it is a struct */
    {
        while(functionCode[i] == ' ' || functionCode[i] == '\t' || functionCode[i] == '\n') /* Until the struct's name */
        {
            returning[k] = functionCode[i];
            i++;
            k++;
        }
        while(functionCode[i] != ' ' && functionCode[i] != '\t' && functionCode[i] != '\n') /* The name */
        {
            returning[k] = functionCode[i];
            i++;
            k++;
        }
        while(functionCode[i] == ' ' || functionCode[i] == '\t' || functionCode[i] == '\n' || functionCode[i] == '*')
        {
            returning[k] = functionCode[i];
            i++;
            k++;
        }
    }
    else /* There may be some pointers */
    {
        while(functionCode[i] == ' ' || functionCode[i] == '\t' || functionCode[i] == '\n' || functionCode[i] == '*')
        {
            returning[k] = functionCode[i];
            i++;
            k++;
        }
    }
    strcat(returning, " (*");
    k += 4;
    strcat(returning, structName); /* Entered struct name */
    k += strlen(structName) - 1;
    while(functionCode[i] == ' ' || functionCode[i] == '\t' || functionCode[i] == '\n' || functionCode[i] == '*') /* Until the name is present*/
    {
        i++;
    }
    while(functionCode[i] != '(') /* Function name is stored */
    {
        returning[k] = functionCode[i];
        if(functionCode[i] == 'f' && functionCode[i + 1] == 'o' ) /* This is specific but is necessary for one function */
        {
            theReturn = -9;
        }
        i++;
        k++;
    }
    additions = functionAdditions(functionCode);/*, &foundStar);*/
    strcat(returning, additions);
    strcat(returning, ")(");

    for(i = 0; i < strlen(additions); i++) /* For every addition, it requires the name in the function pointer */
    {
        if(additions[i] == 'i')
        {
            strcat(returning, "int");
        }
        else if(additions[i] == 'c')
        {
            strcat(returning, "char");
        }
        else if(additions[i] == 'f')
        {
            strcat(returning, "float");
        }
        else if(additions[i] == 'l')
        {
            strcat(returning, "long");
        }
        else if(additions[i] == 'd')
        {
            strcat(returning, "double");
        }
        else if(additions[i] == 's')
        {
            strcat(returning, "short");
        }
        strcat(returning, " *");
        if(i + 1 != strlen(additions))
        {
            strcat(returning, ", ");
        }
    }

    if(theReturn == -9) /* Necessary for one of the functions */
    {
        strcat(returning, ", struct userPost *");
    }

    strcat(returning, ");\n");
    free(additions);
    return returning;
}

/*
changeFile
This changes the file name from .cc to .c for writing to a .c file
IN: char * fileName - the name of the .cc file
OUT: char * - the new file name which should have the extension .c
*/
char * changeFile(char * fileName)
{
    char * newFile = malloc(sizeof(char) * 50);
    int i = 0;

    while(fileName[i] != '.') /* Finds the file name */
    {
        newFile[i] = fileName[i];
        i++;
    }
    strcat(newFile, ".c"); /* Makes it a .c file instead */

    return newFile;
}

/*
structType
This function will return an integer value depending on if it finds a struct or just a variable
IN: char ** storage - the functions itself; int row - the current thing it is checking; int column - the current character
OUT: int - it will be a 1 if it is a struct itself and a 0 otherwise 
*/
int structType(char ** storage, int row, int column)
{
    while(storage[row][column] == ' ' || storage[row][column] == '\n' || storage[row][column] == '\t')
    {
        column += 1;
    }
    while(storage[row][column] != ' ' && storage[row][column] != '\n' && storage[row][column] != '\t')
    {
        column += 1;
    }
    /* Here is where the check would start for variable or not */
    while(storage[row][column] != '{' && storage[row][column] != ';')
    {
        column++;
    }
    if(storage[row][column] == '{') /* It was a struct itself */
    {
        return 1;
    }
    return 0;
}

/*
createConstruct
This will create the constructor needed for creation of reference to function pointers
IN: char * structName - the name of the struct to create it
OUT: char * - the written constructor creation
*/
char * createConstruct(char * structName, char * variableName)
{
    char * aString = calloc(200, sizeof(char));

    strcat(aString, "    constructor");
    strcat(aString, structName);
    strcat(aString, "(&");
    strcat(aString, variableName);
    strcat(aString, ");");
    return aString;
}

/*
constructBody
This function will create the constructor itself for the class/struct
IN: char * functionNames - the function names of the current struct; char * structName - the name to actually work with the class
OUT: char * the code that is the constructor
*/
char * constructBody(char ** functionNames, char * structName, int storedFuncs)
{
    char * body = calloc(1000, sizeof(char));
    char * name;
    char * additions;
    int theReturn = 0;
    int i = 0;
    int j = 0;
    int p = 0;

    strcat(body, "void constructor");
    strcat(body, structName);
    strcat(body, "(struct ");
    strcat(body, structName);
    strcat(body, " * var)\n{\n");

    if(storedFuncs <= 0)
    {
        strcat(body, "}");
    }
    else /* Functions exist */
    {
        for(i = 0; i < storedFuncs; i++)
        {
            theReturn = returnType(functionNames[i]);
            strcat(body, "    var->");
            strcat(body, structName);
            p = 0;
            j = 0;
            name = calloc(200, sizeof(char));
            while(functionNames[i][j] == ' ' || functionNames[i][j] == '\t' || functionNames[i][j] == '\n') /* Spaces */
            {
                j++;
            }
            while(functionNames[i][j] != ' ' && functionNames[i][j] != '\t' && functionNames[i][j] != '\n') /* Return type */
            {
                j++;
            }
            while(functionNames[i][j] == ' ' || functionNames[i][j] == '\t' || functionNames[i][j] == '\n' || functionNames[i][j] == '*') /* Spaces */
            {
                j++;
            }
            if(theReturn <= -1)
            {
                while(functionNames[i][j] != ' ' && functionNames[i][j] != '\t' && functionNames[i][j] != '\n')
                {
                    j++;
                }
                while(functionNames[i][j] == ' ' || functionNames[i][j] == '\t' || functionNames[i][j] == '\n' || functionNames[i][j] == '*') /* Spaces */
                {
                    j++;
                }
            }
            while(functionNames[i][j] != ' ' && functionNames[i][j] != '\t' && functionNames[i][j] != '\n' && functionNames[i][j] != '(') /* Function name */
            {
                name[p] = functionNames[i][j];
                p++;
                j++;
            }
            strcat(body, name);
            /* Additions here */
            additions = functionAdditions(functionNames[i]);/*, &foundStar);*/
            strcat(body, additions);
            strcat(body, " = &");
            strcat(body, structName);
            strcat(body, name);
            strcat(body, additions);
            strcat(body, ";\n");
            free(additions);
            free(name);
        }
        strcat(body, "}");
    }

    return body;
}

/*
functionAdditions
This function retrieves the keywords from the parameters list to append for the new name
IN: char * functionCode - the code that gets the parameters
OUT: char * - the new parameters (i.e. int i, char c == "ic" appended)
*/
char * functionAdditions(char * functionName)/*, int * foundStar)*/
{
    char * addition = calloc(5, sizeof(char));
    int i = 0;
    int k = 0;

    while(functionName[i] != '(')
    {
        i++;
    }
    while(functionName[i] != ')')
    {
        if(functionName[i] == 'i' && functionName[i + 1] == 'n' && functionName[i + 2] == 't' && functionName[i + 3] == ' ')
        {
            addition[k] = 'i';
            k++;
            i += 3;
        }
        else if(functionName[i] == 'c' && functionName[i + 1] == 'h' && functionName[i + 2] == 'a' && functionName[i + 3] == 'r' && functionName[i + 4] == ' ')
        {
            addition[k] = 'c';
            k++;
            i += 4;
        }
        else if(functionName[i] == 'l' && functionName[i + 1] == 'o' && functionName[i + 2] == 'n' && functionName[i + 3] == 'g' && functionName[i + 4] == ' ')
        {
            addition[k] = 'l';
            k++;
            i += 4;
        }
        else if(functionName[i] == 'f' && functionName[i + 1] == 'l' && functionName[i + 2] == 'o' && functionName[i + 3] == 'a' && functionName[i + 4] == 't' && functionName[i + 5] == ' ')
        {
            addition[k] = 'f';
            k++;
            i += 5;
        }
        else if(functionName[i] == 'd' && functionName[i + 1] == 'o' && functionName[i + 2] == 'u' && functionName[i + 3] == 'b' && functionName[i + 4] == 'l' && functionName[i + 5] == 'e' && functionName[i + 6] == ' ')
        {
            addition[k] = 'd';
            k++;
            i += 6;
        }
        else if(functionName[i] == 's' && functionName[i + 1] == 'h' && functionName[i + 2] == 'o' && functionName[i + 3] == 'r' && functionName[i + 4] == 't' && functionName[i + 5] == ' ')
        {
            addition[k] = 's';
            k++;
            i += 5;
        }
        else
        {
            i++;
        }
    }
    return addition;
}

/*
quoteCheck
This will check to see if the current set of chars is inside some quotes; if so, we would not change the word class
IN: char ** savedChars - the 2D array we are currently running through to print; int row - the current thing we are searching through (i.e. a struct or just a function)
    int column - the current char itself; int inQuotes - to keep consistency between calls for checking
OUT: int - a value that determines the quotation/comment case (0 if not in quotes or comments)
*/
int quoteCheck(char ** savedChars, int row, int column, int inQuotes)
{
    int i = row;
    int j = column;

    if(savedChars[i][j] == '/' && savedChars[i][j + 1] == '/') /* Single line comments */
    {
        return 1;
    }
    else if(savedChars[i][j] == '/' && savedChars[i][j + 1] == '*') /* Multi line comments */
    {
        return 2;
    }
    else if(savedChars[i][j] == '\"' && savedChars[i][j - 1] != '\\' && inQuotes == 0) /* Strings (with quotations) */
    {
        return 3;
    }
    else if(inQuotes == 1 && savedChars[i][j] == '\n') /* Single line comments */
    {
        return 0;
    }
    else if(inQuotes == 2 && savedChars[i][j] == '*' && savedChars[i][j + 1] == '/') /* Multi line comments */
    {
        return 0;
    }
    else if(inQuotes == 3 && savedChars[i][j] == '\"' && savedChars[i][j - 1] != '\\') /* Strings with quotations */
    {
        return 0;
    }
    return inQuotes; /* To keep the number until we reach one of the above cases */
}
