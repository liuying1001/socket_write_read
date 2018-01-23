all : server client

server : sk_server.o sk_comm.o
	cc -o server sk_server.o sk_comm.o

client : sk_client.o sk_comm.o
	cc -o client sk_client.o sk_comm.o

sk_server.o : sk_comm.h
sk_client.o : sk_comm.h
sk_comm.o : sk_comm.h

clean :
	rm  server
	rm  client
	rm  *.o
