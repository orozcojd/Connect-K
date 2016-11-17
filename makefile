PROG=botnetAI
CC=g++
CPPFLAGS=-std=c++11
OBJS=ConnectK.o AIShell.o Move.o

$(PROG): $(OBJS)
	$(CC) -o $(PROG) $(OBJS)

ConnectK.o:
	$(CC) $(CPPFLAGS) -c ConnectK.cpp
AIShell.o: AIShell.h
	$(CC) $(CPPFLAGS) -c AIShell.cpp
Move.o: Move.h
	$(CC) $(CPPFLAGS) -c Move.cpp

clean:
	rm -f core $(PROG) $(OBJS)