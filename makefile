CXX = g++
CXXFLAGS = -std=c++11
EXEC = bin/AnagramGenerator
	
all:
	rm -f $(EXEC)
	$(CXX) $(CXXFLAGS) -o $(EXEC) src/main.cpp src/AnagramGenerator.cpp

clean:
	$(RM) -r $(EXEC) *.o *~
