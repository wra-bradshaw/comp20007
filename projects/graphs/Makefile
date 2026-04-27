#
# created for COMP20007 Design of Algorithms 2017
# by Matt Farrugia <matt.farrugia@unimelb.edu.au>
#
# changed for COMP20007 Design of Algorithms 2020
# by Tobias Edwards <tobias.edwards@unimelb.edu.au>
#

# specifying the C Compiler and Compiler Flags for make to use
CC     = gcc
CFLAGS = -Wall -Wno-comment -std=c99

# exe name and a list of object files that make up the program
EXE    = main
OBJ    = main.o list.o graph.o graphalgs.o priorityqueue.o

$(EXE): $(OBJ)
	$(CC) $(CFLAGS) -o $(EXE) $(OBJ)

graphalgs.o: graphalgs.c graphalgs.h graph.h
	$(CC) $(CFLAGS) -c graphalgs.c

graph.o: graph.c graph.h list.h
	$(CC) $(CFLAGS) -c graph.c

list.o: list.c list.h
	$(CC) $(CFLAGS) -c list.c

priorityqueue.o: priorityqueue.c priorityqueue.h
	$(CC) $(CFLAGS) -c priorityqueue.c

clean:
	rm -f $(OBJ) $(EXE)
