OBJ = main.o threads_nix.o
CFLAGS = -Wall -pedantic -std=c99 -D _XOPEN_SOURCE=500

all: bin

bin: $(OBJ)
	gcc $(CFLAGS) $(OBJ) -o bin -lncurses -lpthread

%.o : %.c
	gcc $(CFLAGS) -c $<

clean:
	rm bin $(OBJ)

rebuild: clean all
