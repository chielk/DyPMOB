CC=g++
CFLAGS=-O3 -Wall

simulation: simulation.cpp channel.o agent.o
	$(CC) $(CFLAGS) *.o -o simulation simulation.cpp

channel.o: channel.cpp channel.h
	$(CC) $(CFLAGS) -c -o channel.o channel.cpp

agent.o: agent.cpp agent.h
	$(CC) $(CFLAGS) -c -o agent.o agent.cpp

default:
	simulation

clean:
	rm -f simulation *.o
