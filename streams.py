#!/usr/bin/python3

#Jacob Young 0932387
#CIS*2750 Assignment Three
#This program will check the inputted username and see what streams they are a part of

import sys, subprocess

if __name__ == '__main__':
    if len(sys.argv) == 1:
        print("No username entered. Please press the button to return to the login page.<BR>")
        exit()
    streams = []
    shortNames = []
    streamList = []
    trimming = ""
    curname = ""
    query = ""
    k = 0
    j = 0
    commandLine = sys.argv[1:]
    username = ""
    for i in commandLine:
        if k != len(commandLine) - 1:
            username = username + i + " "
        else:
            username = username + i
        k += 1
    
    query = "show tables like '%Users'"
    subprocess.call(["./db", query])
    with open("temp.txt", 'r') as table:
        for curLine in table:
            if "Users" in curLine: #if it is catsStreamUsers for example
                streams.append(curLine)
    subprocess.call('rm temp.txt', shell = True)

    if len(streams) == 0: #We did not find any *StreamUsers tables
        print("There are no streams yet! Press the button to go create a new one.")
        print("<form method=\"post\" action=\"author.php\">")
        print("<input type=\"hidden\" name=\"user\" value=\"" + username + "\">")
        print("<input type=\"submit\" value=\"Add Streams\">")
        print("</form>")

    else: #We need to see if the user is in these gotten streams or not
        for i in streams:
            trimming = i.strip('\n')
            query = "SELECT " + trimming + ".username FROM " + trimming + " WHERE " + trimming + ".username = '" + username + "'"
            subprocess.call(["./db", query])
            with open("temp.txt", 'r') as users:
                for curLine in users:
                    if len(curLine) > 1: #If not found, it should be length 1 for the newline character (or 0 if it is the EOF)
                        streamList.append(trimming) #This will append the current *StreamUsers if there username was found in it
            subprocess.call('rm temp.txt', shell = True)

        curname = ""
        if len(streamList) == 0: #No streams found with this username
            print("No streams found with this username!")
            print("<form method=\"POST\" action=\"author.php\">")
            print("<input type=\"hidden\" name=\"user\" value=\"" + username + "\">")
            print("<input type=\"submit\" value=\"Add To Streams\">")
            print("</form>")
        else:
            print("<form method=\"POST\" action=\"view.php\">")
            print("<input type=\"hidden\" name=\"user\" value=\"" + username + "\">")
            for name in streamList: #Getting individual stream names
                suffix = 0
                for i in name:
                    if i == "S":
                        suffix = 1
                    elif suffix == 0:
                        curname = curname + i
                shortNames.append(curname)
                curname = ""

            for stream in shortNames: #Printing them in a form
                    print("<input type=\"radio\" name=\"streamChoice\" value=\"" + stream + "\">" + stream + "<BR>")
            print("<input type=\"radio\" name=\"streamChoice\" value=\"all\">all<BR>")
            print("<input type =\"submit\" value=\"Submit\">")
            print("</form>")
