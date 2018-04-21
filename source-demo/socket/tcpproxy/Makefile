#by pafone 2011.02.21
all:server client
	echo "pafone make "
server: server.o
	gcc -o server server.o -lpthread
client: client.o
	gcc -o client client.o
%.o:%.c
	gcc -c $< -o $@
clean:
	rm -f server client *.o
