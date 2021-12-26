INCLUDE_PATHS = -I../../../include
CXXFLAGS += -std=c++1z

all:
	$(CXX) *.cpp $(CXXFLAGS) $(INCLUDE_PATHS) -o a.out

run:
	cat words.txt | ./a.out

clean:
	rm a.out
