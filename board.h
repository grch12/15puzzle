#ifndef _BOARD_H_
#define _BOARD_H_

#include <stdbool.h>

extern int board[16];

bool is_solved(void);
int find_empty(void);

bool move_up(void);
bool move_down(void);
bool move_left(void);
bool move_right(void);

#endif
