#Macros
CFLAGS = -g -ansi -Wall
LIBS = -L. -lstream
SQL = -lmysqlclient -L/usr/lib/x86_64-linux-gnu/

#All programs/commands
all: lib convert post addauthor view pageRead db

#    A1    #
convert: A1.o newCode.o newCode2.o newCode.h newCode2.h
	gcc $(CFLAGS) A1.o newCode.o newCode2.o -o convert

post: convert lib stream.h
	./convert post.cc
	gcc $(CFLAGS) post.c -o post $(LIBS) $(SQL)
	rm convert

newCode.o: newCode.c newCode.h
	gcc $(CFLAGS) -c newCode.c

newCode2.o: newCode2.c newCode2.h
	gcc $(CFLAGS) -c newCode2.c

A1.o: A1.c newCode2.h newCode.h
	gcc -c $(CFLAGS) A1.c
############

#  Add Author  #
addauthor: addauthor.o lib
	gcc $(CFLAGS) addauthor.o -o addauthor $(LIBS) $(SQL)

addauthor.o: addauthor.c
	gcc $(CFLAGS) -c addauthor.c
################

#   Library   #
lib: stream.o
	ar cr libstream.a stream.o

stream.o: stream.c stream.h
	gcc -c $(CFLAGS) stream.c -o stream.o $(SQL)
###############

#   View   #
view: view.py streams.py postNum.py
	chmod +x view.py
	chmod +x streams.py
	chmod +x postNum.py
############

# Page Reader for HTML #
pageRead: A3.o elements.o
	gcc $(CFLAGS) A3.o elements.o -o pageRead

A3.o: A3.c
	gcc $(CFLAGS) -c A3.c

elements.o: elements.c elements.h
	gcc $(CFLAGS) -c elements.c
########################

# Database reset program #
db: db.o
	gcc $(CFLAGS) db.o -o db $(SQL)

db.o: db.c
	gcc $(CFLAGS) db.c -c -o db.o
##########################

clean:
	rm *.o
