OBJ = main.o file.o slfs.o dir.o utils.o
CFLAGS = -Wall -pedantic -std=c++11

all: $(OBJ)
	g++ $(CFLAGS) $(OBJ) -o bin -lncurses

%.o : %.cpp
	g++ $(CFLAGS) -c $<

clean:
	rm bin $(OBJ)

rebuild: clean all
