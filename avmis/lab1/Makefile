OBJ = main.o
CFLAGS = -Wall -pedantic -msse4 -mfpmath=sse -fopenmp  -mavx
CFLAGS_NO_OPT = -fno-tree-vectorize
OPTIMIZE_FLAGS = -O3 -funroll-loops -ftree-vectorize -ftree-vectorizer-verbose=0 -ffast-math

CPPFLAGS = -O3 -msse4 -mavx -ftree-vectorize -ftree-vectorizer-verbose=0 -fopenmp
# CPPFLAGS = -msse4 -mavx -fno-tree-vectorize

all: nix

nix: $(OBJ)
	g++ $(CFLAGS) $(OPTIMIZE_FLAGS) $(OBJ) -o bin

%.o : %.c
	g++ $(CFLAGS) -c $< $(CPPFLAGS)

clean:
	rm bin $(OBJ)

rebuild: clean all
