CFLAGS=-std=c99 -ggdb -DDEBUG
CFLAGS += -D_GNU_SOURCE
CFLAGS += -D_POSIX_C_SOURCE=200809L
CFLAGS += -Wall -Werror -Wpedantic -Wuninitialized -Wmaybe-uninitialized -Winit-self -Wunused-result -Wunused
CFLAGS += -MD -MP
CFLAGS += -pthread -fPIC
CFLAGS += `sdl2-config --cflags`
# CFLAGS += `pkg-config --cflags cairo poppler-glib pangocairo`
# LFLAGS= -lm -lreadline -lgc -lSDL2_net -lSDL2_image -lSDL2_mixer -lavutil -lavformat -lavcodec
LFLAGS= -lm -lavutil -lavformat -lavcodec -lportaudio -pthread
LFLAGS += `sdl2-config --libs`

# SRC = $(wildcard *.c)
SRC = $(filter-out main.c, $(wildcard *.c))
# P = main.c

all: tutplayer libtutplayer.so Makefile linecount

tutplayer: libtutplayer.so
	gcc -L. -ltutplayer -Wl,-rpath=. -o tutplayer main.c

libtutplayer.so: $(SRC:%.c=%.o)
	cc $(LFLAGS) --shared -o $@ $^

clean:
	rm -f *.o *.d libtutplayer tutplayer

linecount:
	@echo "linecount:" `cat *.c *.h | wc -l`

.PHONY: all clean linecount

-include .dummy
.dummy: Makefile
	@touch $@
	@$(MAKE) -s clean

-include $(SRC:%.c=%.d)
