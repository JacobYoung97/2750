/********************************************
Jacob Young 0932387
Assignment One
This program is used to convert c++ to c code
*********************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "newCode2.h"

/*
localVar
This function will go through the function and the saved items to see if there is a variable that is needed to be replaced
IN: char * function - the current function that is being checked; char * saved - the information saved from the struct
OUT: int - a detection saying that a variable being used is for the struct (returns 1 if it finds one)
*/
int localVar(char * function, char * saved)
{
    int variableFlag = 0;
    int inFirstBrackets = 0;
    char * variables = malloc(sizeof(char) * 2000);
    int foundVar = 0;
    int i;
    int j = 0;

    for(i = 0; i < strlen(function); i++)
    {
        if(function[i] == '(' && inFirstBrackets == 0) /* We have reached the variables list */
        {
            inFirstBrackets = 1;
            i++;
            while(function[i] != ')') /* Until the end of the variables list */
            {
                while(function[i] != ',' && function[i] != ')')
                {
                    variables[j] = function[i];
                    i++;
                    j++;
                }
                variables[j] = '/';
                i++;
                j++;
            }
            i--;
        }
        else
        {
            if(function[i] == 'i' && function[i + 1] == 'n' && function[i + 2] == 't' && (function[i + 3] == ' ' || function[i + 3] == '\t'))
            {
                foundVar = 1;
                variables[j] = function[i];
                variables[j + 1] = function[i + 1];
                variables[j + 2] = function[i + 2];
                variables[j + 3] = ' ';
                j+= 4;
                i+= 3;
            }
            else
            {

            }

            if(foundVar == 1)
            {
                while(function[i] == ' ' || function[i] == '\t' || function[i] == '\n')
                {
                    i++;
                }
                while(function[i] != ' ' && function[i] != ';')
                {
                    variables[j] = function[i];
                    j++;
                    i++;
                }
                variables[j] = '/';
            }
        }
    }

    free(variables);
    return variableFlag;
}

/*
returnType
This function will go through the function's beginning and determine if it is returning a struct
or if it is returning a pointer like a char *
IN: char * functionCode - this is the function itself
OUT: int - it is a <= -1 if a struct type with -1 being no pointers
*/
int returnType(char * functionCode)
{
    int i = 0;
    int theType = 0;

    for(i = 0; functionCode[i] != '('; i++)
    {
        if(functionCode[i] == 's' && functionCode[i + 1] == 't' && functionCode[i + 2] == 'r') /* It is a struct */
        {
            theType = -1;
            i += 2;
        }
        else if(functionCode[i] == '*')
        {
            if(theType < 0) /* A struct with some pointers for return type will keep decreasing */
            {
                theType--;
            }
        }
    }
    return theType;
}

/*
fixingInput
This will check for if the input is weird and then correct it
IN: char * input - the string in question
OUT: char * - the fixed string
*/
char * fixingInput(char * input)
{
    int i = 0;

    for(i = 0; i < strlen(input); i++)
    {
        if(((int)input[i] - 65 >= 0 && (int)input[i] - 65 <= 25) || ((int)input[i] - 97 >= 0 && (int)input[i] - 97 <= 25)) /* This checks for weird characters */
        {

        }
        else
        {
            input[i] = '\0';
        }
    }

    return input;
}
