#ifndef UTILS_H
#define UTILS_H

#include <inttypes.h>
#include <string.h> 
#include "quoridor.h"
#include "walls.h"

void update_adiacent_positions(Player* player);
void update_possible_movements(Player* player, uint8_t tokens_status[GRID_DIM_X][GRID_DIM_Y], uint8_t walls_matrix[6][6]);	
uint8_t is_wall(int8_t current_pos[2], int8_t next_pos[2], uint8_t walls_matrix[6][6]);
uint8_t is_victory(Player* player);
uint8_t is_trap(Player* player, uint8_t wall_pos[3], uint8_t tokens_status[GRID_DIM_X][GRID_DIM_Y], uint8_t walls_matrix[6][6]);

#endif
