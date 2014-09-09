OBJ_PARENT = main.o
OBJ_CHILD = child.o
CFLAGS = -Wall -std=c99

all: bin child

bin: $(OBJ_PARENT)
	gcc $(CFLAGS) $(OBJ_PARENT) -o bin

child: $(OBJ_CHILD)
	gcc $(CFLAGS) $(OBJ_CHILD) -o child

%.o : %.c
	gcc $(CFLAGS) -c $<

clean:
	rm bin child $(OBJ_PARENT) $(OBJ_CHILD)
