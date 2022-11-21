#Makefile

TARGET=ex9
CXXfLAGS=-W2
OBJECTS=ex9.o

$(TARGET): ex9.o
	gcc $(CXXFLAGS) -o $@ $^
ex9.o: ex9.c
	gcc $(CXXFLAGS) -c $^
clean:
	rm $(OBJECTS) $(TARGET)

