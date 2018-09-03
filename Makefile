CC = g++
CFLAGS = -Wall -std=c++1z
DEPS = termOps.h dir.h commandMode.h
OBJ = termOps.o dir.o commandMode.o fileXplorer.o 
%.o:%.cpp $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<
	
fileXplorer: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^
 
