#Makefile

TARGET=ex8
CXXfLAGS=-W2
OBJECTS=ex8.o

$(TARGET): ex8.o
	gcc $(CXXFLAGS) -o $@ $^
ex8.o: ex8.c
	gcc $(CXXFLAGS) -c $^
clean:
	rm $(OBJECTS) $(TARGET)

