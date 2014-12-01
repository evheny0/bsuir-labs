OBJ_NIX = main.o threads_nix.o
OBJ_NIX_SHARED = file_io_nix.o
OBJ_WIN = main.o threads_win.o
OBJ_WIN_SHARED = file_io_win.o
CFLAGS = -Wall -pedantic -std=c99 -D _XOPEN_SOURCE=500
SHARED_CFLAGS = -shared -Wall -pedantic -std=c99

all: nix

win: $(OBJ_WIN)
	gcc $(CFLAGS) $(OBJ_WIN) -o bin

nix: $(OBJ_NIX)
	gcc $(CFLAGS) $(OBJ_NIX) -o bin -lncurses -lpthread -lrt -ldl
	gcc $(SHARED_CFLAGS) $(OBJ_NIX_SHARED) -o faio.so -lrt -ldl

%.o : %.c
	gcc $(CFLAGS) -c $<

clean:
	rm bin $(OBJ) $(OBJ_NIX)

rebuild: clean all
