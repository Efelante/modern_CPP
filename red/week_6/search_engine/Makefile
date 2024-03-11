INCLUDE_PATHS = -I../../../include
CXXFLAGS += -std=c++1z

all:
	$(CXX) *.cpp $(CXXFLAGS) $(INCLUDE_PATHS) -o a.out

tar: clean
	tar cf search_server.tar ./*.cpp ./*.h
	

clean:
	-rm a.out
	-rm search_server.tar

run:
	./a.out	
