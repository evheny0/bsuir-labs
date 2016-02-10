OBJ = main.o
CFLAGS = -Wall -pedantic -msse3 -mfpmath=sse -fopenmp  -mavx
CFLAGS_NO_OPT = -fno-tree-vectorize
OPTIMIZE_FLAGS = -O3 -funroll-loops -ftree-vectorize -ftree-vectorizer-verbose=0 -ffast-math

# CPPFLAGS = -O3 -msse3 -mavx -ftree-vectorize -ftree-vectorizer-verbose=0
CPPFLAGS = -msse3 -mavx

all: nix

nix: $(OBJ)
	g++ $(CFLAGS) $(OPTIMIZE_FLAGS) $(OBJ) -o bin

%.o : %.c
	g++ $(CFLAGS) -c $< $(CPPFLAGS)

clean:
	rm bin $(OBJ)

rebuild: clean all
