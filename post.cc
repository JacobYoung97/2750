/****************************************
Jacob Young 0932387
CIS*2750 Assignment Two
This file will write a post to a stream
****************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "stream.h"

/* This class will get the user's info and then */
class PostEntry {
    char * readInput(char * argv) {
        /* Read the post entry from the user (easier now than in A2) */
        char * entry = calloc(1100, sizeof(char));

        /* Should already be in the form username\stream\post */
        strcat(entry, argv);
        return entry;
    }
    void formatEntry(char * input, struct userPost * thePost) {
        /* Struct ready for the updateStream() function */
        thePost->username = calloc(30, sizeof(char));
        thePost->streamname = calloc(30, sizeof(char));
        thePost->date = calloc(40, sizeof(char));
        thePost->text = calloc(800, sizeof(char));
        char * tokedLine;
        int i = 0;

        tokedLine = strtok(input, "|");
        while(tokedLine != NULL)
        {
            if(i == 0) /* The username */
            {
                strcpy(thePost->username, tokedLine);
                i += 1;
            }
            else if(i == 1) /* The stream name */
            {
                strcpy(thePost->streamname, tokedLine);
                i += 1;
            }
            else if(i == 2) /* The text they entered */
            {
                strcpy(thePost->text, tokedLine);
                i += 1;
            }
            else if(i == 3) /* The date string */
            {
                strcpy(thePost->date, tokedLine);
            }
            tokedLine = strtok(NULL, "|");
        }
    }
    char * getTimeDate() {
        /* Gets the time and date and formats it as a string */
        time_t curTime = time(NULL);
        struct tm timeStruct = *localtime(&curTime);
        char * timeString = calloc(28, sizeof(char));
        char * mons = calloc(4, sizeof(char));
        char * days = calloc(4, sizeof(char));
        char * hours = calloc(4, sizeof(char));
        char * mins = calloc(4, sizeof(char));
        char * secs = calloc(4, sizeof(char));
        int year = timeStruct.tm_year + 1900;
        int month = timeStruct.tm_mon + 1;
        int day = timeStruct.tm_mday;
        int hour = timeStruct.tm_hour;
        int min = timeStruct.tm_min; /* SQL will like YYYY-MM-DD HH:MM:SS (ex. "2017-03-31 20:50:40") */
        int sec = timeStruct.tm_sec;

        sprintf(mons, "%.2d", month);
        sprintf(days, "%.2d", day);
        sprintf(hours, "%.2d", hour);
        sprintf(mins, "%.2d", min);
        sprintf(secs, "%.2d", sec);

        sprintf(timeString, "%d-%s-%s %s:%s:%s", year, mons, days, hours, mins, secs);
        free(mons);
        free(days);
        free(hours);
        free(mins);
        free(secs);
        return timeString;
    }
    void submitPost(struct userPost * st) {
        /* This will submit the post to the file specified by the user */
        updateStream(st);
    }
};

int main(int argc, char * argv[])
{
    if(argc < 4 || strlen(argv[2]) == 0 || strcmp(argv[1], "all") == 0)
    {
        if(strcmp(argv[1], "all") == 0)
        {
            printf("Can't post to all!\n");
        }
        else
            printf("Post to the last viewed stream here!\n");
        exit(0);
    }
    /* ./post [stream] [text] [username] */
    class PostEntry thePost;
    struct userPost sentText; /* Sent to file */
    char * enteredInfo; /* Not the date and time */
    char * postTime; /* The date and time */
    char * username = calloc(1100, sizeof(char));
    int i = 0;

    /* Multiworded usernames from command line */
    for(i = 3; i < argc; i++)
    {
        strcat(username, argv[i]);

        if(i != argc - 1)
        {
            strcat(username, " ");
        }
    }
    strcat(username, "|");
    strcat(username, argv[1]);
    strcat(username, "|");
    strcat(username, argv[2]);
    strcat(username, "\n");

    enteredInfo = thePost.readInput(username); /* Send the name of the user entered on command line */
    if(enteredInfo != NULL)
    {
        postTime = thePost.getTimeDate(); /* The time and date from time.h */
        strcat(enteredInfo, "|");
        strcat(enteredInfo, postTime);
        thePost.formatEntry(enteredInfo, &sentText); /* Formats the entry */
        thePost.submitPost(&sentText); /* Submits the post */
        free(sentText.streamname);
        free(sentText.username);
        free(sentText.date);
        free(sentText.text);
        free(postTime);
        printf("Posted to %s successfully!\n", argv[1]);
    }

    /* Free all memory */
    free(enteredInfo);
    free(username);

    return 0;
}
