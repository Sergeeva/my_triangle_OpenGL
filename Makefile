BIN = my_triangle

OBJ = glad.o my_triangle.o 

CC = g++ 

CFLAGS = -g -Wall -MMD


CC_DEFINES	+=  -D_TECH 

all: $(BIN)

$(BIN): $(OBJ) 
	$(CC) $(CFLAGS) $(CC_DEFINES)  $(OBJ) -o $(BIN) -lm -lglfw -lX11 -lGL -lpthread -lXrandr  -ldl   

include $(wildcard *.d)

.PHONY : clean

clean:
	rm -f $(BIN) *.o *.d
	rm -f *.dat *.log *.txt  
	rm -f *~ \#*\# *.cache log

