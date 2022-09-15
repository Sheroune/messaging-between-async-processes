all: server client1

server:
	gcc -w server.c -o server.o

client1:
	gcc -w client1.c -o client1.o

clean:
	rm -f *.o
