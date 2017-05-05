CC=gcc

CFLAGS=-c -Wall -std=c99 
LFLAGS=-lmingw32 -lSDL2main -lSDL2 -lXinput -lm -lSDL2_gfx 
LFLAGS2=-lcomdlg32 -lole32 -static-libgcc -Wl,-Bstatic -lpthread -Wl,-Bdynamic
#nfd.lib -lcomctl32 


HEADONE=ss_config.h ss_parallel_helpers.h ss_constants.h ss_ps3_constants.h
HEADTWO=ss_window_constants.h ss_shape.h ss_menu.h ss_gc_constants.h 
HEADTHR=ss_gamecontroller.h ss_stats.h ss_canvas_color.h ss_canvas.h 
HEADFOR=ss_ps3_canvas.h ss_ps3_statcolors.h ss_statprocessor.h 
HEADFIV=tinyfiledialogs.h
HEADERS=$(HEADONE) $(HEADTWO) $(HEADTHR) $(HEADFOR) $(HEADFIV)

OBJONE=ss_ps3_constants.o ss_parallel_helpers.o ss_window_constants.o 
OBJTWO=ss_gc_constants.o ss_gamecontroller.o ss_stats.o ss_canvas_color.o 
OBJTHR=ss_canvas.o ss_ps3_canvas.o ss_ps3_statcolors.o ss_statprocessor.o 
OBJFOR=tinyfiledialogs.o ss_menu.o ss_runner.o
OBJECTS=$(OBJONE) $(OBJTWO) $(OBJTHR) $(OBJFOR)

EXECUTABLE=startselect.exe

#----------------------------

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXECUTABLE) $(LFLAGS) $(LFLAGS2)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -rf *.o $(EXECUTABLE)
