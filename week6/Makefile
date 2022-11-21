#Makefile

TARGET=cron
CXXfLAGS=-W2
OBJECTS=cron.o

$(TARGET): cron.o
	gcc $(CXXFLAGS) -o $@ $^
cron.o: cron.c
	gcc $(CXXFLAGS) -c $^
clean:
	rm $(OBJECTS) $(TARGET)

