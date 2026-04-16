CC = gcc
RM = del /f /q

CFLAGS = -IE:/projects/libs/pdcurses/mod4.5.4/ -Wall -Wextra -Werror -std=c99 -pedantic -O2
LDFLAGS = E:/projects/libs/pdcurses/mod4.5.4/wingui/pdcurses.a -lgdi32 -lcomdlg32 -lwinmm -Wl,--gc-sections -s -mwindows

BIN = 15puzzle.exe

SOURCES = $(wildcard *.c)
OBJS = $(patsubst %.c, %.o, $(SOURCES))

.PHONY: all clean rebuild

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) -o $(BIN) $^ $(LDFLAGS)

clean:
	$(RM) $(BIN) $(OBJS)

rebuild: clean all
