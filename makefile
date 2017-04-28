CC=gcc

CFLAGS=-c -Wall -std=c99 
LFLAGS=-lmingw32 -lSDL2main -lSDL2 -lpthread -lXinput -lm -lSDL2_gfx


HEADONE=ss_config.h ss_parallel_helpers.h ss_constants.h ss_ps3_constants.h
HEADTWO=ss_shape.h ss_menu.h ss_gc_constants.h ss_gamecontroller.h ss_stats.h 
HEADTHR=ss_canvas_color.h ss_canvas.h ss_ps3_canvas.h ss_ps3_statcolors.h
HEADFOR=ss_statprocessor.h
HEADERS=$(HEADONE) $(HEADTWO) $(HEADTHR) $(HEADFOR)

OBJONE=ss_ps3_constants.o ss_parallel_helpers.o ss_gc_constants.o
OBJTWO=ss_gamecontroller.o ss_stats.o ss_canvas_color.o ss_canvas.o
OBJTHR=ss_ps3_canvas.o ss_ps3_statcolors.o ss_statprocessor.o ss_menu.o 
OBJFOR=ss_runner.o
OBJECTS=$(OBJONE) $(OBJTWO) $(OBJTHR) $(OBJFOR)

EXECUTABLE=startselect.exe

#----------------------------

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXECUTABLE) $(LFLAGS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -rf *.o $(EXECUTABLE)
