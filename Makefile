CXX=g++
CXXFLAGS=-g -std=c++11
LDFLAGS=-lsfml-system -lsfml-window -lsfml-graphics
SRCS=main.cpp renderer.cpp model.cpp
OBJS=$(SRCS:.cpp=.o)

%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $^ -o $@

viewer: $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(OBJS) -oviewer

clean:
	rm $(OBJS)
