CC = gcc
RM = del /f /q

CFLAGS = -IE:/projects/libs/pdcurses/mod4.5.4/ -Wall -Wextra -Werror -std=c99 -pedantic -O2 -finput-charset=utf-8 -fexec-charset=gbk
LDFLAGS = E:/projects/libs/pdcurses/mod4.5.4/wingui/pdcurses.a -lgdi32 -lcomdlg32 -lwinmm -Wl,--gc-sections -s -mwindows

BIN = 15puzzle.exe

.PHONY: all clean rebuild

all: $(BIN)

$(BIN): main.o
	$(CC) -o $(BIN) main.o $(LDFLAGS)

clean:
	$(RM) $(BIN) main.o

rebuild: clean all
