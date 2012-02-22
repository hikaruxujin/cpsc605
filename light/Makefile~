
#CXX = g++
CXX = clang

# Warnings frequently signal eventual errors:
CXXFLAGS= -g -W -Wall -Weffc++ -Wextra -pedantic -O0 

ifeq ("$(shell uname)", "Darwin")
  LDFLAGS =
else
  ifeq ("$(shell uname)", "Linux")
  LDFLAGS = -lglut -lGL -lGLU -lm -lX11 -lXmu
  endif
endif

OBJS = \robot.o
	

EXEC = run

# $< refers to the first dependency
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# $@ refers to the target
$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS)

robot.o: robot.c

clean:
	rm -rf $(OBJS)
	rm -rf $(EXEC)
