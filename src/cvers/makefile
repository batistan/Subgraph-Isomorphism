CC=g++
CFLAGS=-Wall -std=c++11
LDFLAGS=
TGT=main

all: main.o graph.o isomorphism.o 
	$(CC) -o $(TGT) main.o graph.o isomorphism.o $(LDFLAGS)

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

graph.o: graph.cpp graph.h
	$(CC) $(CFLAGS) -c graph.cpp

isomorphism.o: isomorphism.cpp isomorphism.h
	$(CC) $(CFLAGS) -c isomorphism.cpp

debug: main.cpp graph.cpp graph.h isomorphism.cpp isomorphism.h
	$(CC) $(CFLAGS) -c -g graph.cpp
	$(CC) $(CFLAGS) -c -g isomorphism.cpp
	$(CC) $(CFLAGS) -c -g main.cpp
	$(CC) -o $(TGT) main.o graph.o isomorphism.o $(LDFLAGS)

clean:
	rm *.o

# also removes the executable
# might come in handy
cleanall:
	rm *.o
	rm $(TGT)
