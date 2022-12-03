#Makefile

TARGET=pa2
CXXfLAGS=-W2
OBJECTS=pa2.o

$(TARGET): pa2.o
	gcc $(CXXFLAGS) -o $@ $^ -lpthread
pa2.o: pa2.c
	gcc $(CXXFLAGS) -c $^
clean:
	rm $(OBJECTS) $(TARGET)

