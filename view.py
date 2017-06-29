#!/usr/bin/python3

#Jacob Young 0932387
#CIS*2750 Assignment Two
#This is the python program to view the messages of the streams and updates (updated to web interface)

import sys, readline, subprocess
import glob, re, os
import collections

if __name__ == '__main__':

    if len(sys.argv) < 6: #If they only enter the script name (nothing was passed properly)
        print("Error. The program did not work.")
        exit()

    query = ""
    database = ""
    k = 0
    singleStream = 0 #Different operations if we are viewing "all" streams
    postNums = 0
    totalPosts = 0
    username = ""
    seenPosts = 0
    pressedO = 0 #To change the sorting

    #./view.py [post#] [sorting] [stream] [inputChoice] [user] (THE COMMMAND LINE PASSED)
    viewing = sys.argv[1] #Not technically, but it is used to tell if we should look for the newest post
    sorting = sys.argv[2] #How they sort the output (name should use the order by for username)
    if sorting == "chrono":
        pressedO = 0
    elif sorting == "name":
        pressedO = 1
    streamChoice = sys.argv[3] #all, cats, dogs, etc.
    command = sys.argv[4] #inputChoice
    commandLine = sys.argv[5:] #Gets the username
    for i in commandLine:
        if k != len(commandLine) - 1:
            username = username + i + " "
        else:
            username = username + i
        k += 1;

    k = 0
    postNums = 0

    if streamChoice == "all": #THE QUERY FOR THIS SHOULD BE THE OVERALL POST TABLE WITH THE USERNAME
        singleStream = 0
        #query = "SELECT posts.sender, posts.stream, posts.time FROM posts WHERE posts.sender = " + username + " AND ROW_NUMBER = 1 (FOR THE FIRST POST THEY SEE IN THIS) 
        #query = ? HOW DO I ACTUALLY GET THE FIRST STREAM THEY ARE A PART OF?
        #SAVE THE STREAM NAMES FOR LATER USES!!!!!!

    else:
        database = streamChoice + "Stream"
        singleStream = 1
        #This should be getting the count of the posts in here (if 0, then it should state this)
        query = "SELECT COUNT(DISTINCT ID) AS POSTS FROM " + database
        subprocess.call(["./db", query])
        with open("temp.txt", 'r') as theFile:
            for curLine in theFile:
                tempName = curLine.strip('\n')
                postNums = int(tempName) #The file should only have one line anyways which is the number of posts
        subprocess.call('rm temp.txt', shell = True)

    if postNums == 0 and streamChoice != "all": #There are no posts in an individual stream
        print("There are no posts yet! ")
        print("Click the button above this to post to " + streamChoice + " stream!")
        exit()
    elif postNums == 0 and streamChoice == "all":
        print("Cannot post to all! ")
        print("Change streams to a single one to be able to post!")
        exit()
    #if there are posts in the stream, we need to get the postNum one (done below at the bottom)

    if command == "Change": #They want to change the order that it is sorted by
        if sorting == "chrono": #The SQL for this one will be by ID number
            pressedO = 0
        elif sorting == "name": #The SQL for this one will be sorted by name and getting the row_number (sorted by this first)
            pressedO = 1 #This will then get the row by the row_number() to print it out 

    elif command == "Mark":
        if singleStream == 1: #For single stream, just needs the count of posts and then updates the StreamUsers database
            #Says that they have seen every post in a single stream
            postNums = 0
            database = streamChoice + "Stream"
            query = "SELECT COUNT(DISTINCT ID) FROM " + database
            subprocess.call(["./db", query])
            with open("temp.txt", 'r') as posts:
                for curLine in posts:
                    postNums = curLine.strip('\n')
            subprocess.call('rm temp.txt', shell = True)
            database = streamChoice + "StreamUsers"
            query = "UPDATE " + database + " SET seen = " + postNums + " WHERE " + database + ".sender = '" + username + "'"
            subprocess.call(["./db", query]) #We don't need to check because it never outputs anyways
            subprocess.call('rm temp.txt', shell = True)

        elif singleStream == 0: #Marks in all streams
            query = "show tables" #This will get all streams and then it will find the number of posts in each "*Stream"

    elif command == "Check": #This will jump to the newest and freshest post available
        totalPosts = 0
        if singleStream == 1:
            database = streamChoice + "StreamUsers"
            #The query here will be to get the number that they have seen and check if count is larger (if so, increment by 1)
            query = "SELECT " + database + ".seen FROM " + database + " WHERE " + database + ".username = '" + username + "'"
            subprocess.call(["./db", query])
            with open("temp.txt", 'r') as check:
                for curLine in check:
                    postNums = curLine.strip('\n')
            subprocess.call('rm temp.txt', shell = True)
            database = streamChoice + "Stream"
            query = "SELECT COUNT(*) AS POSTS FROM " + database
            subprocess.call(["./db", query])
            with open("temp.txt", 'r') as posts:
                for curLine in posts:
                    totalPosts = curLine.strip('\n')
            subprocess.call('rm temp.txt', shell = True)
            if int(totalPosts) > int(postNums):
                viewing = int(viewing) + 1
        elif singleStream == 0:
            #Need some way of telling which streams they are a part of (STORE THEM ALL AT THE BEGINNING OF THE SESSION)
            print("ALL")

    if pressedO == 0: #Sorting single stream
        postNums = 0
        k = 0
        if singleStream == 1: #Viewing a single stream chronologically
            database = streamChoice + "Stream"
            query = "SELECT COUNT(DISTINCT ID) FROM " + database
            subprocess.call(["./db", query])
            with open("temp.txt", 'r') as posts:
                for curLine in posts:
                    postNums = curLine.strip('\n')
            subprocess.call('rm temp.txt', shell = True)
            if int(viewing) > int(postNums):
                viewing = int(viewing) - 1
            elif int(viewing) <= 0:
                viewing = 1
            query = "SELECT " + database + ".sender, " + database + ".stream, " + database + ".time FROM " + database + " WHERE " + database + ".ID = " + str(viewing)
            subprocess.call(["./db", query])
            with open("temp.txt", 'r') as post:
                for curLine in post:
                    if k == 0:
                        print("Sender: ", end = "")
                    elif k == 1:
                        print("Stream: ", end = "")
                    elif k == 2:
                        print("Date: ", end = "")
                    if k == 1 and len(curLine) == 1:
                        print(streamChoice)
                    print(curLine)
                    print("<BR>")
                    k += 1
            query = "SELECT " + database + ".text FROM " + database + " WHERE " + database + ".ID = " + str(viewing)
            subprocess.call(["./db", query])
            with open("temp.txt", 'r') as text:
                for curLine in text:
                    print(curLine)
                    print("<BR>")

        elif singleStream == 0: #Viewing all streams chronologically
            streams = []
            query = "show tables"
            subprocess.call(["./db", query])
            subprocess.call('rm temp.txt', shell = True)
            #CHECK THE FILE FOR THINGS ENDING WITH "STREAM" THEN APPEND TO streams LIST BY PERMISSIONS

        if singleStream == 1: #This will update the *StreamUsers to be accurate
            database = streamChoice + "StreamUsers"
            query = "SELECT " + database + ".seen FROM " + database + " WHERE username = '" + username + "'"
            subprocess.call(["./db", query])
            with open("temp.txt", 'r') as users:
                for curLine in users:
                    num = curLine.strip('\n')
            if int(num) < int(viewing): #Every new post they see will update the table
                query = "UPDATE " + database + " SET seen = " + str(viewing) + " WHERE " + database + ".username = '" + username + "'"
                subprocess.call(["./db", query])
            subprocess.call('rm temp.txt', shell = True);

        elif singleStream == 0: #Viewing all streams to update every single one to mark what has been seen
            k = 0
            #seenPosts = {}
            #splitName = []
            #theSplit = ""
            #for j in range(1, int(viewing) + 1): #Goes up to the post they have seen
                #for k in postDict[j]:
                    #if "Stream:" in k:
                        #splitName = k.split(' ')
                        #theSplit = splitName[1].strip('\n')
                        #if theSplit in seenPosts:
                            #seenPosts[theSplit] += 1
                        #else:
                            #seenPosts[theSplit] = 0
                            #seenPosts[theSplit] += 1
            #for m in seenPosts: #For all the names in seen posts
                #theLines = []
                #theFile = "messages/" + m + "StreamUsers"
                #with open(theFile, 'r') as users:
                    #for curLine in users:
                        #theLines.append(curLine)
                #for i in theLines:
                    #if username in i:
                        #userLine = l
                        #alreadyRead = i
                    #l += 1
                #l = 0
                #split = alreadyRead.split(' ')
                #num = split[len(split) - 1].strip('\n')
                #if int(num) < seenPosts[m]: #Gets the number stored here
                    #newLine = username + " " + str(seenPosts[m]) + "\n"
                #else:
                    #newLine = username + " " + str(num) + "\n"
                #k = 0
                #with open(theFile, 'w') as update: #Updates and writes the new number of seen posts
                    #for curLine in theLines:
                        #if k == userLine:
                            #update.write(newLine)
                        #else:
                            #update.write(theLines[k])
                        #k += 1

    elif pressedO == 1: #They are sorting by name and not by time posted
        if singleStream == 1: #A single stream by name
            database = streamChoice + "Stream"
            query = "SELECT COUNT(DISTINCT ID) FROM " + database
            subprocess.call(["./db", query])
            with open("temp.txt", 'r') as posts:
                for curLine in posts:
                    postNums = curLine.strip('\n')
            subprocess.call('rm temp.txt', shell = True)
            if int(viewing) > int(postNums):
                viewing = int(viewing) - 1
            elif int(viewing) <= 0:
                viewing = 1
            query = "SELECT " + database + ".sender, " + database + ".stream, " + database + ".time, " + database + ".text, ROW_NUMBER() over (ORDER BY " + database + ".sender) AS order FROM " + database + " WHERE " + database + ".order = " + str(viewing)
            subprocess.call(["./db", query])
            with open("temp.txt", 'r') as post:
                for curLine in post:
                    if k == 0:
                        print("Sender: ", end = "")
                    elif k == 1:
                        print("Stream: ", end = "")
                    elif k == 2:
                        print("Date: ", end = "")
                    print(curLine)
                    print("<BR>")
                    k += 1
        elif singleStream == 0: #Viewing all streams sorted by name
            k = 0
        #if viewing in name: #This post does exist
            #viewing = int(viewing)
        #elif int(viewing) <= 0: #They need to see the first post
            #viewing = 1 
        #else: #It is too high (out of range)
            #viewing = int(viewing) - 1
        #lines = name[viewing]
        #for i in lines:
            #print(i)
            #if "<br />" not in i:
                #print("<BR>")
