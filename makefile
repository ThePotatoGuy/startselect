CC=gcc

CFLAGS=-c -Wall -std=c99
LFLAGS=-lmingw32 -lSDL2main -lSDL2

HEADERS=

OBJECTS=runner.o

EXECUTABLE=startselect.exe

#----------------------------

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXECUTABLE) $(LFLAGS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -rf *.o $(EXECUTABLE)
