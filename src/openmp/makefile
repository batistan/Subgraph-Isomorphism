CC=g++
CFLAGS=-Wall -std=c++11
LDFLAGS=-fopenmp
TGT=main
PTGT=perf

all: main.o graph.o isomorphism.o 
	$(CC) -o $(TGT) main.o graph.o isomorphism.o $(LDFLAGS)

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

graph.o: graph.cpp graph.h
	$(CC) $(CFLAGS) -c graph.cpp

isomorphism.o: isomorphism.cpp isomorphism.h
	$(CC) $(CFLAGS) $(LDFLAGS) -c isomorphism.cpp

perf: perf.cpp graph.o isomorphism.o
	$(CC) $(CFLAGS) -c perf.cpp
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(PTGT) perf.o graph.o isomorphism.o

clean:
	rm *.o

# also removes the executable
# might come in handy
cleanall:
	rm *.o
	rm $(TGT)
