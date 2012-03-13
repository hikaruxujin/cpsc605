
CXX = gcc
#CXX = clang

# Warnings frequently signal eventual errors:

ifeq ("$(shell uname)", "Darwin")
  LDFLAGS =
else
  ifeq ("$(shell uname)", "Linux")
  LDFLAGS = -lglut -lGL -lGLU -lm -lX11 -lXmu
  endif
endif

OBJS = \main.o
	

EXEC = run

# $< refers to the first dependency
%.o: %.c
	$(CXX) $(CXXFLAGS) -c $< -o $@

# $@ refers to the target
$(EXEC): $(OBJS)
	$(CXX) -o $@ $(OBJS) $(LDFLAGS)

main.o: main.c

clean:
	rm -rf $(OBJS)
	rm -rf $(EXEC)
