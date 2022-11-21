#Makefile

TARGET=week5
CXXfLAGS=-W2
OBJECTS=week5.o

$(TARGET): week5.o
	gcc $(CXXFLAGS) -o $@ $^
week5.o: week5.c
	gcc $(CXXFLAGS) -c $^
clean:
	rm $(OBJECTS) $(TARGET)

