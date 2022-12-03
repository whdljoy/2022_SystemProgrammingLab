#Makefile

all : pa2 client
CXXfLAGS=-W2
OBJECTS=pa2.o
OBJECTS2=client.o

pa2: pa2.o
	gcc $(CXXFLAGS) -o $@ $^ -lpthread
pa2.o: pa2.c
	gcc $(CXXFLAGS) -c $^

client: client_sim-1.o
	gcc $(CXXFLAGS) -o $@ $^	
client_sim-1.o: client_sim-1.c
	gcc $(CXXFLAGS) -c $^	
clean:
	rm $(OBJECTS) $(OBJECTS2) pa2 client

