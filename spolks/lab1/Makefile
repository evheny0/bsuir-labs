OBJ = main.o soc_lib.o
CFLAGS = -Wall -pedantic

all: nix

nix: $(OBJ)
	g++ $(CFLAGS) $(OBJ) -o bin

%.o : %.c
	g++ $(CFLAGS) -c $<

clean:
	rm bin $(OBJ)

rebuild: clean all
