all: server client1 client2

server:
	gcc -w server.c -o server.o

client1:
	gcc -w client1.c -o client1.o

client2:
	gcc -w client2.c -o client2.o

clean:
	rm -f *.o
