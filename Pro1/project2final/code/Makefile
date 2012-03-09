
CXX = g++
#CXX = clang

# Warnings frequently signal eventual errors:
CXXFLAGS= -g -W -Wall -Wextra -pedantic -O0 

LDFLAGS = -lglut -lGL -lGLU -lm -lX11 -lXmu


OBJS = pro2.o
	

EXEC = run

# $< refers to the first dependency
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# $@ refers to the target
$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS)

pro2.o: pro2.cpp

clean:
	rm -rf $(OBJS)
	rm -rf $(EXEC)
