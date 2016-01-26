OBJ = main.o basic_socket_handler.o client.o server.o socket.o utils.o transmission_rater.o client_connection_state.o package.o udp_client.o
CFLAGS = -Wall -pedantic -std=c++11 -std=gnu++0x
CPPFLAGS = -std=c++11 

all: nix

nix: $(OBJ)
	g++ $(CFLAGS) $(OBJ) -o bin

%.o : %.c
	g++ -c $< $(CFLAGS) $(CPPFLAGS)

clean:
	rm bin $(OBJ)

rebuild: clean all
