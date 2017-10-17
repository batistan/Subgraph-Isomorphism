CC=g++
CFLAGS=-Wall
LDFLAGS=
TGT=main

all: main.o graph.o isomorphism.o 
	$(CC) -g -o $(TGT) main.o graph.o isomorphism.o $(CFLAGS) $(LDFLAGS)

main.o:
	$(CC) -c main.cpp

graph.o:
	$(CC) -c graph.cpp

isomorphism.o:
	$(CC) -c isomorphism.cpp

clean:
	rm *.o

# also removes the executable
# might come in handy
cleanall:
	rm *.o
	rm $(TGT)
