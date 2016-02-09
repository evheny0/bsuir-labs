OBJ = main.o
CFLAGS = -Wall -pedantic -msse4 -mfpmath=sse -fopenmp -O3 -funroll-loops -ftree-vectorize
CFLAGS_NO_OPT = -fno-tree-vectorize

all: nix

nix: $(OBJ)
	g++ $(CFLAGS) $(OBJ) -o bin

%.o : %.c
	g++ $(CFLAGS) -c $<

clean:
	rm bin $(OBJ)

rebuild: clean all
