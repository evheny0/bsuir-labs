OBJ_PARENT = main.o
OBJ_CHILD = child.o
CFLAGS = -Wall

all: bin child

bin: $(OBJ_PARENT)
	gcc $(CFLAGS) $(OBJ_PARENT) -o bin -lncurses

child: $(OBJ_CHILD)
	gcc $(CFLAGS) $(OBJ_CHILD) -o child -lncurses

%.o : %.c
	gcc $(CFLAGS) -c $<

clean:
	rm bin child $(OBJ_PARENT) $(OBJ_CHILD)

rebuild: clean all
