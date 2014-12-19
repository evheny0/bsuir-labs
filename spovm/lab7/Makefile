OBJ = main.o file.o slfs.o dir.o utils.o graphics.o file_manager.o
CFLAGS = -Wall -pedantic -std=c++11

all: $(OBJ)
	g++ $(CFLAGS) $(OBJ) -o bin -lncurses

%.o : %.cpp
	g++ $(CFLAGS) -c $<

clean:
	rm bin $(OBJ)

rebuild: clean all
