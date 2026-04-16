#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#ifdef _POSIX
#include <ncurses.h>
#else
#include <curses.h>
#endif

#include "board.h"

int moves = 0;

typedef enum {DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT} Direction;

// Array of move functions, used to map directions to moves
bool (* move_map[])(void) = {move_up, move_down, move_left, move_right};

/**
 * Prints the 15-puzzle game board with borders and numbers.
 * The board is represented as a 4x4 grid of numbers, with
 * the empty space represented as 0.
 */
void print_grid(void) {
  int rows = 4;
  int cols = 4;

  for (int row = 0; row <= rows; row++) {
    for (int col = 0; col <= cols; col++) {
      if (row == 0) {
        addch(col == 0    ? ACS_ULCORNER :   // ┌
              col == cols ? ACS_URCORNER :   // ┐
                            ACS_TTEE);       // ┬
      } else if (row == rows) {
        addch(col == 0    ? ACS_LLCORNER :   // └
              col == cols ? ACS_LRCORNER :   // ┘
                            ACS_BTEE);       // ┴
      } else {
        addch(col == 0    ? ACS_LTEE :       // ├
              col == cols ? ACS_RTEE :       // ┤
                            ACS_PLUS);       // ┼
      }

      if (col < cols) {
        addch(ACS_HLINE);
        addch(ACS_HLINE);
      }
    }
    addch('\n');

    if (row < rows) {
      for (int col = 0; col <= cols; col++) {
        addch(ACS_VLINE);           // │
        if (col < cols) {
          int i = row * cols + col;
          int n = board[i];
          if (n != 0)
            printw("%2d", board[i]);
          else
            addstr("  ");
        }
      }
      addch('\n');
    }
  }
}

void print_game(void) {
  clear();
  print_grid();
  addstr("Use WASD or arrow keys to control, Q to quit, R to restart\n");
  printw("Moves: %d\n", moves);
}


/**
 * @brief Shuffle the game board with a random sequence of moves
 * @details This function performs a sequence of 1000 random moves on the
 *          game board, effectively shuffling the board. This is used to
 *          generate a random starting position for the game.
 */
void shuffle(void) {
  for (int i = 0; i < 1000; i++) {
    Direction dir = rand() % 4;
    move_map[dir]();
  }
}

/**
 * @brief Handles a key press event in the game loop
 *
 * This function is called when the user presses a key in the game loop.
 * It checks if the game is currently solved, and if not, it attempts to
 * make a move in the specified direction. If the move is successful, it
 * increments the move counter.
 *
 * @param dir The direction of the move to attempt
 * @param solved Whether the game is currently solved
 */
void keypress(Direction dir, bool solved) {
  if (!solved) {
    bool moved = move_map[dir]();
    if (moved) moves++;
  }
}

/**
 * @brief The main game loop
 *
 * This function contains the main game loop. It shuffles the board, then
 * enters a loop where it prints the game, checks if the game is solved, and
 * waits for user input to make a move. If the user wins, it prints a message
 * and waits for the user to either restart or quit.
 *
 * @return void
 */
void game_loop(void) {
  for (;;) {
    shuffle();
    moves = 0;
    for (;;) {
      print_game();
      bool solved = is_solved();
      if (solved) {
        attron(A_REVERSE | A_BOLD);
        addstr("You win!\n");
        attroff(A_REVERSE | A_BOLD);
      }
      refresh();
      int ch = getch();
      switch (ch) {
        case 'w': case 'W': case KEY_UP: {
          keypress(DIR_UP, solved);
          break;
        }
        case 's': case 'S': case KEY_DOWN: {
          keypress(DIR_DOWN, solved);
          break;
        }
        case 'a': case 'A': case KEY_LEFT: {
          keypress(DIR_LEFT, solved);
          break;
        }
        case 'd': case 'D': case KEY_RIGHT: {
          keypress(DIR_RIGHT, solved);
          break;
        }
        case 'r': case 'R': {
          goto loop_end; // goto is not always evil! for now, it's ok here
        }
        case 'q': case 'Q': {
          return;
        }
        default: {
          break;
        }
      }
    }
    loop_end:;
  }
}

int main(void) {
  srand(time(NULL));
  initscr();
  noecho();
  cbreak();
  keypad(stdscr, true);

  game_loop();

  endwin();
  return 0;
}
