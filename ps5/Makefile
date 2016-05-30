CXXFLAGS = -std=c++11 -Wall -O0 -g
CXX = g++

OBJECTS = Hopscotch_wrapper.o hopscotch.o 

default: hopscotch-benchmark

hopscotch-benchmark: $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

Main.o: Hopscotch_wrapper.h hopscotch.hpp 

h.o: %.cc %.h Hashes.h

clean:
	rm -f run-tests *.o *~
