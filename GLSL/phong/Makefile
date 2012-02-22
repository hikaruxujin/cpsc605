
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

OBJS = \phong.o
	

EXEC = run

# $< refers to the first dependency
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# $@ refers to the target
$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS)

#phong.o: phong.c

clean:
	rm -rf $(OBJS)
	rm -rf $(EXEC)
