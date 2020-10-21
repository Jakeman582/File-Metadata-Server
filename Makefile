CFLAGS = -Wall -g
LDLIBS = -lpthread

SERVER = filed
CLIENTS = newKey fileGet fileDigest fileRun
CSAPP = csapp.h csapp.c
FILE_SERVER = file_server.h file_server.c

all: $(CSAPP) ${FILE_SERVER} $(SERVER) $(CLIENTS)

csapp.h:
	wget http://csapp.cs.cmu.edu/2e/ics2/code/include/csapp.h

csapp.c:
	wget http://csapp.cs.cmu.edu/2e/ics2/code/src/csapp.c

csapp.o: csapp.h csapp.c

file_server.o: file_server.h file_server.c

$(SERVER): csapp.o file_server.o
$(CLIENTS): csapp.o file_server.o

.PHONY: clean
clean:
	/bin/rm -rf csapp.h csapp.c file_server.h file_server.c *.o filed newKey fileGet fileDigest fileRun

zip:
	zip project5.zip *.c README
