#Makefile

TARGET=minishell
CXXfLAGS=-W2
OBJECTS=minishell.o

$(TARGET): minishell.o
	gcc $(CXXFLAGS) -o $@ $^
minishell.o: minishell.c
	gcc $(CXXFLAGS) -c $^
clean:
	rm $(OBJECTS) $(TARGET)

