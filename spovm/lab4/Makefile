OBJ_NIX = main.o threads_nix.o
OBJ_WIN = main.o threads_win.o
CFLAGS = -Wall -pedantic -std=c99 -D _XOPEN_SOURCE=500

all: nix

win: $(OBJ_WIN)
	gcc $(CFLAGS) $(OBJ_WIN) -o bin

nix: $(OBJ_NIX)
	gcc $(CFLAGS) $(OBJ_NIX) -o bin -lncurses -lpthread

%.o : %.c
	gcc $(CFLAGS) -c $<

clean:
	rm bin $(OBJ)

rebuild: clean all
