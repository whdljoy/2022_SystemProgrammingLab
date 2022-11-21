#Makefile

TARGET=ex4
CXXfLAGS=-W2
OBJECTS=ex4.o

$(TARGET): ex4.o
	gcc $(CXXFLAGS) -o $@ $^
ex4.o: ex4.c
	gcc $(CXXFLAGS) -c $^
clean:
	rm $(OBJECTS) $(TARGET)

