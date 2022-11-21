#Makefile

TARGET=main
CXX=gcc
CXXfLAGS=-W2
OBJECTS=plus.o minus.o main.o
$(TARGET): $(OBJECTS)
		$(CXX) $(CXXFLAGS) -o $@ $^
plus.o: plus.c
		$(CXX) $(CXXFLAGS) -c $^
minus.o: minus.c
		$(CXX) $(CXXFLAGS) -c $^
main.o: main.c
		$(CXX) $(CXXFLAGS) -c $^
clean:
		rm  $(OBJECTS) $(TARGET)
