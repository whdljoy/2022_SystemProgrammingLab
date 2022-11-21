#Makefile

TARGET=ex13
CXXfLAGS=-W2
OBJECTS=ex13.o

$(TARGET): ex13.o
	gcc $(CXXFLAGS) -o $@ $^ -lpthread
ex13.o: ex13.c
	gcc $(CXXFLAGS) -c $^
clean:
	rm $(OBJECTS) $(TARGET)

