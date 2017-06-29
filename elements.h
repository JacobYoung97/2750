/*************************************************
Jacob Young 0932387
CIS*2750 Assignment 3
This assignment converts A2 to work with webpages
*************************************************/

#ifndef __ELEMENTS_H__
#define __ELEMENTS_H__

/*
assign
This should run the assignment programs from A2 with setting some variables and other things
IN: char * element - this is the assignment programs name and other useful things; int index - the index of the element
OUT: int - because this can include another attribute inside of it, we need to return the position of the last read thing in it
*/
int assign(char * element, int index);

/*
button
This function will read in a button that the user can select
IN: char * element - the text containing the button; int index - the starting index of the attributes
*/
void button(char * element, int index);

/*
exe
This will read in the executable and place it into the file to run on server
IN: char * element - this is the exe attribute; int index - the start of the index
*/
void exe(char * element, int index);

/*
exePrint
This will print out the php necessary to run the executable
IN: char * command - the command being run
*/
void exePrint(char * command);

/*
heading
This will create a heading on the page
IN: char * element - this is the start of the attribute for heading; int index - the starting location
*/
void heading(char * element, int index);

/*
input
This function creates the area where a user can enter one or more textboxes of input
IN: chear * element - the line contanining the data; int index - start of the attributes
*/
void input(char * element, int index);

/*
line
This function is for if any other input was given to the horizontal tag
IN: char * element - the line in question; int index - where it starts
*/
void line(char * element, int index);

/*
link
This function will create a link on a web page with default name "link"
IN: char * element - the actual text containing link info; int index - the start of the elements
*/
void link(char * element, int index);

/*
multiple
This custom tag will create a form with multiple input values associated with it
IN char * element - the attribute itself; int index - the starting location of the attribute
*/
void multiple(char * element, int index);

/*
picture
This will load in a picture to the web page
IN: char * element - the tag for the picture; int index - the start of the attributes for the tag
*/
void picture(char * element, int index);

/*
radio
This function will create a list of options on the webpage
IN: char * element - the tag's attributes for creation; int index - the start of the tag's attributes
*/
void radio(char * element, int index);

/*
text
This function will display the text in either a file or entered in the .wpml file
IN: char * element - the tag itself; int index - the start of it's attributes
*/
void text(char * element, int index);

/*
lastBracket
This is used to find the index of the last bracket to skip ahead in reading a line
IN: char * line - the fileLine itself; int index - the starting index of the stuff in the attribute
OUT: int - the index where the last ')' is located
*/
int lastBracket(char * line, int index);

/*
dataStart
This function will be used to skip forward to where the actual data of the tags starts
IN: char * element - the overall attribute used to move forward; int index - the start of where we need to skip through
OUT: int - the starting index of where the data will be written
*/
int dataStart(char * element, int index);

#endif
