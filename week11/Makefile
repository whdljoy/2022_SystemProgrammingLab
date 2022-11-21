#Makefile

TARGET=ex11
CXXfLAGS=-W2
OBJECTS=ex11.o

$(TARGET): ex11.o
	gcc $(CXXFLAGS) -o $@ $^ -lpthread
ex11.o: ex11.c
	gcc $(CXXFLAGS) -c $^
clean:
	rm $(OBJECTS) $(TARGET)

