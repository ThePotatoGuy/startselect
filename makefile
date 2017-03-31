CC=gcc

CFLAGS=-c -Wall -std=c99 -g -O0
LFLAGS=-lmingw32 -lSDL2main -lSDL2 -lpthread

HEADERS=ss_config.h ss_parallel_helpers.h ss_constants.h ss_menu.h

OBJECTS=ss_parallel_helpers.o ss_menu.o ss_runner.o

EXECUTABLE=startselect.exe

#----------------------------

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXECUTABLE) $(LFLAGS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -rf *.o $(EXECUTABLE)
