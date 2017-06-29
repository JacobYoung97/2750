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
struct PostEntry {
   char *  (*PostEntryreadInputc)(char *);
  void  (*PostEntryformatEntryc)(char *, struct userPost *);
  char *  (*PostEntrygetTimeDate)();
  void  (*PostEntrysubmitPost)();
};
   char * PostEntryreadInputc(char * argv) {
        /* Read the post entry from the user (easier now than in A2) */
        char * entry = calloc(1100, sizeof(char));

        /* Should already be in the form username\stream\post */
        strcat(entry, argv);
        return entry;
    }

  void PostEntryformatEntryc(char * input, struct userPost * thePost) {
        /* Struct ready for the updateStream() function */
        thePost->username = calloc(30, sizeof(char));
        thePost->streamname = calloc(30, sizeof(char));
        thePost->date = calloc(40, sizeof(char));
        thePost->text = calloc(500, sizeof(char));
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

  char * PostEntrygetTimeDate() {
        /* Gets the time and date and formats it as a string */
        time_t curTime = time(NULL);
        struct tm timeStruct = *localtime(&curTime);
        char * timeString = calloc(28, sizeof(char));
        int year = timeStruct.tm_year + 1900;
        char theYear[6];
        char stringDay[4];
        char theHour[4];
        char theMin[4];
        sprintf(theYear, "%d ", year);
        int month = timeStruct.tm_mon + 1;
        int day = timeStruct.tm_mday;
        sprintf(stringDay, "%d, ", day);
        int hour = timeStruct.tm_hour;
        int min = timeStruct.tm_min;

        switch(month) /* This is a switch for the month (only Jan-Apr for now) */
        {
            case 1:
                strcat(timeString, "Jan. ");
                break;

            case 2:
                strcat(timeString, "Feb. ");
                break;

            case 3:
                strcat(timeString, "Mar. ");
                break;

            case 4:
                strcat(timeString, "Apr. ");
                break;
        }
        strcat(timeString, stringDay); /* The day they posted */
        strcat(timeString, theYear); /* The year it was posted */

        if(hour >= 13) /* It is afternoon */
        {
            sprintf(theHour, "%d:", hour - 12);
        }
        else /* It is morning */
        {
            sprintf(theHour, "%d:", hour);
        }
        sprintf(theMin, "%.2d", min);
        strcat(timeString, theHour);
        strcat(timeString, theMin);
        if(hour >= 12) /* Needs a PM suffix */
            strcat(timeString, " PM");
        else /* Needs an AM suffix */
            strcat(timeString, " AM");

        return timeString;
    }

  void PostEntrysubmitPost(struct userPost * st) {
        /* This will submit the post to the file specified by the user */
        updateStream(st);
    }

void constructorPostEntry(struct PostEntry * var)
{
    var->PostEntryreadInputc = &PostEntryreadInputc;
    var->PostEntryformatEntryc = &PostEntryformatEntryc;
    var->PostEntrygetTimeDate = &PostEntrygetTimeDate;
    var->PostEntrysubmitPost = &PostEntrysubmitPost;
}

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
    struct PostEntry thePost;
    constructorPostEntry(&thePost);
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

    enteredInfo = thePost.PostEntryreadInputc(username); /* Send the name of the user entered on command line */
    if(enteredInfo != NULL)
    {
        postTime = thePost.PostEntrygetTimeDate(); /* The time and date from time.h */
        strcat(enteredInfo, "|");
        strcat(enteredInfo, postTime);
        thePost.PostEntryformatEntryc(enteredInfo, &sentText); /* Formats the entry */
        thePost.PostEntrysubmitPost(&sentText); /* Submits the post */
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
