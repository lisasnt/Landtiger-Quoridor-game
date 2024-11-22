#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <inttypes.h>
#include "../quoridor/quoridor.h"

// LCD dimensions : 240 x 320
#define LCD_X 240
#define LCD_Y 320

#define SPACE	4	//TODO: define using grid dimensions
#define CELL_X	30	//TODO: define using grid dimensions
#define CELL_Y	CELL_X
#define WALL_L	(2*CELL_X+SPACE)
#define WALL_H	SPACE

void draw_grid(uint8_t tokens_status[GRID_DIM_X][GRID_DIM_Y]);
void draw_stats(Player* Player1, Player* Player2);
void update_time_stats(uint8_t timer_time);
void update_walls_stats(Player* player);
void redraw_token_position(Player* player);
void color_pos(int8_t* pos_player, uint16_t fill_color);
void draw_possible_movements(Player* player, uint16_t fill_color);
void remove_possible_movements(Player* player);

void draw_wall(uint8_t wall_x, uint8_t wall_y, uint8_t orientation, uint16_t color);
void draw_new_wall(uint8_t wall_x, uint8_t wall_y, uint8_t orientation);
void delete_wall(uint8_t wall_x, uint8_t wall_y, uint8_t orientation);
void move_wall(uint8_t wall_x, uint8_t wall_y, uint8_t orientation);
void redraw_old_walls(uint8_t walls_matrix[WALL_GRID_DIM_X][WALL_GRID_DIM_Y]);


void draw_victory(Player* player1, Player* player2);

#endif // GRAPHICS_H
