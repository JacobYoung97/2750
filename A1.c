/********************************************
Jacob Young 0932387
Assignment One
This program is used to convert c++ to c code
*********************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"newCode.h"
#include"newCode2.h"

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        printf("You do not have the correct amount of arguments!\n");
        exit(0);
    }
    FILE * filePtr; /* The reading from file */
    FILE * writtenPtr; /* The writing to file */
    char * fileName = malloc(sizeof(char)* 50);
    char * newFile;
    int inStruct = 0;
    int stillStruct = 0;
    int isFunc = 0;
    int isUnique = 0;
    int inQuotes = 0;
    int theChar = 0;
    char convertedChar = '\0';
    char * structName; /* The struct name when read in (or compared with) */
    char ** storage; /* 2D array for storing the whole file inside of */
    char ** functionNames; /* The function names themselves (used to compare and remember for later) */
    char ** savedFunctions; /* This is for when we move function outside */
    char * tempName;
    char * tempVar;
    char * constructor;
    char * additions;
    int i = 0;
    int j = 0;
    int k = 0; /* Tertiary variable used to loop through line to check for some stuff before going too far */
    int m; /* To store the amount of functions that were inside a struct */
    int l = 0;
    int r = 0;
    int p = 0;
    int thisFunc = 0;
    int curlyBraces = 0;
    int rows = 10;
    int columns = 2000;
    int curLine = 0;
    int curChar = 0;
    int amountFuncs = 20;
    int storedFuncs = 0;
    int savedItems = -1;
    int saved = 0;
    int left = 0;
    int right = 0;
    int notEqual = 0;
    int theReturn = 0;
    char * function;
    char * classVar;
    char * newName;
    int * numFuncs = calloc(amountFuncs, sizeof(int));
    storage = calloc(rows, sizeof(char*));
    savedFunctions = malloc(sizeof(char*) * rows);
    functionNames = malloc(sizeof(char*) * 30);
    for(i = 0; i < rows; i++)
    {
        storage[i] = calloc(columns, sizeof(char));
        savedFunctions[i] = calloc(columns, sizeof(char));
    }
    for(i = 0; i < 20; i++)
    {
        functionNames[i] = malloc(sizeof(char) * columns);
    }
    i = 0;

    /* The following converts the file name */
    filePtr = fopen(argv[1], "r");
    if(filePtr == NULL)
    {
        printf("Error opening the cc file!\n");
        free(fileName);
        exit(0);
    }
    strcpy(fileName, argv[1]);
    newFile = changeFile(fileName);
    writtenPtr = fopen(newFile, "w");
    /* Error opening file */
    if(writtenPtr == NULL)
    {
        printf("Error opening file!\n");
        free(fileName); free(newFile);
        exit(0);
    }

    /* This gets every single char from the file */
    while((theChar = fgetc(filePtr)) != EOF)
    {
        convertedChar = (char)theChar;
        switch(convertedChar)
        {
            case '{':
                curlyBraces += 1;
                storage[curLine][curChar] = convertedChar;
                curChar++;
                break;
            case '}':
                curlyBraces -= 1;
                if(curlyBraces == 0)
                {
                    curlyBraces = -1; /* This is used to signal and check if a class (to keep the ; on the same thing) */
                }
                storage[curLine][curChar] = convertedChar;
                curChar++;
                break;

            case ';':
                storage[curLine][curChar] = convertedChar;
                if(curlyBraces == -1)
                {
                    /* Because of class, then increase line */
                    curLine++;
                    curChar = 0;
                    curlyBraces = 0;
                }
                else if(curlyBraces == 0) /* They have found a function prototype, a global variable */
                {
                    curLine++;
                    curChar = 0;
                }
                else
                {
                    curChar++; /* Just a standard end of a line statement */
                }
                break;

            case '\n':
            case ' ':
            case '\t':
            case '\r':
                storage[curLine][curChar] = convertedChar;
                curChar++;
                break;

            default:
                if(curlyBraces == -1) /* The char was something not a whitespace or ; then it would have to be stored on a new line */
                {
                    curLine++;
                    curChar = 0;
                    curlyBraces = 0;
                }
                storage[curLine][curChar] = convertedChar;
                curChar++;
                break;
        }
        if(curLine == (rows - 1)) /* Need to realloc because we ran out of room */
        {
            rows += 5; /* Gets quite large rather quickly but we shouldn't need much past 40 */
            storage = realloc(storage, sizeof(char*) * rows);
            for(i = rows/2; i < rows; i++)
            {
                storage[i] = calloc(columns, sizeof(char));
            }
        }
        else if(curChar == (columns - 1))
        {
            columns += 500; /* Can get quite large */
            storage[curLine] = realloc(storage[curLine], sizeof(char) * columns);
        }
    }
    savedFunctions = realloc(savedFunctions, sizeof(char*) * rows);
    for(i = 0; i < rows; i++)
    {
        savedFunctions[i] = realloc(savedFunctions[i], sizeof(char) * columns);
    }
    curlyBraces = 0;
    /* This is when we go through each character and do stuff with it */
    for(i = 0; i < rows; i++)
    {
        storedFuncs = 0;
        isUnique = 0;
        isFunc = 0;
        inStruct = 0;
        stillStruct = 0;
        m = 0;
        l = 0;
        structName = calloc(50, sizeof(char));
        for(j = 0; j < strlen(storage[i]); j++)
        {
            inQuotes = quoteCheck(storage, i, j, inQuotes); /* Check whether "class" should be replaced */
            if(inStruct == 0) /* Not in a struct */
            {
                if(storage[i][j] == 'c' && storage[i][j + 1] == 'l' && storage[i][j + 2] == 'a' && storage[i][j + 3] == 's' && storage[i][j + 4] == 's' && (storage[i][j + 5] == ' ' || storage[i][j + 5] == '\n') && inQuotes == 0)
                {
                    if(isFunc == 0)
                    {
                        fputs("struct", writtenPtr);
                        j += 5;
                    }

                    /* This is where it is checked for being a variable or not */
                    k = j;
                    inStruct = structType(storage, i, k);
                    /* Needs to check if already inside of a struct */
                    if(inStruct == 1) /* They are inside a struct so we print until we reach the { */
                    {
                        stillStruct = 1;
                        while(storage[i][j] == ' ' || storage[i][j] == '\n' || storage[i][j] == '\t') /* Spaces until the name */
                        {
                            fputc(storage[i][j], writtenPtr);
                            j++;
                        }
                        while(storage[i][j] != ' ' && storage[i][j] != '\t' && storage[i][j] != '\n' && storage[i][j] != '{') /* The name of the struct */
                        {
                            structName[l] = storage[i][j];
                            fputc(storage[i][j], writtenPtr);
                            j++;
                            l++;
                        }
                        savedItems++;
                        strcat(functionNames[savedItems], structName);
                        strcat(functionNames[savedItems], "/"); /* Now that we have started to store this stuff, we need to compare for other things later */
                        fputc(storage[i][j], writtenPtr); /* Need to update savedItems at some point (update when finding a new struct or when going to next thing in original 2D?) */
                    }
                    else /* A variable so we print the space */
                    {
                        fputc(storage[i][j], writtenPtr); /* At this part, we should go through until the ; and then print constructor */
                        j++;
                        while(storage[i][j] == '\n' || storage[i][j] == ' ' || storage[i][j] == '\t')
                        {
                            fputc(storage[i][j], writtenPtr);
                            j++;
                        }
                        tempName = calloc(50, sizeof(char));
                        tempVar = calloc(50, sizeof(char));
                        p = 0;
                        while(storage[i][j] != '\n' && storage[i][j] != ' ' && storage[i][j] != '\t') /* Gets the struct */
                        {
                            tempName[p] = storage[i][j];
                            fputc(storage[i][j], writtenPtr);
                            j++;
                            p++;
                        }
                        tempName[p] = '\0';
                        while(storage[i][j] == ' ' || storage[i][j] == '\t' || storage[i][j] == '\n') /* Up to the variable name */
                        {
                            fputc(storage[i][j], writtenPtr);
                            j++;
                        }
                        p = 0;
                        while(storage[i][j] != ' ' && storage[i][j] != '\n' && storage[i][j] != '\t' && storage[i][j] != ';')
                        {
                            tempVar[p] = storage[i][j]; /* Store the variable's name */
                            fputc(storage[i][j], writtenPtr);
                            p++;
                            j++;
                        }
                        while(storage[i][j] != '\n')
                        {
                            fputc(storage[i][j], writtenPtr);
                            j++;
                        }
                        fputc(storage[i][j], writtenPtr);
                        constructor = createConstruct(tempName, tempVar); /* Create the constructor */
                        fputs(constructor, writtenPtr);
                        fputc('\n', writtenPtr);
                        for(p = 0; p <= savedItems; p++) /* Associate the variable name with the struct name for later comparisons */
                        {
                            notEqual = 0;
                            for(l = 0; l < strlen(tempName); l++)
                            {
                                if(tempName[l] != functionNames[p][l])
                                {
                                    notEqual = 1;
                                }
                            }
                            if(notEqual == 0) /* We found the matching struct so store this with it */
                            {
                                strcat(functionNames[p], tempVar);
                                strcat(functionNames[p], "/");
                            }
                        }
                        free(constructor);
                        free(tempName);
                        free(tempVar);
                    }
                }
                else /* Anything else just prints the character found */
                {
                    fputc(storage[i][j], writtenPtr);
                    /* This is where the check for a dot will occur for functions that were not part of the struct */
                    if(storage[i][j] == '.' && storage[i][j + 1] != 'h' && storage[i][j + 1] != '2')
                    {
                        saved = j;
                        classVar = calloc(30, sizeof(char));
                        function = calloc(30, sizeof(char));
                        left = 0;
                        right = 0;
                        p = 0;

                        while(storage[i][saved + right] != '(' && storage[i][saved + right] != '=')
                        {
                            right++; /* The function name itself */
                        }
                        while(storage[i][saved - left] != ' ')
                        {
                            left++;
                        }
                        if(storage[i][saved + right] == '(') /* They found a function (variable does not matter) */
                        {
                            for(p = saved + 1; p < saved + right; p++)
                            {
                                function[p - (saved + 1)] = storage[i][p];
                            }
                        }
                        for(p = saved - left; p < saved; p++) /* The struct variable itself */
                        {
                            if(storage[i][p + 1] != '.')
                            {
                                classVar[p - (saved - left)] = storage[i][p + 1];
                            }
                        }
                        /* Now here, we compare the found function to the variable and see what to change to */
                        for(p = 0; p <= savedItems; p++)
                        {
                            notEqual = 0;
                            r = 0;
                            left = 0;
                            for(l = 0; l < strlen(functionNames[p]); l++) /* Everything saved */
                            {
                                right = l; /* We are going right of the start */
                                left = 0;
                                while(functionNames[p][l] != '/') /* Length of the var */
                                {
                                    if(functionNames[p][l] != function[r] && (l - right) < strlen(function))
                                    {
                                        notEqual = 1;
                                        left = 0;
                                    }
                                    else
                                    {
                                        left++;
                                    }
                                    l++;
                                    r++;
                                }
                                if(notEqual == 0) /* We found a match, now we need to append the correct things to it */
                                {
                                    r = 0;
                                    if(strlen(function) > 0)
                                    {
                                        while(functionNames[p][r] != '/') /* Puts the struct name */
                                        { /* This if statement will only print the struct's good characters */
                                            if(((int)functionNames[p][r] - 65 >= 0 && (int)functionNames[p][r] - 65 <= 25) || ((int)functionNames[p][r] - 97 >= 0 && (int)functionNames[p][r] - 97 <= 25)) /* This checks for weird characters */
                                            {
                                                fputc(functionNames[p][r], writtenPtr);
                                            }
                                            r++;
                                        }
                                        /* Print the extra characters of the function name */
                                        r = right;
                                        while(functionNames[p][r] != '/')
                                        {
                                            fputc(functionNames[p][r], writtenPtr);
                                            r++;
                                            j++;
                                        }
                                        if(left != strlen(function)) /* This happens when there is paramters */
                                        {
                                            j--;
                                        }
                                    }
                                }
                                notEqual = 0;
                                r = 0;
                            }
                        }
                        free(classVar);
                        free(function);
                    }
                }
            }
            else if(inStruct == 1 || stillStruct == 1) /* They are inside a struct */
            {
                if(isFunc == 0) /* They are not inside a function */
                {
                    k = j;
                    while(storage[i][k] != '\n' && isUnique == 0) /* Scans through the struct finding where the functions are located */
                    {
                        if(storage[i][k] == '(' && isUnique == 0) /* A function's start so we store reference to it */
                        {
                            isFunc = 1;
                            isUnique = 1;
                            storedFuncs += 1;
                            if(storedFuncs == (amountFuncs - 1))
                            {
                                amountFuncs += 4;
                                savedFunctions = realloc(savedFunctions, sizeof(char*) * amountFuncs);
                                for(r = 0; r < amountFuncs; r++)
                                {
                                    savedFunctions[r] = realloc(savedFunctions, sizeof(char) * columns);
                                }
                            }
                        }
                        k += 1;
                    }
                    if(isUnique == 0) /* The line did not end up having a bracket on it (save the thing to the function names 2D) */
                    {
                        while(j < k)
                        {
                            fputc(storage[i][j], writtenPtr);
                            /* Start to save the name and type here */
                            j++;
                        }
                        fputc(storage[i][j], writtenPtr);
                    }
                    k = 0;
                }
                else if(isFunc == 1) /* They are inside a function right now */
                {
                    if(m <= storedFuncs) /* If they stored at least one function */
                    {
                        curlyBraces = 0; /* Reset it for future functions inside of structs */
                        while(storage[i][j] != '{')
                        {
                            savedFunctions[m][k] = storage[i][j];
                            k++;
                            j++;
                        }
                        curlyBraces += 1;
                        savedFunctions[m][k] = storage[i][j]; /* These next few are for the first { in the function */
                        j++;
                        k++;
                        do /* End of a function (do-while to check for the braces better) */
                        {
                            if(storage[i][j] == '{')
                            {
                                curlyBraces += 1;
                            }
                            else if(storage[i][j] == '}')
                            {
                                curlyBraces -= 1;
                            }
                            savedFunctions[m][k] = storage[i][j];
                            k++;
                            j++;
                        } while(curlyBraces > 0);
                        savedFunctions[m][k] = storage[i][j]; /* To get the closing brace */
                        j++;
                        m++;
                        k = 0;
                        isUnique = 0;
                    }
                    isFunc = 0;
                }
                if(storage[i][j] == '}' && storage[i][j + 1] == ';') /* They have reached the end of a struct */
                {
                    inStruct = 0;
                    stillStruct = 0;
                    for(k = 0; k < storedFuncs; k++) /* Goes through all the functions stored from this struct */
                    {
                        newName = createNewFunc(savedFunctions[k], structName);
                        fputs(newName, writtenPtr);
                        free(newName);
                    }
                    fputc(storage[i][j], writtenPtr); /* Prints the '}' character */
                }
            }
        }
        /* NEED TO FIND OUT WHY DOUBLE FREE ERROR */
        for(k = 0; k < m; k++)
        {
            fputc('\n', writtenPtr);
            stillStruct = 0; /* Has to reset for each function */
            thisFunc = -1;
            for(j = 0; j < strlen(savedFunctions[k]); j++)
            {
                inQuotes = quoteCheck(savedFunctions, k, j, inQuotes);

                if(savedFunctions[k][j] == 'c' && savedFunctions[k][j + 1] == 'l' && savedFunctions[k][j + 2] == 'a' && savedFunctions[k][j + 3] == 's' && savedFunctions[k][j + 4] == 's' && savedFunctions[k][j + 5] == ' ' && inQuotes == 0)
                {
                    fputs("struct", writtenPtr);
                    j += 5;
                    /* Here I need to add the constructor call */
                    while(savedFunctions[k][j] == ' ' || savedFunctions[k][j] == '\t' || savedFunctions[k][j] == '\n')
                    {
                        fputc(savedFunctions[k][j], writtenPtr); /* Up until the struct name */
                        j++;
                    }
                    tempName = calloc(50, sizeof(char));
                    tempVar = calloc(50, sizeof(char));
                    p = 0;
                    while(savedFunctions[k][j] != ' ' && savedFunctions[k][j] != '\t' && savedFunctions[k][j] != '\n')
                    {
                        /* The struct name */
                        tempName[p] = savedFunctions[k][j];
                        fputc(savedFunctions[k][j], writtenPtr);
                        p++;
                        j++;
                    }
                    tempName[p] = '\0';
                    while(savedFunctions[k][j] == ' ' || savedFunctions[k][j] == '\t' || savedFunctions[k][j] == '\n') /* Up to the variable name */
                    {
                        fputc(savedFunctions[k][j], writtenPtr);
                        j++;
                    }
                    p = 0;
                    while(savedFunctions[k][j] != ' ' && savedFunctions[k][j] != '\n' && savedFunctions[k][j] != '\t' && savedFunctions[k][j] != ';')
                    {
                        tempVar[p] = savedFunctions[k][j] ; /* Store the variable's name */
                        fputc(savedFunctions[k][j] , writtenPtr);
                        p++;
                        j++;
                    }
                    while(savedFunctions[k][j] != '\n') /* Until the newline is present */
                    {
                        fputc(savedFunctions[k][j], writtenPtr);
                        j++;
                    }
                    fputc(savedFunctions[k][j], writtenPtr);
                    constructor = createConstruct(tempName, tempVar); /* Create the constructor with the name and variable */
                    fputs(constructor, writtenPtr);
                    fputc('\n', writtenPtr);
                    for(p = 0; p <= savedItems; p++) /* Associate the variable name with the struct name for later comparisons */
                    {
                        notEqual = 0;
                        for(l = 0; l < strlen(tempName); l++)
                        {
                            if(tempName[l] != functionNames[p][l])
                            {
                                notEqual = 1;
                            }
                        }
                        if(notEqual == 0) /* We found the matching struct so store this with it */
                        {
                            strcat(functionNames[p], tempVar);
                            strcat(functionNames[p], "/");
                        }
                    }
                    free(constructor);
                    free(tempName);
                    free(tempVar);
                }
                if(stillStruct == 1 && thisFunc == -1) /* We need to save the name for later association */
                {
                    j--;
                    for(p = 0; p <= savedItems; p++) /* To give the function to the struct */
                    {
                        notEqual = 0;
                        for(l = 0; l < strlen(structName); l++)
                        {
                            if(structName[l] != functionNames[p][l])
                            {
                                notEqual = 1;
                            }
                        }
                        if(notEqual == 0) /* We found the matching struct so store this with it */
                        {
                            thisFunc = p;
                            while(savedFunctions[k][j] != '(')
                            {
                                functionNames[p][strlen(functionNames[p])] = savedFunctions[k][j];
                                fputc(savedFunctions[k][j], writtenPtr);
                                j++;
                            }
                        }
                    }
                    additions = functionAdditions(savedFunctions[k]);
                    fputs(additions, writtenPtr);
                    strcat(functionNames[thisFunc], additions); /* To get the parameter types */
                    strcat(functionNames[thisFunc], "/");
                    free(additions);
                }
                else if(stillStruct == 0) /* For the name */
                {
                    theReturn = returnType(savedFunctions[k]);
                    while(savedFunctions[k][j] == ' ') /* The blank spaces */
                    {
                        fputc(savedFunctions[k][j], writtenPtr);
                        j++;
                    }
                    while(savedFunctions[k][j] != ' ') /* The return type */
                    {
                        fputc(savedFunctions[k][j], writtenPtr);
                        j++;
                    }
                    while(savedFunctions[k][j] == ' ' || savedFunctions[k][j] == '*') /* Until the name appears */
                    {
                        fputc(savedFunctions[k][j], writtenPtr);
                        j++;
                    }
                    if(theReturn <= -1) /* It is a struct type */
                    {
                        while(savedFunctions[k][j] != ' ') /* The struct name */
                        {
                            fputc(savedFunctions[k][j], writtenPtr);
                            j++;
                        }
                        while(savedFunctions[k][j] == ' ' || savedFunctions[k][j] == '*') /* Until function name */
                        {
                            fputc(savedFunctions[k][j], writtenPtr);
                            j++;
                        }
                    }
                    fputs(structName, writtenPtr); /* Put struct name first */
                    stillStruct = 1; /* Now we put the additions themselves */
                }
                /* Add here the ability to check the . for stuff on both sides */
                if(savedFunctions[k][j] == '.' && (savedFunctions[k][j + 1] != 't' && savedFunctions[k][j + 2] != 'm' && savedFunctions[k][j + 3] != '_'))
                {
                    /* In here, we need to go left and right; newClass.fn(); (see the other spot) then replace */
                    fputc(savedFunctions[k][j], writtenPtr);
                    j++;
                    saved = j;
                    classVar = calloc(30, sizeof(char));
                    function = calloc(30, sizeof(char));
                    left = 0;
                    right = 0;
                    p = 0;

                    while(savedFunctions[k][saved + right] != ')' && savedFunctions[k][saved + right] != '=')
                    {
                        right++; /* The function name itself */
                    }
                    while(savedFunctions[k][saved - left] != ' ')
                    {
                        left++;
                    }
                    if(savedFunctions[k][saved + right] == ')') /* They found a function (variable does not matter) */
                    {
                        for(p = saved + 1; p < saved + right; p++)
                        {
                            function[p - (saved + 1)] = savedFunctions[k][p];
                        }
                    }
                    for(p = saved - left; p < saved; p++) /* The struct variable itself */
                    {
                        if(savedFunctions[k][p + 1] != '.')
                        {
                            classVar[p - (saved - left)] = savedFunctions[k][p + 1];
                        }
                    }
                    /* Now here, we compare the found function to the variable and see what to change to */
                    for(p = 0; p <= savedItems; p++)
                    {
                        notEqual = 0;
                        r = 0;
                        right = l; /* We are going right of the start */
                        for(l = 0; l < strlen(functionNames[p]); l++) /* Everything saved */
                        {
                            while(functionNames[p][l] != '/') /* Length of the var */
                            {
                                if(functionNames[p][l] != classVar[r])
                                {
                                    notEqual = 1;
                                }
                                l++;
                                r++;
                            }
                            if(notEqual == 0) /* We found a match, now we need to append the correct things to it */
                            {
                                r = 0;
                                if(strlen(function) > 0)
                                {
                                    while(functionNames[p][r] != '/') /* Puts the struct name */
                                    {
                                        fputc(functionNames[p][r], writtenPtr);
                                        r++;
                                    }
                                    /* Check if there are variables for possible overloading */
                                }
                            }
                            notEqual = 0;
                            right = l + 1;
                            r = 0;
                        }
                    }
                    free(classVar);
                    free(function);
                }
                if(stillStruct == 1 && thisFunc != -1) /* This happens becuase it was printing an extra one for no reason */
                {
                    fputc(savedFunctions[k][j], writtenPtr);
                }
            }
        }
        /* Remove functions from previous struct here */
        if(storedFuncs > 0)
        {
            fputc('\n', writtenPtr);
            constructor = constructBody(savedFunctions, structName, storedFuncs); /* This will print the body of the constructor */
            fputs(constructor, writtenPtr);
            free(constructor);
        }
        free(structName);
        for(j = 0; j < storedFuncs; j++)
        {
            strcpy(savedFunctions[j], "");
        }
    }

    /* Closing files and freeing memory */
    fclose(filePtr);
    fclose(writtenPtr);
    free(fileName);
    free(newFile);
    for(i = 0; i < rows; i++)
    {
        free(storage[i]);
        free(savedFunctions[i]);
    }
    for(i = 0; i < 20; i++)
    {
        free(functionNames[i]);
    }
    free(savedFunctions);
    free(storage);
    free(functionNames);
    free(numFuncs);
    return 0;
}
