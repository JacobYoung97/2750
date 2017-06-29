/***********************************************
Jacob Young 0932387
CIS*2750 Assignment Two
This is the .c file for the necessary library
The library is libstream.a to manipulate streams
***********************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#include "stream.h"

#define MAX_QUERY 1024 /* The text of a post is at max 800 chars (need to account for this) */
#define HOSTNAME "dursley.socs.uoguelph.ca"
#define USERNAME "jyoung17"
#define PASSWORD "0932387"
#define DATABASE "jyoung17"

/*
clstr
This function gets the query ready by setting the first character to the null terminator
IN: char * buf - the line used as the query string
*/
void clstr(char * buf)
{
    buf[0] = '\0';
}

/*
error
The SQL error message generated by a failure
IN: char * msg- the programmer written message; MYSQL * mysql - the mysql error message generated
*/
void error(char * msg, MYSQL * mysql)
{
    printf("%s\n %s\n", msg, mysql_error(mysql));
    exit(0); /* Change to an exit(1) later but when first testing I want to see the message generated */
}

/*
updateStream
This function will add a new posting to the stream which is specified in <stream-name> in post program.
If stream does not exist, then it creates a new one.
IN: struct userPost * st - the user's input for posting to stream (userName, streamName, date, text)
*/
void updateStream(struct userPost * st)
{
    MYSQL mysql;
    MYSQL_RES * res;
    MYSQL_ROW output;
    char * streamName = calloc(100, sizeof(char));
    char * sender = calloc(50, sizeof(char));
    char * date = calloc(80, sizeof(char));
    char query[MAX_QUERY];
    clstr(query);
    int bytes = 0;
    int data = 0;
    int postID;

    mysql_init(&mysql); /* Opening the database connection */
    mysql_options(&mysql, MYSQL_READ_DEFAULT_GROUP, "mydb");
    if (!mysql_real_connect(&mysql, HOSTNAME, USERNAME, PASSWORD, DATABASE, 0, NULL, 0))
    {
        error("Could not connect to database.",&mysql);
    }

    /* This block is getting the database for the streamName */
    strcat(streamName, st->streamname);
    strcat(streamName, "Stream");

    /* Adding a newline to the very end just to be safe */
    /* strcat(st->text, "\n"); */

    data = 0;

    /* This will see if an overall posts thing exists */
    strcpy(query, "SELECT COUNT(*) FROM posts");
    if(mysql_query(&mysql, query)) /* Table doesn't exist */
    {
        clstr(query);
        strcpy(query, "CREATE TABLE posts (sender char(30) NOT NULL, stream char(40) NOT NULL, time DATETIME NOT NULL, text varchar (800))");
        if(mysql_query(&mysql, query))
        {
            error("Failed to create posts table", &mysql);
        }
        clstr(query);        
        sprintf(query, "INSERT INTO posts (sender, stream, time, text) VALUES ('%s', '%s', '%s', '%s')", st->username, st->streamname, st->date, st->text);
        if(mysql_query(&mysql, query))
        {
            error("Failed to insert into posts", &mysql);
        }
    }
    else /* Table exists */
    {
        if(!(res = mysql_store_result(&mysql)))
        {
            error("Failed to store.", &mysql);
        }
        mysql_free_result(res); /* We don't actually need the result */
        clstr(query);
        sprintf(query, "INSERT INTO posts (sender, stream, time, text) VALUES ('%s', '%s', '%s', '%s')", st->username, st->streamname, st->date, st->text);
        if(mysql_query(&mysql, query))
        {
            error("Failed to insert into posts", &mysql);
        }
    }

    clstr(query);
    data = 0;
    sprintf(query, "SELECT COUNT(DISTINCT ID) AS POSTS FROM %s", streamName); /* Gets the number of posts */
    if(mysql_query(&mysql, query)) /* Table doesn't exist */
    {
        clstr(query);
        sprintf(query, "CREATE TABLE %s (ID INTEGER NOT NULL, sender CHAR(30) NOT NULL, stream CHAR(40) NOT NULL, time DATETIME NOT NULL, text VARCHAR(800) NOT NULL, PRIMARY KEY (ID))", streamName);
        if(mysql_query(&mysql, query))
        {
            error("Failed to create table!", &mysql);
        }
        postID = 1;
        clstr(query);
        sprintf(query, "INSERT INTO %s (ID, sender, stream, time, text) VALUES (%d, '%s', '%s', '%s', '%s')", streamName, postID, st->username, st->streamname, st->date, st->text);
        if(mysql_query(&mysql, query))
        {
            error("Failed to insert into new table!", &mysql);
        }
    }
    else /* Table exists and we find how many posts there are */
    {
        if(!(res = mysql_store_result(&mysql)))
        {
            error("Failed to store.", &mysql);
        }
        while((output = mysql_fetch_row(res)))
        {
            data = atoi(output[0]);
        }
        mysql_free_result(res);
        postID = data + 1;
        clstr(query);
        sprintf(query, "INSERT INTO %s(ID, sender, stream, time, text) VALUES (%d, '%s', '%s', '%s', '%s')", streamName, postID, st->username, st->streamname, st->date, st->text);
        if(mysql_query(&mysql, query))
        {
            error("Failed to insert into existing table!", &mysql);
        }
    }

    /* This block is for the sender (FOR THE BYTES) */
    strcat(sender, "Sender: ");
    strcat(sender, st->username);

    /* This block is for the date (FOR THE BYTES) */
    strcat(date, "Date: ");
    strcat(date, st->date);

    free(streamName);
    streamName = calloc(100, sizeof(char)); /* SQL STATEMENTS INSTEAD */
    strcat(streamName, st->streamname);
    strcat(streamName, "StreamData");
    bytes += strlen(sender);
    bytes += strlen(date);
    bytes += strlen(st->text); /* Gets the size of the post for later deduction on */
    data = 0; /* In case it doesn't return anything anyways (0 is my 'it doesn't exist' clause) */
    clstr(query);

    sprintf(query, "SELECT %s.data FROM %s ORDER BY data", streamName, streamName);
    if(mysql_query(&mysql, query)) /* Table doesn't exist */
    {
        clstr(query);
        sprintf(query, "CREATE TABLE %s (data INTEGER NOT NULL, PRIMARY KEY (data))", streamName);
        if(mysql_query(&mysql, query))
        {
            error("Failed to create table", &mysql);
        }
        clstr(query);
        sprintf(query, "INSERT INTO %s(DATA) VALUES (%d)", streamName, bytes);
        if(mysql_query(&mysql, query))
        {
            error("Failed to insert into table", &mysql);
        }
    }
    else /* The table exists */
    {
        if(!(res = mysql_store_result(&mysql)))
        {
            error("Failed to store.", &mysql);
        }
        while((output = mysql_fetch_row(res))) /* Should only return one number which is the number of bytes at max */
        {
            data = atoi(output[0]); /* This keeps getting assigned until the last number (last post's number of bytes) */
        }
        mysql_free_result(res);
        bytes += data;
        sprintf(query, "INSERT INTO %s(DATA) VALUES (%d)", streamName, bytes);
        if(mysql_query(&mysql, query))
        {
            error("Failed to insert into data", &mysql);
        }
    }

    free(streamName);
    free(sender);
    free(date);
    mysql_close(&mysql); /* Closing the database connection */
    mysql_library_end();
}

/*
addUser
This function will add the user/author to all streams specified in the addAuthor program
IN: char * username - the specified userName on command line; char * list - the CSV list of the stream names
*/
void addUser(char * username, char * list)
{
    MYSQL mysql;
    MYSQL_RES * res;
    MYSQL_ROW output;
    char * streamName;
    char * tokedLine;
    char query[MAX_QUERY];
    int i;
    clstr(query);

    mysql_init(&mysql); /* Opening the database connection */
    mysql_options(&mysql, MYSQL_READ_DEFAULT_GROUP, "mydb");
    if (!mysql_real_connect(&mysql, HOSTNAME, USERNAME, PASSWORD, DATABASE, 0, NULL, 0))
    {
        error("Could not connect to database.",&mysql);
    }

    /* This will insert/create an overall users table */
    strcpy(query, "SELECT COUNT(*) AS USERS FROM users");
    if(mysql_query(&mysql, query)) /* Table doesn't exist so we have to create it */
    {
        clstr(query);
        strcpy(query, "CREATE TABLE users (username char(30) NOT NULL, PRIMARY KEY (username))");
        if(mysql_query(&mysql, query))
        {
            error("Failed to create users table!", &mysql);
        }
        clstr(query);
        sprintf(query, "INSERT INTO users (username) VALUES ('%s')", username);
        if(mysql_query(&mysql, query))
        {
            error("Failed to add to users table", &mysql);
        }
    }
    else /* Table exists */
    {
        if(!(res = mysql_store_result(&mysql)))
        {
            error("Failed to store a table name!", &mysql);
        }
        mysql_free_result(res);
        clstr(query);
        sprintf(query, "SELECT users.username FROM users WHERE users.username = '%s'", username);
        if(mysql_query(&mysql, query)) /* Username not in the table (if it doesn't fail then it is already in there) */
        {
            error("Failed to select!", &mysql);
        }
        if(!(res = mysql_store_result(&mysql)))
        {
            error("Failed to store!", &mysql);
        }
        while((output = mysql_fetch_row(res)))
        {
            i += 1;
        }
        mysql_free_result(res);
        if(i == 0) /* The username does not exist */
        {
            clstr(query);
            sprintf(query, "INSERT INTO users (username) VALUES ('%s')", username);
            if(mysql_query(&mysql, query))
            {
                error("Could not insert username", &mysql);
            }
        }
    }

    /* This will create a streams table */
    i = 0;
    clstr(query);
    strcpy(query, "SELECT COUNT(*) FROM streams");
    if(mysql_query(&mysql, query)) /* If this returns an error, table doesn't exist */
    {
        strcpy(query, "CREATE TABLE streams (name char(30) NOT NULL, primary key(name))");
        if(mysql_query(&mysql, query))
        {
            error("Failed to create streams!", &mysql);
        }
    }
    else
    {
        if(!(res = mysql_store_result(&mysql)))
        {
            error("Failed to store count!", &mysql);
        }
        mysql_free_result(res);
    }

    tokedLine = strtok(list, ",");

    while(tokedLine != NULL) /* This will get all the streams specified and make/insert for their own tables */
    {
        i = 0;
        streamName = calloc(60, sizeof(char));
        strcat(streamName, tokedLine);
        strcat(streamName, "StreamUsers");

        clstr(query);
        sprintf(query, "SELECT streams.name FROM streams WHERE streams.name = '%s'", tokedLine);
        if(mysql_query(&mysql, query)) /* If it failed to select, it must not exist in the streams table */
        {
            error("Failed to select stream name", &mysql);
        }
        if(!(res = mysql_store_result(&mysql)))
        {
            error("Failed to store a table name!", &mysql);
        }
        while((output = mysql_fetch_row(res)))
        {
            i += 1;
        }
        mysql_free_result(res);
        
        if(i == 0) /* Empty set so they need to insert this stream into the table */
        {
            clstr(query);
            sprintf(query, "INSERT INTO streams (name) VALUES ('%s')", tokedLine);
            if(mysql_query(&mysql, query))
            {
                error("Failed to insert into streams table!", &mysql);
            }
        }
        clstr(query);
        sprintf(query, "SHOW TABLES LIKE '%s'", streamName); /* Finds the table that is the StreamUsers */
        if(mysql_query(&mysql, query)) /* It doesn't exist */
        {
            error("Failed to show tables", &mysql);
        }
        if(!(res = mysql_store_result(&mysql)))
        {
            error("Failed to store table name!", &mysql);
        }
        i = 0;
        while((output = mysql_fetch_row(res)))
        {
            i += 1;
        }
        mysql_free_result(res);

        if(i == 0) /* Empty set (table does not exist) */
        {
            clstr(query);
            sprintf(query, "CREATE TABLE %s (username char(30) NOT NULL, seen integer NOT NULL, PRIMARY KEY (username))", streamName);
            if(mysql_query(&mysql, query)) /* It doesn't exist */
            {
                error("Failed to create a StreamUsers table", &mysql);
            }
            clstr(query);
            sprintf(query, "INSERT INTO %s (username, seen) VALUES ('%s', 0)", streamName, username);
            if(mysql_query(&mysql, query)) /* It doesn't exist */
            {
                error("Failed to insert into a StreamUsers table", &mysql);
            }
            printf("%s was inserted into the %s stream<BR>\n", username, tokedLine); /* If our insertion worked for the table */
        }
        else /* A table exists */
        {
            i = 0;
            clstr(query);
            sprintf(query, "SELECT %s.username from %s where %s.username = '%s'", streamName, streamName, streamName, username);
            if(mysql_query(&mysql, query))
            {
                error("Failed to select username from a stream!", &mysql);
            }
            if(!(res = mysql_store_result(&mysql)))
            {
                error("Failed to store username", &mysql);
            }
            while((output = mysql_fetch_row(res)))
            {
                i += 1;
            }
            mysql_free_result(res);
            if(i == 0) /* Username not in stream */
            {
                clstr(query);
                sprintf(query, "INSERT INTO %s (username, seen) VALUES ('%s', 0)", streamName, username);

                if(mysql_query(&mysql, query))
                {
                    error("Failed to insert into table!\n", &mysql);
                }
                printf("%s was inserted into the %s stream<BR>\n", username, tokedLine); /* If our insertion worked for the table */
            }
            else
            {
                printf("%s already in the %s stream!<BR>\n", username, tokedLine);
            }
        }
        free(streamName);
        tokedLine = strtok(NULL, ",");
    }
    mysql_close(&mysql); /* Closing the database connection */
    mysql_library_end(); /* Freeing memory at the end */
}

/*
removeUser
This function is very similar to the addUser excepts it removes them from the streams specified
IN: char * username - specified on command line; char * list - specified CSV to remove
*/
void removeUser(char * username, char * list)
{
    MYSQL mysql;
    MYSQL_RES * res;
    MYSQL_ROW output;
    int i;
    char * tokedLine;
    char * streamName;
    char query[MAX_QUERY];
    clstr(query);

    mysql_init(&mysql); /* Opening the database connection */
    mysql_options(&mysql, MYSQL_READ_DEFAULT_GROUP, "mydb");
    if (!mysql_real_connect(&mysql, HOSTNAME, USERNAME, PASSWORD, DATABASE, 0, NULL, 0))
    {
        error("Could not connect to database.",&mysql);
    }

    tokedLine = strtok(list, ",");

    while(tokedLine != NULL) /* Gets all stream names */
    {
        i = 0;
        streamName = calloc(60, sizeof(char));
        strcat(streamName, tokedLine);
        strcat(streamName, "StreamUsers"); /* REPLACE WITH SQL QUERY */

        clstr(query);
        sprintf(query, "SHOW TABLES LIKE '%s'", streamName); /* Finds the table that is the StreamUsers */
        if(mysql_query(&mysql, query))
        {
            error("Error showing tables", &mysql);
        }
        
        /* In here, we need to see if it actually returned some output */
        if(!(res = mysql_store_result(&mysql)))
        {
            error("Failed to store table", &mysql);
        }
        while((output = mysql_fetch_row(res)))
        {
            i += 1;
        }
        mysql_free_result(res);
        
        if(i == 0) /* The table doesn't even exist */
        {
            printf("%s stream doesn't even exist!<BR>\n", tokedLine);
        }
        else /* It exists so we check for it */
        {
            i = 0;
            clstr(query);
            sprintf(query, "SELECT %s.username from %s where %s.username = '%s'", streamName, streamName, streamName, username);
            if(mysql_query(&mysql, query))
            {
                error("Failed to select a username!", &mysql);
            }
            if(!(res = mysql_store_result(&mysql)))
            {
                error("Failed to store table", &mysql);
            }
            while((output = mysql_fetch_row(res)))
            {
                i += 1;
            }
            mysql_free_result(res);
            if(i == 0)
            {
                printf("%s is not in the %s stream!<BR>\n", username, tokedLine);
            }
            else
            {
                clstr(query);
                sprintf(query, "DELETE FROM %s WHERE %s.username = '%s'", streamName, streamName, username);
                if(mysql_query(&mysql, query))
                {
                    error("Failed to delete!", &mysql);
                }
                printf("%s was removed from the %s stream!<BR>\n", username, tokedLine);
            }
        }
        free(streamName);
        tokedLine = strtok(NULL, ",");
    }
    mysql_close(&mysql); /* Closing the database connection */
    mysql_library_end();
}
