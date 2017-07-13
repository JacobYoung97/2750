# CIS*2750
This is the final code for CIS*2750 from University of Guelph. The Angel of Death course.
The overall assignment was creating a messaging system on the Internet. This is where users could add themselves to streams, and post in the streams. The messages were organized by both date posted and by alphabetical ordering.

Final grade = 82%. Assignment grade = 28.75/40 (mainly because of A1's 1/10 grade)

## Grade (and Assignment) Breakdown:
*********************************
A1: Convert C++ "Lite" to C
- This assignment was taking in a "C++" program and changing the code to work with a C compiler.
- Some of the changed aspects include changing objects to structs with function pointers and then calling those new functions, creating a "constructor" that would set the function pointers, etc. This can be seen in comparing post.cc to post.c.
1/10 (9/10 of the test cases failed when marking through auto-grading)

A2: 
- This assignment was a terminal version of the overall messaging program.
- This was the backbone of the assignment. It included making the initial versions of adding to streams (addauthor.c), posting to streams (post.c)
- It also utilized A1's conversion program to change post.cc to post.c
9/10 (I accidentally submitted the files I was testing with (-1))

A3: 
- This was the first assignment that interfaced the code that was written with the Internet.
- There was also an aspect that was needed to convert markup files (the .wpml files) into the webpages themselves that would display the content
10/10

A4: 
- This assignment was converting the usage of files in the previous two assignment to a database with SQL commands
8.75/10 (did not sort by user name alphabetically (-1) and did not display multiple user names (-0.25))

## Coding Languages Used:
*********************************
The code in this repository is for A4 which was the final version of the project.
The HTML that it creates is not structured with CSS or JavaScript to make it look nicer and perform differently (as of June 28, 2017).

Languages used during creation:
C, "C++", Python, PHP, HTML, SQL (embedded in C), and Makefiles

## Possible future additions:
*********************************
"Easy": add css stylesheet to be included at the <head> </head> and/or a title for the webpages
	-this can be added as a secondary file on the command line to execute the program that creates the webpages

"Easy": change the file structure to better organize the files with src/ , include/ , docs/ , and possible css or javascript later
	-will need to change where programs are called in every file

"Medium": add javascript functionality either from a script file on the command line or embedded in the tags (would need some more if-statements)	
