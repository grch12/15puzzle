#include "board.h"

int board[] = {
  1,  2,  3,  4,
  5,  6,  7,  8,
  9,  10, 11, 12,
  13, 14, 15, 0
};

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

inline void swap_board(int i, int j) {
  int temp = board[i];
  board[i] = board[j];
  board[j] = temp;
}

bool move_up(void) {
  int empty_index = find_empty();
  if (empty_index / 4 == 3)
    return false;
  swap_board(empty_index, empty_index + 4);
  return true;
}

bool move_down(void) {
  int empty_index = find_empty();
  if (empty_index / 4 == 0)
    return false;
  swap_board(empty_index, empty_index - 4);
  return true;
}

bool move_left(void) {
  int empty_index = find_empty();
  if (empty_index % 4 == 3)
    return false;
  swap_board(empty_index, empty_index + 1);
  return true;
}

bool move_right(void) {
  int empty_index = find_empty();
  if (empty_index % 4 == 0)
    return false;
  swap_board(empty_index, empty_index - 1);
  return true;
}
