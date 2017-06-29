*******************************************
Jacob Young 0932387
CIS*2750 Assignment 4
This takes A3 and adds MYSQL functionality
*******************************************

***************
Compiling/Using
***************
-type 'make' to create all the .c and .py files necessary to run on the server
-type ./db [flag] to use the commands in the assignment ("-help" to see what each one does)
    -I have also made a new command check in the program for the python programs to be able to access the SQL functions
    -this is basically just sending in a query from the python programs and the db program will do what is necessary with the output and send to a file

***********
Limitations
***********
-the database reset program is getting extra functionality to pass through SQL commands so the python programs can get this functionality
    -the assignment states that I needed to have "some changes to the Python program to access the database using C" so my change is using subprocess.call() to give the db program the command
-the "-posts" output might be a little disorganized depending on how large the actual text from the post is
-in the db program, if you try to use an option before anything exists, depending on the option, it will may fail on you (like -posts before any have been made)
-not sure how the program is being tested but the only occurences of the #defines that are necessary to change the database are in stream.c and db.c at the top of the .c file
-to check for new posts only works with a single stream's viewing
-dates will be in the time format YYYY-MM-DD HH:MM:SS (in 24 hour clock)
-for some reason some of the posts aren't being seen on the view page but they are in the tables as seen with ./db -posts
    -it does not want to display the stream name or the actual text but using the above command "-posts" will show you that it did save correctly
-if there are too many tables, the program looking for the streams a user is a part of may not work
-viewing "all" does not work (could not figure out how to mainpulate the tables effectively enough to retrieve posts from all the streams they are a part of)
-the sorting by name does not work either as I could not get a row_number based on username/sender to work
-some limits from previous assignment still exist (limit of 30 chars for username, 24 for stream name, 800 for a posting)
