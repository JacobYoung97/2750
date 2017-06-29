/************************************************************
Jacob Young 0932387
CIS*2750 Assignment Four
This is the database reset program to handle certain things.
Some things include clearing and printing all streams.
************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>

#define MAX_QUERY 512
#define HOSTNAME ""
#define USERNAME ""
#define PASSWORD ""
#define DATABASE ""

void error(char * msg, MYSQL *mysql)
{
    printf("%s\n%s\n", msg, mysql_error(mysql));
    exit(1);
}

void clrstr(char * buf)
{
    buf[0] = '\0';
}

int main(int argc, char * argv[])
{
    if(argc < 2)
    {
        printf("No command given.\n");
        printf("Run with -help to see available commands!\n");
        return 0;
    }

    MYSQL mysql;
    MYSQL_RES *res;
    MYSQL_ROW output;
    FILE * temp;
    char query[MAX_QUERY];
    int numTables = 50;
    char ** dbNames = calloc(50, sizeof(char*));
    int i = 0;
    int j = 0;

    mysql_init(&mysql);
    mysql_options(&mysql, MYSQL_READ_DEFAULT_GROUP, "mydb");

    /* This will happen if the connection does not happen */
    if(!mysql_real_connect(&mysql, HOSTNAME, USERNAME, PASSWORD, DATABASE, 0, NULL, 0))
    {
        error("Could not connect to host.", &mysql);
    }

    clrstr(query);

    if(strcmp("-clear", argv[1]) == 0)
    {
        /* This will clear the tables but not delete them */
        strcpy(query, "show tables");
        if(mysql_query(&mysql, query))
        {
            error("Failed showing tables", &mysql); /* The SQL query failed to display the tables */
        }
        if(!(res = mysql_store_result(&mysql))) /* The output could not be stored */
        {
            error("Failed to store output!", &mysql);
        }
        while((output = mysql_fetch_row(res)))
        {
            dbNames[i] = calloc(strlen(output[0]) + 1, sizeof(char));
            strcpy(dbNames[i], output[0]);
            i += 1;
            if(i == numTables)
            {
                numTables += 25; /* Will then make room for more tables to be cleared*/
                dbNames = realloc(dbNames, sizeof(char*) * numTables);
            }
        }
        mysql_free_result(res);
        for(j = 0; j < i; j++) /* Will go through and then truncate all table information */
        {
            clrstr(query);
            sprintf(query, "TRUNCATE TABLE %s", dbNames[j]);
            if(mysql_query(&mysql, query))
            {
                error("Failed to truncate/clear a table!", &mysql);
            }
        }
        printf("Cleared information from all tables in database!\n");
    }
    else if(strcmp("-reset", argv[1]) == 0)
    {
        /* This will drop the tables completely */
        strcpy(query, "show tables");
        if(mysql_query(&mysql, query))
        {
            error("Failed to show tables!", &mysql);
        }
        if(!(res = mysql_store_result(&mysql)))
        {
            error("Failed to store result!", &mysql);
        }
        while((output = mysql_fetch_row(res)))
        {
            dbNames[i] = calloc(strlen(output[0]) + 1, sizeof(char));
            strcpy(dbNames[i], output[0]);
            i += 1;
            if(i == numTables)
            {
                numTables += 25;
                dbNames = realloc(dbNames, sizeof(char*) * numTables);
            }
        }
        mysql_free_result(res);
        for(j = 0; j < i; j++) /* Goes through and drops all tables */
        {
            clrstr(query);
            sprintf(query, "DROP TABLE %s", dbNames[j]);
            if(mysql_query(&mysql, query))
            {
                error("Failed to drop a table!", &mysql);
            }
        }
        printf("Deleted all tables in the database!\n");
    }
    else if(strcmp("-posts", argv[1]) == 0)
    {
        /* This will write out all the posts in the database */
        strcpy(query, "SELECT * FROM posts");
        if(mysql_query(&mysql, query))
        {
            error("Failed to get posts!", &mysql);
        }
        if(!(res = mysql_store_result(&mysql)))
        {
            error("Failed to store result!", &mysql);
        }
        while((output = mysql_fetch_row(res))) /* This will check the returned tables for the appropriate ones (*Stream) */
        {
            for(i = 0; i < mysql_num_fields(res); i++)
            {
                printf("%s ", output[i]);
            }
            printf("\n");
        }
        mysql_free_result(res);
    }
    else if(strcmp("-users", argv[1]) == 0)
    {
        /* This will write out all the users in the database */
        strcpy(query, "SELECT DISTINCT users.username FROM users");
        if(mysql_query(&mysql, query))
        {
            error("Failed to get usernames!", &mysql);
        }
        if(!(res = mysql_store_result(&mysql)))
        {
            error("Failed to store result!", &mysql);
        }
        printf("USERS:\n");
        while((output = mysql_fetch_row(res))) /* Prints out the usernames */
        {
            printf("%s\n", output[0]);
        }
        mysql_free_result(res);
    }
    else if(strcmp("-streams", argv[1]) == 0)
    {
        /* This will print out all the streams in the database */
        strcpy(query, "SELECT DISTINCT streams.name FROM streams");
        if(mysql_query(&mysql, query))
        {
            error("Failed to show tables!", &mysql);
        }
        if(!(res = mysql_store_result(&mysql)))
        {
            error("Failed to store result!", &mysql);
        }
        printf("STREAMS:\n");
        while((output = mysql_fetch_row(res))) /* Will get all the streams */
        {
            printf("%s\n", output[0]);
        }
        mysql_free_result(res);
    }
    else if(strcmp("-help", argv[1]) == 0) /* A help 'page' */
    {
        printf("Every command is used in the form: \"db [command]\"\n");
        printf("Run with -clear to clear all the tables without deleting them\n");
        printf("Run with -reset to delete all the tables in the database\n");
        printf("Run with -posts to print out all stored posts in the database\n");
        printf("Run with -users to print out all usernames stored in the database\n");
        printf("Run with -streams to print out all the stream names\n");
    }
    else
    {
        /* This is where the passed commands go from command line; written to a file to retrieve later */
        temp = fopen("temp.txt", "w");
        strcpy(query, argv[1]);
        if(mysql_query(&mysql, query))
        {
            exit(0); /* Should exit quietly */
        }
        if(!(res = mysql_store_result(&mysql)))
        {
            exit(0); /* Exiting quietly */
            error("Failed to store", &mysql);
        }
        while((output = mysql_fetch_row(res))) /* Gets all possible input and sends it to the file */
        {
            for(i = 0; i < mysql_num_fields(res); i++)
            {
                strcat(output[i], "\n"); /* Appends a newline to the current column so it can easily print to file */
                fputs(output[i], temp);
            }
        }
        mysql_free_result(res);
        fclose(temp);
    }

    /* Closing the connection and freeing memory usage */
    mysql_close(&mysql);
    mysql_library_end();
    for(i = 0; i < numTables; i++)
    {
        free(dbNames[i]);
    }
    free(dbNames);

    return 0;
}
