CC=gcc

CFLAGS=-c -Wall -std=c99 -g 
LFLAGS=-lmingw32 -lSDL2main -lSDL2 -lpthread -lXinput -lm


HEADONE=ss_config.h ss_parallel_helpers.h ss_constants.h ss_menu.h
HEADTWO=ss_gamecontroller.h
HEADERS=$(HEADONE) $(HEADTWO)

OBJECTS=ss_parallel_helpers.o ss_gamecontroller.o ss_menu.o ss_runner.o

EXECUTABLE=startselect.exe

#----------------------------

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXECUTABLE) $(LFLAGS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -rf *.o $(EXECUTABLE)
