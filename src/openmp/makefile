CC=g++
CFLAGS=-Wall -std=c++11
LDFLAGS= -fopenmp
TGT=main

all: main.o graph.o isomorphism.o
	$(CC) -g -o $(TGT) main.o graph.o isomorphism.o $(CFLAGS) $(LDFLAGS)

main.o: main.cpp
	$(CC) -c main.cpp

graph.o: graph.cpp graph.h
	$(CC) -c graph.cpp

isomorphism.o: isomorphism.cpp isomorphism.h
	$(CC) -c isomorphism.cpp

clean:
	rm *.o

# also removes the executable
# might come in handy
cleanall:
	rm *.o
	rm $(TGT)
