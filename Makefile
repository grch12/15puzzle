# Usage:
#   make [CURSES_INC=<PATH>] [CURSES_LIB=<PATH>]
#        [all|clean|rebuild]
#
# On Unix and Cygwin/MSYS, it assumes that pkg-config is available.
# If not, specify CURSES_INC and CURSES_LIB manually.

# Configure your compiler and flags here
# The default ones are for Windows with MinGW
# If you're on Unix, you can keep CFLAGS unchanged and leave LDFLAGS empty

CC = gcc

CFLAGS = $(CURSES_INC) -Wall -Wextra -Werror -std=c99 -pedantic -O2
LDFLAGS = $(CURSES_LIB) -lgdi32 -lcomdlg32 -lwinmm -Wl,--gc-sections -s -mwindows

# Do not edit below

PROJECT_NAME = 15puzzle

UNAME_S := $(shell uname -s 2>/dev/null)
PKGCONFIG_AVAILABLE = 0

ifeq ($(OS),Windows_NT)
	RM = cmd /c del /f /q
	ifneq (,$(findstring CYGWIN,$(UNAME_S)))
		PKGCONFIG_AVAILABLE = 1
		RM = rm -f
		CFLAGS += -DUSE_NCURSES
	endif
	ifneq (,$(findstring MSYS,$(UNAME_S)))
		PKGCONFIG_AVAILABLE = 1
		RM = rm -f
		CFLAGS += -DUSE_NCURSES
	endif
else
	RM = rm -f
	PKGCONFIG_AVAILABLE = 1
	CFLAGS += -DUSE_NCURSES
endif

ifeq ($(PKGCONFIG_AVAILABLE),1)
	ifneq (,$(CURSES_INC))
		CURSES_INC = $(shell pkg-config --cflags ncurses)
		CURSES_LIB = $(shell pkg-config --libs ncurses)
	endif
endif

ifeq ($(OS),Windows_NT)
	BIN = $(PROJECT_NAME).exe
else
	BIN = $(PROJECT_NAME)
endif

SOURCES = $(wildcard *.c)
OBJS = $(patsubst %.c, %.o, $(SOURCES))

.PHONY: all clean rebuild

all: $(BIN)

$(BIN): $(OBJS) $(ERR)
	$(CC) -o $(BIN) $^ $(LDFLAGS)

clean:
	$(RM) $(BIN) $(OBJS)

rebuild: clean all
