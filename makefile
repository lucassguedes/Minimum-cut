CXX = g++
CXX_BIN = /usr/bin/g++

CXXFLAGS = -Wall -fPIC -std=c++11 -O3 -DNDEBUG -Iinclude -I. -DARCH_K8 -Wno-deprecated -DUSE_CBC -DUSE_CLP -DUSE_BOP -DUSE_GLOP

BINDIR := bin
OBJDIR := obj
SRCDIR := src
SOURCES := $(wildcard $(SRCDIR)/*.cpp)
OBJECTS := $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SOURCES))

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@echo "\033[32mCompiling \033[35m$*.cpp"
	@$(CXX) -c -o $@ $< $(CXXFLAGS)


$(BINDIR)/Mincut: $(OBJECTS)
	@echo "\033[32mLinking $@"
	@$(CXX) -o $@ $^ $(CXXFLAGS)

clean:
	rm -rf $(OBJDIR)/*.o $(BINDIR)/Mincut 