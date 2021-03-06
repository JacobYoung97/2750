/***********************************************
Jacob Young 0932387
CIS*2750 Assignment Two
This is the .c file for the necessary library
The library is libstream.a to manipulate streams
***********************************************/

#ifndef __STREAM_H__
#define __STREAM_H__

#include <mysql/mysql.h>

/*
struct userPost
This struct is the information from post that the user enters
into a particular stream
*/
struct userPost {
    char * username; /* Who they are */
    char * streamname; /* Where they are posting */
    char * date; /* The time they post */
    char * text; /* What they are saying */
};

/* 
clrstr
This function gets the query ready
IN: char * buf - the line used as the query string 
*/
void clstr(char * buf);

/*
error
The SQL error message generated by a failure
IN: char * msg- the programmer written message; MYSQL * mysql - the mysql error message
*/
void error(char * msg, MYSQL * mysql);

/*
updateStream
This function will add a new posting to the stream which is specified in <stream-name> on command line.
If stream does not exist, then it creates a new one.
IN: struct userPost * st - the user's input for posting to stream (userName, streamName, date, text)
*/
void updateStream(struct userPost * st);

/*
addUser
This function will add the user/author to all streams specified in the addAuthor program
IN: char * username - the specified userName on command line; char * list - the CSV list of the stream names
*/
void addUser(char * username, char * list);

/*
removeUser
This function is very similar to the addUser excepts it removes them from the streams specified
IN: char * username - specified on command line; char * list - specified CSV to remove
*/
void removeUser(char * username, char * list);

#endif
