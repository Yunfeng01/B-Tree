#DEBUG  = -g
CFLAGS = -Wall -Wextra -pedantic $(DEBUG) 
LFLAGS = -Wall -lm $(DEBUG)
# for Windows use del for UNIX use \rm -f
RM = del
INCS = BpTree.h
SRCS = BpTree.cpp
OBJS = $(SRCS:.cpp=.o)
EXEC = BpTree.exe

all: $(SRCS) $(EXEC)

# To make an object from source
#.cpp.o: 
%.o:%.cpp $(INCS)
	$(CXX) $(CFLAGS) -c $< -o $@

$(EXEC): $(OBJS) 
	$(CXX) $(LFLAGS) $(OBJS) -o $@

clean:
	del   *.o  $(EXEC)
	echo clean done


