OBJ = main.o
CFLAGS = -Wall -pedantic -std=c99 -D _XOPEN_SOURCE

all: bin

bin: $(OBJ)
	gcc $(CFLAGS) $(OBJ) -o bin -lncurses

%.o : %.c
	gcc $(CFLAGS) -c $<

clean:
	rm bin $(OBJ)

rebuild: clean all
