CC = g++
CFLAGS = -std=c++11
EXEC = AnagramGenerator
	
all:
	rm -f $(EXEC)
	$(CC) $(CFLAGS) -o $(EXEC) main.cpp AnagramGenerator.cpp
