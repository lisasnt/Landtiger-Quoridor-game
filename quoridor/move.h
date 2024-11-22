#ifndef MOVE_H
#define MOVE_H

#include <inttypes.h>
#include "quoridor.h"

void player_move(state_t* state, Player* player, uint8_t tokens_status[GRID_DIM_X][GRID_DIM_Y], uint8_t walls_matrix[WALL_GRID_DIM_X][WALL_GRID_DIM_Y]);

#endif
