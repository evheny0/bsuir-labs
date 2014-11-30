OBJ = main.o collector.o pointer.o memory_info.o
CFLAGS = -Wall -pedantic -D _LOG_TO_NCURSES=1

all: $(OBJ)
	g++ $(CFLAGS) $(OBJ) -o bin -lncurses

%.o : %.cpp
	g++ $(CFLAGS) -c $<

clean:
	rm bin $(OBJ)

rebuild: clean all
