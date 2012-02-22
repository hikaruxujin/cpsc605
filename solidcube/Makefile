
ifeq ("$(shell whoami)", "hikaru")
  CXX = clang
else
  CXX = gcc
endif

# Warnings frequently signal eventual errors:
CXXFLAGS= #-g -W -Wall -Weffc++ -Wextra -pedantic -O0 

ifeq ("$(shell uname)", "Darwin")
  LDFLAGS =
else
  ifeq ("$(shell uname)", "Linux")
  LDFLAGS = -lglut -lGL -lGLU -lm -lX11 -lXmu
  endif
endif

OBJS = \solidcube.o
	

EXEC = run

# $< refers to the first dependency
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# $@ refers to the target
$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS)


solidcube.o: solidcube.c
#cube.o: cube.c

clean:
	rm -rf $(OBJS)
	rm -rf $(EXEC)
