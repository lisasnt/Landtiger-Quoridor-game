#ifndef WALLS_H
#define WALLS_H

#include <inttypes.h>
#include "quoridor.h"

#define VERTICAL				0
#define HORIZONTAL			1
#define NO_WALL					0
#define VERTICAL_WALL 	1
#define HORIZONTAL_WALL 2
#define WALL_COLLISION 	1

#define WALL_DEFAULT_X			3
#define WALL_DEFAULT_Y			3
#define WALL_DEFAULT_ORIENT	HORIZONTAL

void player_place_wall(state_t* state, Player* player, uint8_t tokens_status[GRID_DIM_X][GRID_DIM_Y], uint8_t walls_matrix[WALL_GRID_DIM_X][WALL_GRID_DIM_Y]);

#endif
