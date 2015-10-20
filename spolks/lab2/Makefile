OBJ = main.o basic_socket_handler.o client.o server.o socket.o
CFLAGS = -Wall -pedantic

all: nix

nix: $(OBJ)
	g++ $(CFLAGS) $(OBJ) -o bin

%.o : %.c
	g++ $(CFLAGS) -c $<

clean:
	rm bin $(OBJ)

rebuild: clean all
