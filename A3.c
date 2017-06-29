/*************************************************
Jacob Young 0932387
CIS*2750 Assignment 3
This assignment converts A2 to work with webpages
*************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "elements.h"

int main(int argc, char * argv[])
{
    if(argc < 2)
    {
        printf("Need a webpage markup file!\n");
        return 0;
    }

    FILE * markup = fopen(argv[1], "r");
    char fileLine[512]; /* A fileline is at max 509 lines */
    int lineLength = 0;
    int i = 0;
    int inQuotes = 0; /* To detect if the user slyly put an interface tag in their text PROBABLY NOT NECESSARY */

    printf("<!DOCTYPE html>\n");
    printf("<HTML>\n");
    printf("<BODY>\n");

    while(fgets(fileLine, 510, markup) != NULL) /* Gets all of the lines of the file and processes as we go along */
    {
        lineLength = strlen(fileLine);
        for(i = 0; i < lineLength; i++) /* Checks the whole line for tags */
        {
            if(fileLine[i] == '\"' && (i > 0 && fileLine[i - 1] != '\\') && inQuotes == 0)
            {
                inQuotes = 1; /* Basically, if we find a quotation mark, this will say so */
            }
            else if(fileLine[i] == '\"' && (i > 0 && fileLine[i - 1] != '\\') && inQuotes == 1)
            {
                inQuotes = 0; /* Basically, if we find the end quotation mark, this will say so */
            }
            else if(inQuotes == 0) /* This will check the interface tag when we are not in quotes*/
            {
                if(fileLine[i] == '.' && fileLine[i + 1] == 'd' && fileLine[i + 2] == '(') /* Easiest argument, horizontal line */
                {
                    if(fileLine[i + 3] == ')') /* Nothing extra is specified */
                    {
                        printf("<HR>\n");
                        i += 3;
                    }
                    else /* They add extra stuff to the attribute */
                    {
                        line(fileLine, i + 3);
                        i = lastBracket(fileLine, i + 2);
                    }
                }
                else if(fileLine[i] == '.' && fileLine[i + 1] == 'h' && fileLine[i + 2] == '(') /* This is the heading argument */
                {
                    heading(fileLine, i + 3);
                    i = lastBracket(fileLine, i + 2);
                }
                else if(fileLine[i] == '.' && fileLine[i + 1] == 'b' && fileLine[i + 2] == '(') /* Button argument */
                {
                    button(fileLine, i + 3);
                    i = lastBracket(fileLine, i + 2);
                }
                else if(fileLine[i] == '.' && fileLine[i + 1] == 'e' && fileLine[i + 2] == '(') /* Executable */
                {
                    exe(fileLine, i + 3);
                    i = lastBracket(fileLine, i + 2);
                }
                else if(fileLine[i] == '.' && fileLine[i + 1] == 'i' && fileLine[i + 2] == '(') /* Input */
                {
                    input(fileLine, i + 3);
                    i = lastBracket(fileLine, i + 2);
                }
                else if(fileLine[i] == '.' && fileLine[i + 1] == 'l' && fileLine[i + 2] == '(') /* Link */
                {
                    link(fileLine, i + 3);
                    i = lastBracket(fileLine, i +2);
                }
                else if(fileLine[i] == '.' && fileLine[i + 1] == 'p' && fileLine[i + 2] == '(') /* Picture */
                {
                    picture(fileLine, i + 3);
                    i = lastBracket(fileLine, i + 2);
                }
                else if(fileLine[i] == '.' && fileLine[i + 1] == 'r' && fileLine[i + 2] == '(') /* Radio */
                {
                    radio(fileLine, i + 3);
                    i = lastBracket(fileLine, i + 2);
                }
                else if(fileLine[i] == '.' && fileLine[i + 1] == 't' && fileLine[i + 2] == '(') /* Text */
                {
                    text(fileLine, i + 3);
                    i = lastBracket(fileLine, i + 2);
                }
                else if(fileLine[i] == '.' && fileLine[i + 1] == 'm' && fileLine[i + 2] == '(') /* Mulit-form */
                {
                    multiple(fileLine, i + 3);
                    i = lastBracket(fileLine, i + 2);
                }
                else if(fileLine[i] == '.' && fileLine[i + 1] == 'a' && fileLine[i + 2] == '(') /* Applications from before */
                {
                    i = assign(fileLine, i + 3);
                }
            }
        }
    }

    printf("</BODY>\n");
    printf("</HTML>\n");

    fclose(markup);
    return 0;
}
