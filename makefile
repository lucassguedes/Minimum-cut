src = $(wildcard src/*.cpp)
obj = $(src:.cpp=.o)

CXXFLAGS = -std=c++11 -I include/ -g

Mincut: $(obj)
	$(CXX) -o $@ $^ $(CXXFLAGS)
clean:
	rm -rf *o TSP