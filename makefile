CC=gcc

CFLAGS=-c -Wall -std=c99 -g 
LFLAGS=-lmingw32 -lSDL2main -lSDL2 -lpthread -lXinput -lm -lSDL2_gfx


HEADONE=ss_config.h ss_parallel_helpers.h ss_constants.h ss_ps3_constants.h
HEADTWO=ss_shape.h ss_menu.h ss_gamecontroller.h ss_stats.h ss_canvas.h
HEADERS=$(HEADONE) $(HEADTWO)

OBJONE=ss_ps3_constants.o ss_parallel_helpers.o ss_gamecontroller.o
OBJTWO=ss_menu.o ss_stats.o ss_canvas.o ss_runner.o
OBJECTS=$(OBJONE) $(OBJTWO)

EXECUTABLE=startselect.exe

#----------------------------

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXECUTABLE) $(LFLAGS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -rf *.o $(EXECUTABLE)
