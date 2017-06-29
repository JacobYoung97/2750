/*************************************************
Jacob Young 0932387
CIS*2750 Assignment 3
This assignment converts A2 to work with web pages
*************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "elements.h"

/*
assign
This should run the assignment programs from A2
IN: char * element - this is the assignment programs name; int index - the index of the element
OUT: int - the ending location of this attribute
*/
int assign(char * element, int index)
{
    int i = 0;
    int j = 0;
    int escape = 0;
    char * set = calloc(50, sizeof(char));
    char * command = calloc(250, sizeof(char));

    /* My current solution is to write to another php file and include that later; including php inside another php script */
    printf("<?php\n");

    for(i = index; element[i] != ')'; i++)
    {
        if(element[i] == 's' && element[i + 1] == 'e' && element[i + 2] == 't')
        {
            /* Need a check for if they want to nl2br some text */
            i += 3;
            j = 0;
            i = dataStart(element, i);
            set[j] = '$';
            j += 1;
            while(element[i] != ',')
            {
                set[j] = element[i];
                j += 1;
                i += 1;
            }
            i += 1;
            /* If n/ then you should do the nl2br for this variable */
            if(element[i] == 'n' && element[i + 1] == '/')
            {
                i += 2;
                set[j] = ' ';
                set[j + 1] = '=';
                set[j + 2] = ' ';
                j += 3;
                strcat(set, "nl2br($");
                j += strlen("nl2br($");
                while(element[i] != '\"')
                {
                    set[j] = element[i];
                    j += 1;
                    i += 1;
                }
                set[j] = ')';
            }
            else /* Standard $_POST with a webpage */
            {
                set[j] = ' ';
                set[j + 1] = '=';
                set[j + 2] = ' ';
                set[j + 3] = '$';
                set[j + 4] = '_';
                j += 5;
                strcat(set, "POST[");
                j += strlen("POST[");
                while(element[i] != '\"')
                {
                    set[j] = element[i];
                    j += 1;
                    i += 1;
                }
                set[j] = ']';
            }
            set[j + 1] = ';';
            set[j + 2] = '\0';
            printf("%s\n", set);
            free(set);
            set = calloc(30, sizeof(char));
        }
        else if(element[i] == 'e' && element[i + 1] == 'x' && element[i + 2] == 'e')
        {
            i += 3;
            j = 0;
            i = dataStart(element, i);

            command[j] = '\"'; /* Useful for if the $cmd variable contains the escapeshellarg() command */
            j += 1;

            while(element[i] != '\"')
            {
                if(element[i] == ' ' && element[i + 1] == 'e' && element[i + 2] == '/')
                {
                    i += 3;
                    escape = 1;
                    /* Need a check for escpaeshellarg()
                    Need to set a variable to check for this and print it out instead of the \"\" from earlier */
                    command[j] = ' ';
                    j += 1;
                    command[j] = '\"'; /* To get [ " . escapeshellarg($VARNAME) . " ]*/
                    j += 1;
                    command[j] = ' ';
                    j += 1;
                    command[j] = '.';
                    j += 1;
                    command[j] = ' ';
                    j += 1;
                    strcat(command, "escapeshellarg($");
                    j += strlen("escapeshellarg($");
                    while(element[i] != ' ')
                    {
                        command[j] = element[i];
                        i += 1;
                        j += 1;
                    }
                    command[j] = ')';
                    j += 1;
                    command[j] = ' ';
                    j += 1;
                    command[j] = '.';
                    j += 1;
                    command[j] = ' ';
                    j += 1;
                    command[j] = '\"';
                    j += 1;
                    command[j] = ' ';
                }
                else if(element[i] == ' ') /* a variable is included */
                {
                    command[j] = element[i];
                    /* Need to include the quotation marks again */
                    j += 1;
                    command[j] = '$';
                    j += 1;
                    i += 1;
                }
                command[j] = element[i];
                j += 1;
                i += 1;
                if(escape == 1)
                {
                    i -= 1;
                    escape = 0;
                }
            }
            printf("$cmd = %s\";\n", command); /* probably easier to remove the quotations and add them myself */
            printf("unset($output);\n");
            printf("exec($cmd, $output, $return);\n");
            printf("if($return) { echo \"Program crashed or failed\"; }\n");
            printf("else {\n");
            printf("foreach($output as $line) {\n");
            printf("echo \"$line\",\"\\n\";\n");
            printf("}\n");
            printf("}\n");
        }
    }

    printf("?>\n");
    free(command);
    free(set);
    return i;
}

/*
button
This function will read in a button that the user can select
IN: char * element - the text containing the button; int index - the starting index of the attributes
OUT: char * - the button's HTML text
*/
void button(char * element, int index)
{
    int i;
    int j = 0;
    char * name = calloc(50, sizeof(char));
    char * link = calloc(100, sizeof(char));
    char * output = calloc(600, sizeof(char));
    char * extra = calloc(300, sizeof(char));
    for(i = index; element[i] != ')'; i++)
    {
        /* Need to detect if they put some () in their text as well */
        if(element[i] == 'n' && element[i + 1] == 'a' && element[i + 2] == 'm' && element[i + 3] == 'e')
        {
            i += 4;
            j = 0;
            i = dataStart(element, i);
            while(element[i] != '\"')
            {
                name[j] = element[i];
                j += 1;
                i += 1;
            }
            j = 0;
        }
        else if(element[i] == 'l' && element[i + 1] == 'i' && element[i + 2] == 'n' && element[i + 3] == 'k')
        {
            i += 4;
            j = 0;
            i = dataStart(element, i);
            while(element[i] != '\"')
            {
                link[j] = element[i];
                j += 1;
                i += 1;
            }
            j = 0;
            if(link[strlen(link) - 4] != '.' && link[strlen(link) - 4] != 'h') /* If there is no .php suffix then this adds it */
                strcat(link, ".php");
        }
        else if(element[i] != ',' && element[i] != ' ')
        {
            j = strlen(extra);

            while(element[i] != ',' && element[i] != ')')
            {
                extra[j] = element[i];
                j += 1;
                i += 1;
            }
            i -= 1;
            extra[j] = ' ';
        }
    }

    if(strlen(extra) >= 1)
        extra[strlen(extra) - 1] = '\0';

    /* Check the link being blank cause it's assumed to be the same page */
    if(strlen(extra) > 1)
    {
        sprintf(output, "<form method=\"POST\" action=\"%s\" %s>\n", link, extra);
        printf("%s", output);
    }
    else
    {
        printf("<form method=\"POST\" action=\"%s\">\n", link);
    }
    printf("<input type=\"submit\" value=\"%s\">\n", name);
    printf("</form>\n");
    free(name);
    free(link);
    free(output);
    free(extra);
}

/*
exe
This will read in the executable and place it into the file to run on server
IN: char * element - this is the exe attribute; int index - the start of the index
*/
void exe(char * element, int index)
{
    int i;
    int j;
    char * fileName = calloc(50, sizeof(char)); /* Will be used to check where it is located and if it exists in certain directories */
    char * command = calloc(100, sizeof(char));
    char * execute = calloc(100, sizeof(char));
    char * further = calloc(100, sizeof(char));
    FILE * exe;
    for(i = index; element[i] != ')'; i++)
    {
        /* Will get the executable and then return the name */
        if(element[i] == 'e' && element[i + 1] == 'x' && element[i + 2] == 'e')
        {
            i += 3;
            j = 0;
            i = dataStart(element, i);
            while(element[i] != '\"' && element[i] != ' ') /* Check for command line arguments */
            {
                fileName[j] = element[i];
                j += 1;
                i += 1;
            }
            fileName[j] = '\0';
            strcpy(command, fileName);
            /* If the current char is a space, then we have some command line arguments */
            if(element[i] == ' ')
            {
                while(element[i] != '\"')
                {
                    command[j] = element[i];
                    j += 1;
                    i += 1;
                }
                command[j] = '\0';
            }
        }
    }
    exe = fopen(fileName, "r");
    if(exe == NULL) /* Check other directories */
    {
        sprintf(further, "./bin/%s", fileName);
        exe = fopen(further, "r");
        if(exe == NULL) /* System bin directory */
        {
            sprintf(further, "/bin/%s", fileName);
            exe = fopen(further, "r");
            if(exe == NULL)
            {
                printf("<B>COULD NOT FIND EXECUTABLE!</B>");
            }
            else
            {
                sprintf(further, "/bin/%s", command);
                exePrint(further);
                fclose(exe);
            }
        }
        else /* It is in ./bin */
        {
            sprintf(further, "./bin/%s", command);
            exePrint(further);
            fclose(exe);
        }
    }
    else /* It exists*/
    {
        sprintf(execute, "./%s", command);
        exePrint(execute);
        fclose(exe);
    }

    free(command);
    free(fileName);
    free(execute);
    free(further);
}

/*
exePrint
This will print out the php necessary to run the executable
IN: char * command - the command being run
*/
void exePrint(char * command)
{
    printf("<?php\n");
    printf("$cmd = \"%s\";\n", command);
    printf("exec($cmd, $output, $return);\n");
    printf("if($return) { echo \"Failed to execute\"; }\n");
    printf("else {\n");
    printf("foreach($output as $line) {\n");
    printf("echo \"$line\",\"\\n\";\n");
    printf("}\n");
    printf("}\n");
    printf("?>\n");
}

/*
heading
This will create a heading on the page
IN: char * element - this is the start of the attribute for heading; int index - the starting location
*/
void heading(char * element, int index)
{
    int i;
    int j = 0;
    int sizeFound = 0;
    int textFound = 0;
    int theSize;
    char * text = calloc(500, sizeof(char));
    char * output = calloc(510, sizeof(char));
    char * extra = calloc(300, sizeof(char));

    for(i = index; element[i] != ')'; i++)
    {
        if(element[i] == 's' && element[i + 1] == 'i' && element[i + 2] == 'z' && element[i + 3] == 'e')
        {
            /* size has been found (no longer the default of 3)*/
            i += 4;
            sizeFound = 1;
            i = dataStart(element, i);
            theSize = element[i] - '0'; /* Gets the integer equivalent (1 - 6)*/
        }
        else if(element[i] == 't' && element[i + 1] == 'e' && element[i + 2] == 'x' && element[i + 3] == 't')
        {
            /* text has been found (no longer the default of "HEADING") */
            textFound = 1;
            i += 4;
            i = dataStart(element, i);
            while(element[i] != '\"')
            {
                text[j] = element[i];
                j += 1;
                i += 1;
            }
        }
        else if(element[i] != ',' && element[i] != ' ') /* Any other tag would fall under this one */
        {
            j = strlen(extra);

            while(element[i] != ',' && element[i] != ')')
            {
                extra[j] = element[i];
                j += 1;
                i += 1;
            }
            i -= 1;
            extra[j] = ' ';
        }
    }

    if(strlen(extra) >= 1)
        extra[strlen(extra) - 1] = '\0';

    if(textFound == 0) /* If no specified text was found, then we need to use the default*/
    {
        free(text);
        text = calloc(8, sizeof(char));
        strcpy(text, "HEADING");
    }

    if(sizeFound == 0) /* Default size must be used */
    {
        if(strlen(extra) >= 1)
            sprintf(output, "<H3 %s>%s</H3>\n", extra, text);
        else
            sprintf(output, "<H3>%s</H3>\n", text);
    }
    else /* A size was specified */
    {
        if(strlen(extra) >= 1)
            sprintf(output, "<H%d %s>%s</H%d>\n", theSize, extra, text, theSize);
        else
            sprintf(output, "<H%d>%s</H%d>\n", theSize, text, theSize);
    }
    free(text);
    printf("%s", output);
    free(output);
    free(extra);
}

/*
input
This function creates the area where a user can enter one or more textboxes of input
IN: chear * element - the line contanining the data; int index - start of the attributes
*/
void input(char * element, int index)
{
    int i;
    int j = 0;
    int done = 0;
    int numFields = 0;
    char * process = calloc(50, sizeof(char));
    char * text = calloc(150, sizeof(char));
    char * name = calloc(50, sizeof(char));
    char * value = calloc(50, sizeof(char));
    char * extra = calloc(300, sizeof(char));
    char * output = calloc(510, sizeof(char));
    char ** inputFields = calloc(40, sizeof(char*));
    for(i = 0; i < 40; i++)
    {
        inputFields[i] = calloc(510, sizeof(char));
    }
    for(i = index; element[i] != ')'; i++)
    {
        if(element[i] == 'a' && element[i + 1] == 'c' && element[i + 2] == 't' && element[i + 3] == 'i' && element[i + 4] == 'o' && element[i + 5] == 'n')
        {
            j = 0;
            i += 6;
            i = dataStart(element, i);
            while(element[i] != '\"')
            {
                process[j] = element[i];
                j += 1;
                i += 1;
            }

            if(process[strlen(process) - 4] != '.' && process[strlen(process) - 4] != 'h') /* If there is no .php suffix then this adds it */
                strcat(process, ".php");

            sprintf(output, "<form action=\"%s\" method=\"post\"", process);
            free(process); /* We only needed it for this one */
        }
        else if(element[i] == 't' && element[i + 1] == 'e' && element[i + 2] == 'x' && element[i + 3] == 't')
        {
            i += 4;
            j = 0;
            /* text found */
            i = dataStart(element, i);
            while(element[i] != '\"')
            {
                text[j] = element[i];
                j += 1;
                i += 1;
            }
            text[j] = '\0';
        }
        else if(element[i] == 'n' && element[i + 1] == 'a' && element[i + 2] == 'm' && element[i + 3] == 'e')
        {
            i += 4;
            j = 0;
            /* name found */
            i = dataStart(element, i);
            while(element[i] != '\"')
            {
                name[j] = element[i];
                j += 1;
                i += 1;
            }
            name[j] = '\0';
        }
        else if(element[i] == 'v' && element[i + 1] == 'a' && element[i + 2] == 'l' && element[i + 3] == 'u' && element[i + 4] == 'e')
        {
            i += 5;
            j = 0;
            /* value found (if it is empty (i.e. "") then it should still send a blank NULL terminator anyways*/
            i = dataStart(element, i);
            if(element[i] == ')' || element[i] == ',') /* Two quotation marks next to each other like "" */
            {
                value[j] = '\0';
                i -= 2; /* If it is a ) then we use this to account for that in the for loop */
            }
            else
            {
                while(element[i] != '\"')
                {
                    value[j] = element[i];
                    j += 1;
                    i += 1;
                }
                value[j] = '\0';
            }
            done = 1;
        }
        else if(done == 1)
        {
            sprintf(inputFields[numFields], "%s: <input type=\"text\" name=\"%s\" value=\"%s\"><BR>\n", text, name, value);
            /* free the stuff and then realloc it */
            free(text);
            free(name);
            free(value);
            text = calloc(150, sizeof(char));
            name = calloc(50, sizeof(char));
            value = calloc(50, sizeof(char));
            done = 0;
            numFields += 1;
        }
        else if(element[i] != ',' && element[i] != ' ') /* Extra content */
        {
            j = strlen(extra);

            while(element[i] != ',' && element[i] != ')')
            {
                extra[j] = element[i];
                j += 1;
                i += 1;
            }
            i -= 1;
            extra[j] = ' ';
        }
    }

    /* The ending is like <input type="submit" value = "Submit"> </form>
	
    Print the opening with the unique items here */
    if(strlen(extra) >= 1)
    {
        extra[strlen(extra) - 1] = '\0';
        strcat(output, " ");
    }
    strcat(output, extra);
    strcat(output, "><BR>\n");
    printf("%s", output);
    for(i = 0; i < numFields; i++)
    {
        printf("%s", inputFields[i]);
    }
    printf("<input type=\"submit\" value=\"Submit\"><BR>\n");
    free(text);
    free(name);
    free(value);
    printf("</form>\n");
    free(output);
    for(i = 0; i < 40; i++)
    {
        free(inputFields[i]);
    }
    free(inputFields);
    free(extra);
}

/*
line
This function is for if any other input was given to the horizontal tag
IN: char * element - the line in question; int index - where it starts
*/
void line(char * element, int index)
{
    int i;
    int j = 0;
    char * lineArg = calloc(512, sizeof(char));
    char * output = calloc(520, sizeof(char));

    for(i = index; element[i] != ')'; i++)
    {
        while(element[i] != ',' && element[i] != ')')
        {
            lineArg[j] = element[i];
            j += 1;
            i += 1;
        }
        lineArg[j] = ' ';
        j += 1;
        if(element[i] == ')')
            i -= 1;
    }
    lineArg[strlen(lineArg) - 1] = '\0';
    sprintf(output, "<HR %s>\n", lineArg);
    printf("%s", output);
    free(lineArg);
    free(output);
}

/*
link
This function will create a link on a web page with default name "link"
IN: char * element - the actual text containing link info; int index - the start of the elements
*/
void link(char * element, int index)
{
    int i;
    int j;
    int textFound = 0;
    char * formattedOutput = calloc(1000, sizeof(char));
    char * link = calloc(50, sizeof(char));
    char * textName = calloc(50, sizeof(char));
    char * extra = calloc(350, sizeof(char));
    for(i = index; element[i] != ')'; i++)
    {
        if(element[i] == 't' && element[i + 1] == 'e' && element[i + 2] == 'x' && element[i + 3] == 't')
        {
            i += 4;
            j = 0;
            textFound = 1;
            i = dataStart(element, i);
            while(element[i] != '\"')
            {
                textName[j] = element[i];
                j += 1;
                i += 1;
            }
            textName[j] = '\0';
        }
        else if(element[i] == 'l' && element[i + 1] == 'i' && element[i + 2] == 'n' && element[i + 3] == 'k')
        {
            i += 4;
            j = 0;
            i = dataStart(element, i);
            while(element[i] != '\"')
            {
                link[j] = element[i];
                j += 1;
                i += 1;
            }
            link[j] = '\0';
        }
        else if(element[i] != ',' && element[i] != ' ')
        {
            j = strlen(extra);

            while(element[i] != ',' && element[i] != ')')
            {
                extra[j] = element[i];
                j += 1;
                i += 1;
            }
            i -= 1;
            extra[j] = ' ';
        }
    }
    if(strlen(extra) >= 1)
        extra[strlen(extra) - 1] = '\0';
    if(textFound == 1)
    {
        if(strlen(extra) > 0)
            sprintf(formattedOutput, "<a href=\"%s\" %s>%s</a>\n", link, extra, textName);
        else
            sprintf(formattedOutput, "<a href=\"%s\">%s</a>\n", link, textName);
    }
    else /* This will use the default name of 'link' */
    {
        if(strlen(extra) > 0)
            sprintf(formattedOutput, "<a href=\"%s\" %s>link</a>\n", link, extra);
        else
            sprintf(formattedOutput, "<a href=\"%s\">link</a>\n", link);
    }
    printf("%s", formattedOutput);
    free(formattedOutput);
    free(extra);
    free(link);
    free(textName);
}

/*
multiple
This custom tag will create a form with multiple input values associated with it
IN char * element - the attribute itself; int index - the starting location of the attribute
*/
void multiple(char * element, int index)
{
    int i;
    int j;
    int inputType = 0;
    int radioNum = 0;
    char text[30];
    char * action = calloc(30, sizeof(char));
    char * rad = calloc(30, sizeof(char));
    char * hidden = calloc(30, sizeof(char));
    char * name = calloc(30, sizeof(char));
    char submit[30];
    char rows[2];
    char cols[2];

    for(i = 0; i < 30; i++)
    {
        submit[i] = '\0';
        text[i] = '\0';
    }

    for(i = index; element[i] != ')'; i++)
    {
        if(element[i] == 'a' && element[i + 1] == 'c' && element[i + 2] == 't' && element[i + 3] == 'i' && element[i + 4] == 'o' && element[i + 5] == 'n')
        {
            i += 6;
            j = 0;
            i = dataStart(element, i);

            while(element[i] != '\"')
            {
                action[j] = element[i];
                i += 1;
                j += 1;
            }
            action[j] = '\0';
            printf("<form method=\"POST\" action=\"%s\">\n", action);
            free(action);
        }
		/* A radio button option on the multiple input form */
        else if(element[i] == 'r' && element[i + 1] == 'a' && element[i + 2] == 'd' && element[i + 3] == 'i' && element[i + 4] == 'o')
        {
            i += 5;
            j = 0;
            i = dataStart(element, i);
            inputType = 1;
            radioNum += 1;

            while(element[i] != '\"')
            {
                rad[j] = element[i];
                i += 1;
                j += 1;
            }
            rad[j] = '\0';
        }
        else if(element[i] == 't' && element[i + 1] == 'e' && element[i + 2] == 'x' && element[i + 3] == 't') /* One line of text */
        {
            i += 4;
            j = 0;
            i = dataStart(element, i);
            inputType = 0;

            while(element[i] != '\"')
            {
                text[j] = element[i];
                j += 1;
                i += 1;
            }
            text[j] = '\0';
        }
		/* Any output that is hidden from normal view but still needs to be sent to another page */
        else if(element[i] == 'h' && element[i + 1] == 'i' && element[i + 2] == 'd' && element[i + 3] == 'd' && element[i + 4] == 'e' && element[i + 5] == 'n')
        {
            i += 6;
            j = 0;
            i = dataStart(element, i);
            inputType = 2;
            hidden[j] = '$';
            j += 1;

            while(element[i] != '\"')
            {
                hidden[j] = element[i];
                j += 1;
                i += 1;
            }
            hidden[j] = '\0';
        }
        else if(element[i] == 'a' && element[i + 1] == 'r' && element[i + 2] == 'e' && element[i + 3] == 'a') /* Text area */
        {
            i += 4;
            j = 0;
            i = dataStart(element, i);

            while(element[i] != 'X')
            {
                /* Rows */
                rows[j] = element[i];
                j += 1;
                i += 1;
            }
            rows[j] = '\0';
            j = 0;
            i += 1;
            while(element[i] != '\"')
            {
                /* Columns */
                cols[j] = element[i];
                j += 1;
                i += 1;
            }
            cols[j] = '\0';
            printf("<textarea cols=\"%s\" rows=\"%s\" name=\"area\"></textarea><BR>\n", cols, rows);
        }
        else if(element[i] == 'n' && element[i + 1] == 'a' && element[i + 2] == 'm' && element[i + 3] == 'e')
        {
            i += 4;
            j = 0;
            i = dataStart(element, i);

            while(element[i] != '\"')
            {
                name[j] = element[i];
                j += 1;
                i += 1;
            }
            name[j] = '\0';

            if(inputType == 0) /* Text for input */
            {
                printf("%s <input type=\"text\" name=\"%s\"><BR>\n", text, name);
                for(j = 0; j < 30; j++)
                {
                    text[j] = '\0';
                }
            }
            else if(inputType == 1) /* Radio for input */
            {
                if(radioNum == 1)
                {
                    printf("<input type=\"radio\" name=\"%s\" value=\"%s\" checked> %s<BR>\n", name, rad, rad);
                }
                else
                {
                    printf("<input type=\"radio\" name=\"%s\" value=\"%s\"> %s<BR>\n", name, rad, rad);
                }
                free(rad);
                rad = calloc(30, sizeof(char));
            }
            else if(inputType == 2) /* Hidden for input */
            {
                printf("<input type=\"hidden\" name=\"%s\" value=\"<?php echo %s; ?>\">\n", name, hidden);
                free(hidden);
                hidden = calloc(30, sizeof(char));
            }
            free(name);
            name = calloc(30, sizeof(char));
        }
        else if(element[i] == 's' && element[i + 1] == 'u' && element[i + 2] == 'b' && element[i + 3] == 'm' && element[i + 4] == 'i' && element[i + 5] == 't')
        {
            i += 6;
            j = 0;
            i = dataStart(element, i);

            while(element[i] != '\"')
            {
                submit[j] = element[i];
                i += 1;
                j += 1;
            }
            submit[j] = '\0';
            printf("<input type=\"submit\" value=\"%s\">\n", submit);
        }
    }

    free(name);
    free(rad);
    free(hidden);
    printf("</form>\n");
}

/*
picture
This will load in a picture to the web page
IN: char * element - the tag for the picture; int index - the start of the attributes for the tag
*/
void picture(char * element, int index)
{
    int i;
    int j;
    int sizeFound = 0;
    char * width = calloc(5, sizeof(char)); /* An image is at max a four digit number width or length wise */
    char * height = calloc(5, sizeof(char));
    char * name = calloc(50, sizeof(char));
    char * output = calloc(509, sizeof(char));
    char * extra = calloc(350, sizeof(char));
    for(i = index; element[i] != ')'; i++)
    {
        if(element[i] == 's' && element[i + 1] == 'i' && element[i + 2] == 'z' && element[i + 3] == 'e')
        {
            sizeFound = 1;
            i += 4;
            j = 0;
            i = dataStart(element, i);
            /* width first then height */
            while(element[i] != 'x') /* Since width is first, it ends at an 'x' */
            {
                width[j] = element[i];
                j += 1;
                i += 1;
            }
            width[j] = '\0';
            j = 0;
            i += 1;
            while(element[i] != ',' && element[i] != ')') /* Depending on order of size to image, it might either end at ',' or '"' */
            {
                height[j] = element[i];
                j += 1;
                i += 1;
            }
            height[j] = '\0';
            if(element[i] == ')')
                i -= 1;
        }
        else if(element[i] == 'i' && element[i + 1] == 'm' && element[i + 2] == 'a' && element[i + 3] == 'g' && element[i + 4] == 'e')
        {
            i += 5;
            j = 0;
            i = dataStart(element, i);
            while(element[i] != '\"')
            {
                name[j] = element[i];
                j += 1;
                i += 1;
            }
            name[j] = '\0';
        }
        else if(element[i] != ',' && element[i] != ' ')
        {
            j = strlen(extra);

            while(element[i] != ',' && element[i] != ')')
            {
                extra[j] = element[i];
                j += 1;
                i += 1;
            }
            i -= 1;
            extra[j] = ' ';
        }
    }

    if(strlen(extra) >= 1)
        extra[strlen(extra) - 1] = '\0';

    if(sizeFound == 1)
    {
        /* The size here not just 100x100 */
        if(strlen(extra) > 0)
            sprintf(output, "<img src=\"%s\" alt=\"image\" height=\"%s\" width=\"%s\" %s>\n", name, height, width, extra);
        else
            sprintf(output, "<img src=\"%s\" alt=\"image\" height=\"%s\" width=\"%s\">\n", name, height, width);
    }
    else
    {
        /* The default size */
        if(strlen(extra) > 0)
            sprintf(output, "<img src=\"%s\" alt=\"image\" height=\"100\" width=\"100\" %s>\n", name, extra);
        else
            sprintf(output, "<img src=\"%s\" alt=\"image\" height=\"100\" width=\"100\">\n", name);
    }

    printf("%s", output);
    free(output);
    free(width);
    free(height);
    free(name);
    free(extra);
}

/*
radio
This function will create a list of options on the webpage
IN: char * element - the tag's attributes for creation; int index - the start of the tag's attributes
*/
void radio(char * element, int index)
{
    int i;
    int j;
    int numOptions = 0;
    char * output = calloc(510, sizeof(char));
    char * action = calloc(100, sizeof(char));
    char * value = calloc(50, sizeof(char));
    char * name = calloc(50, sizeof(char));
    char * extra = calloc(300, sizeof(char));
    char ** options = calloc(50, sizeof(char*));
    for(i = 0; i < 50; i++)
    {
        options[i] = calloc(500, sizeof(char));
    }
    for(i = index; element[i] != ')'; i++) /* FINISH FOR THE PARSER AND THEN IT'S ONLY THE TEXT THAT DOESN"T APPEAR IN SPEC */
    {
        /* The values have to be after action and name (name needs to be known) */
        if(element[i] == 'a' && element[i + 1] == 'c' && element[i + 2] == 't' && element[i + 3] == 'i' && element[i + 4] == 'o' && element[i + 5] == 'n')
        {
            i += 6;
            j = 0;
            i = dataStart(element, i);
            while(element[i] != '\"')
            {
                action[j] = element[i];
                j += 1;
                i += 1;
            }
            if(action[strlen(action) - 4] != '.' && action[strlen(action) - 4] != 'h') /* If there is no .php suffix then this adds it */
                strcat(action, ".php");
            /* Can print this now OR WAIT TO SEE IF THERE IS ANY CRAP NOT SPECIFIED (DON"T INCLUDE CLOSING TAG THEN AND STRCAT AND AT THE END, A > */
            sprintf(output, "<form action=\"%s\" method=\"POST\"", action);
        }
        else if(element[i] == 'n' && element[i + 1] == 'a' && element[i + 2] == 'm' && element[i + 3] == 'e')
        {
            i += 4;
            j = 0;
            i = dataStart(element, i);
            while(element[i] != '\"')
            {
                name[j] = element[i];
                j += 1;
                i += 1;
            }
            name[j] = '\0';
            /* Store until value is found because we need this for that or a radio button wouldn't work */
        }
        else if(element[i] == 'v' && element[i + 1] == 'a' && element[i + 2] == 'l' && element[i + 3] == 'u' && element[i + 4] == 'e')
        {
            i += 5;
            j = 0;
            i = dataStart(element, i);
            while(element[i] != '\"')
            {
                value[j] = element[i];
                j += 1;
                i += 1;
            }
            value[j] = '\0';
            if(numOptions == 0)
            {
                sprintf(options[numOptions], "<input type=\"radio\" name=\"%s\" value=\"%s\" checked>%s<BR>\n", name, value, value);
            }
            else
            {
                sprintf(options[numOptions], "<input type=\"radio\" name=\"%s\" value=\"%s\">%s<BR>\n", name, value, value);
            }
            numOptions += 1;
            free(value);
            value = calloc(50, sizeof(char));
        }
        else if(element[i] != ',' && element[i] != ' ')
        {
            j = strlen(extra);

            while(element[i] != ',' && element[i] != ')')
            {
                extra[j] = element[i];
                j += 1;
                i += 1;
            }
            i -= 1;
            extra[j] = ' ';
        }
    }

    if(strlen(extra) >= 1)
    {
        extra[strlen(extra) - 1] = '\0';
        strcat(output, " ");
    }
    strcat(output, extra);
    strcat(output, "><BR>\n");
    printf("%s", output);
    free(output);
    for(i = 0; i < numOptions; i++)
    {
        printf("%s", options[i]);
    }
    printf("</form>\n"); /* This will always be here */
    free(action);
    free(name);
    free(value);
    free(extra);
    for(i = 0; i < 50; i++)
    {
        free(options[i]);
    }
    free(options);
}

/*
text
This function will display the text in either a file or entered in the .wpml file
IN: char * element - the tag itself; int index - the start of it's attributes
*/
void text(char * element, int index)
{
    int i;
    int j;
    FILE * textFile;
    int fileFound = 0;
    int textFound = 0;
    char * output = calloc(502, sizeof(char)); /* A line is at max 509 chars and if first, there are already 10 chars to it*/
    char * fileName = calloc(50, sizeof(char));
    char * open = calloc(300, sizeof(char));
    char * extra = calloc(280, sizeof(char));
    char fileLine[512];
    for(i = index; element[i] != ')'; i++)
    {
        /* It is a file that they want the text from */
		if(element[i] == 'f' && element[i + 1] == 'i' && element[i + 2] == 'l' && element[i + 3] == 'e')
        {
            fileFound = 1;
            i += 4;
            j = 0;
            i = dataStart(element, i);

            while(element[i] != '\"')
            {
                fileName[j] = element[i];
                i += 1;
                j += 1;
            }
            fileName[j] = '\0';
            textFile = fopen(fileName, "r");
            if(textFile == NULL)
            {
                printf("<B>FILE WAS NOT FOUND!</B>");
            }
            else
            {
                while(fgets(fileLine, 509, textFile) != NULL) /* Will go through the textfile and print */
                {
                    /* Should probably print a <BR> if one does not exist at the end */
                    printf("%s", fileLine);
                }
                fclose(textFile); /* We are done with the textfile */
            }
        }
		/* The text was written in the custom tag in the .wpml file */
        else if(element[i] == 't' && element[i + 1] == 'e' && element[i + 2] == 'x' && element[i + 3] == 't')
        {
            textFound = 1;
            i += 4;
            j = 0;
            i = dataStart(element, i);

            while(element[i] != '\"')
            {
                output[j] = element[i];
                j += 1;
                i += 1;
            }
            output[j] = '\0';
        }
        else if(element[i] != ',' && element[i] != ' ') /* */
        {
            j = strlen(extra);

            while(element[i] != ',' && element[i] != ')')
            {
                extra[j] = element[i];
                j += 1;
                i += 1;
            }
            i -= 1;
            extra[j] = ' ';
        }
    }

    if(strlen(extra) >= 1)
        extra[strlen(extra) - 1] = '\0';

    if(strlen(extra) > 0)
        sprintf(open, "<p %s>\n", extra);
    else
        sprintf(open, "<p>\n");

    if(fileFound == 0)
        printf("%s", open);

    if(fileFound == 0 && textFound == 0)
    {
        /* There were no arguments specified so it just becomes default text */
        printf("Default text<BR>");
    }
    else if(textFound == 1)
    {
        printf("%s\n", output);
    }
    if(fileFound == 0)
        printf("</p>\n");
    free(output);
    free(fileName);
    free(open);
    free(extra);
}

/*
lastBracket
This is used to find the index of the last bracket to skip ahead in reading a line
IN: char * line - the fileLine itself; int index - the starting index of the stuff in the attribute
*/
int lastBracket(char * line, int index)
{
    int i;
    int end = 0;

    for(i = index; i < strlen(line); i++)
    {
        if((line[i] == ')' && line[i + 1] == '\n') || (line[i] == ')' && line[i + 1] == '.' && line[i + 3] == '(') || (line[i] == ')' && line[i + 1] == ')') || (line[i] == ')' && line[i + 1] == ','))
        {
            end = i; /* In main program, we will then increment one more to be after the last ) */
            i = strlen(line) + 1; /* To ensure we leave this loop now */
        }
    }

    return end;
}

/*
dataStart
This function will be used to skip forward to where the actual data of the tags starts
IN: char * element - the overall attribute used to move forward; int index - the start of where we need to skip through (after name)
*/
int dataStart(char * element, int index)
{
    int i = index;
    while(element[i] == ' ' || element[i] == '=' || element[i] == '\"')
    {
        i += 1; /* Goes through until we see some data; i.e. size=3 will skip the = and start at 3 */
    }
    return i;
}
