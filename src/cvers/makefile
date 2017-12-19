CC=g++
CFLAGS=-Wall -std=c++11
LDFLAGS=
TGT=main
PTGT=perf

all: main.o graph.o isomorphism.o 
	$(CC) -o $(TGT) main.o graph.o isomorphism.o $(LDFLAGS)

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

graph.o: graph.cpp graph.h
	$(CC) $(CFLAGS) -c graph.cpp

isomorphism.o: isomorphism.cpp isomorphism.h
	$(CC) $(CFLAGS) -c isomorphism.cpp

debug: main.cpp graph.cpp graph.h isomorphism.cpp isomorphism.h
	$(CC) $(CFLAGS) -O0 -c -g graph.cpp
	$(CC) $(CFLAGS) -O0 -c -g isomorphism.cpp
	$(CC) $(CFLAGS) -O0 -c -g main.cpp
	$(CC) -o $(TGT) -O0 main.o graph.o isomorphism.o $(LDFLAGS)

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
