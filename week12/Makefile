#Makefile

all : server client
CXXfLAGS=-W2
OBJECTS=server.o
OBJECTS2=client.o

server: server.o
	gcc $(CXXFLAGS) -o $@ $^	
server.o: server.c
	gcc $(CXXFLAGS) -c $^

client: client.o
	gcc $(CXXFLAGS) -o $@ $^	
client.o: client.c
	gcc $(CXXFLAGS) -c $^	
clean:
	rm $(OBJECTS) $(OBJECTS2) server client

