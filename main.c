#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#ifdef _POSIX
#include <ncurses.h>
#else
#include <curses.h>
#endif

int board[] = {
  1,  2,  3,  4,
  5,  6,  7,  8,
  9,  10, 11, 12,
  13, 14, 15, 0
};

int moves = 0;

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

bool is_solved(void) {
  for (int i = 0; i < (int)(sizeof(board) / sizeof(int)); i++) {
    int target = i == 15 ? 0 : i + 1;
    if (board[i] != target)
      return false;
  }
  return true;
}

int find_empty(void) {
  for (int i = 0; i < (int)(sizeof(board) / sizeof(int)); i++) {
    if (board[i] == 0)
      return i;
  }
  return -1;
}

void print_game(void) {
  print_grid();
  addstr("Use WASD or arrow keys to control, Q to quit, R to restart\n");
  printw("Moves: %d\n", moves);
}

void move_up(void) {
  int empty_index = find_empty();
  if (empty_index / 4 == 3)
    return;
  board[empty_index] = board[empty_index + 4];
  board[empty_index + 4] = 0;
  moves++;
}

void move_down(void) {
  int empty_index = find_empty();
  if (empty_index / 4 == 0)
    return;
  board[empty_index] = board[empty_index - 4];
  board[empty_index - 4] = 0;
  moves++;
}

void move_left(void) {
  int empty_index = find_empty();
  if (empty_index % 4 == 3)
    return;
  board[empty_index] = board[empty_index + 1];
  board[empty_index + 1] = 0;
  moves++;
}

void move_right(void) {
  int empty_index = find_empty();
  if (empty_index % 4 == 0)
    return;
  board[empty_index] = board[empty_index - 1];
  board[empty_index - 1] = 0;
  moves++;
}

/**
 * @brief Shuffle the board by making 1000 random moves
 *
 * This function shuffles the board by making 1000 random moves. It
 * uses the rand() function to generate a random direction (up, down, left,
 * right) and then calls the corresponding move function to make the move.
 *
 * @return void
 */
void shuffle(void) {
  for (int i = 0; i < 1000; i++) {
    int direction = rand() % 4;
    switch (direction) {
      case 0: move_up(); break;
      case 1: move_down(); break;
      case 2: move_left(); break;
      case 3: move_right(); break;
    }
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
  bool game_over = false;
  while (!game_over) {
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
          if (!solved) move_up();
          break;
        }
        case 's': case 'S': case KEY_DOWN: {
          if (!solved) move_down();
          break;
        }
        case 'a': case 'A': case KEY_LEFT: {
          if (!solved) move_left();
          break;
        }
        case 'd': case 'D': case KEY_RIGHT: {
          if (!solved) move_right();
          break;
        }
        case 'r': case 'R': {
          goto for_end; // goto is not always evil! for now, it's ok here
        }
        case 'q': case 'Q': {
          game_over = true;
          goto for_end;
        }
        default: {
          break;
        }
      }
      clear();
    }
    for_end:
    clear();
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
