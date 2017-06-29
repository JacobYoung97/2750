/***************************************************************
Jacob Young 0932387
CIS*2750 Assignment Two
This program will add/remove the author to the specified streams
****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stream.h"

int main(int argc, char * argv[])
{
    /* ./addauthor [choice] [streams] [user] */
    if(argc < 4)
    {
        printf("Please put a username and some streams to either add or remove yourself!\n");
        return 0; /* They need to see this message */
    }

    if(strlen(argv[2]) > 202)
    {
        printf("Too many streams added!\n");
        return 0; /* I want the user to see this message */
    }

    char * username = calloc(300, sizeof(char));
    char * streamList = calloc(202, sizeof(char));
    int i = 0;

    /* This will be when it is given the streams as a command line argument */
    strcpy(streamList, argv[2]);

    if(strcmp(argv[1], "remove") == 0) /* Remove from the stream's list */
    {
        for(i = 3; i < argc; i++)
        {
            strcat(username, argv[i]);
            if(i != argc - 1) /* The last argument does not need a space with it */
            {
                strcat(username, " ");
            }
        }
        removeUser(username, streamList);
        free(username);
        free(streamList);
        return 0;
    }
    for(i = 3; i < argc; i++)
    {
        strcat(username, argv[i]);
        if(i != argc - 1)
        {
            strcat(username, " ");
        }
    }
    addUser(username, streamList); /* Added to the stream lists */
    free(username);
    free(streamList);
    return 0;
}
