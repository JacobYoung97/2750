#!/usr/bin/python3

#Jacob Young 0932387
#CIS*2750 Assignment Four
#This python program will get the post number that the user needs to see
#Will only be called if the page is newly created or it forgot the number they need to see

import sys, os, subprocess

if __name__ == '__main__':

    if len(sys.argv) < 3:
        print("Error. The program did not work!")
        exit(1)

    #./postNum.py [stream] [userName]
    stream = sys.argv[1] #Note, this is only the prefix and we will need to add the appropriate suffix later
    commandLine = sys.argv[2:]
    username = ""
    k = 0
    for i in commandLine:
        if k != len(commandLine) - 1:
            username = username + i + " "
        else:
            username = username + i
        k += 1;

    #For example: "SELECT catsStreamUsers.seen FROM catsStreamUsers WHERE catsStreamUsers.username = 'username'
    database = stream + "StreamUsers"
    query = "SELECT " + database + ".seen FROM " + database + " WHERE " + database + ".username = '" + username + "'"
    subprocess.call(["./db", query])
    k = 0
    #Now we get the input from the ./db program (length of line is greater than 1 because of the number and the newline character
    with open("temp.txt", 'r') as theFile:
        for curLine in theFile:
            if len(curLine) > 1:
                print(curLine, end= "") #It should only be the number here
                k += 1 #Might be unnecessary but it is here to check if it happened or not
    subprocess.call('rm temp.txt', shell = True)
