#Makefile

TARGET=signals
CXXfLAGS=-W2
OBJECTS=signals.o

$(TARGET): signals.o
	gcc $(CXXFLAGS) -o $@ $^
signals.o: signals.c
	gcc $(CXXFLAGS) -c $^
clean:
	rm $(OBJECTS) $(TARGET)

