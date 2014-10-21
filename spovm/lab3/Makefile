OBJ_PARENT = main.o
CFLAGS = -Wall -pedantic -std=c99 -D _XOPEN_SOURCE

all: bin

bin: $(OBJ_PARENT)
	gcc $(CFLAGS) $(OBJ_PARENT) -o bin -lncurses

%.o : %.c
	gcc $(CFLAGS) -c $<

clean:
	rm bin child $(OBJ_PARENT)

rebuild: clean all
