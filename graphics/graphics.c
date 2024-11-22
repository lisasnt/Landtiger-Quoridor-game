#include "graphics.h"
#include "../GLCD/GLCD.h"
#include "../quoridor/walls.h"
#include <inttypes.h>
#include <stdio.h>

// ======== HELPERS ========

static void draw_empty_rectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color) {
	uint16_t tmp;
	if(x0 > x1) {
		tmp = x0;
		x0 = x1;
		x1 = tmp;
	}
	
	if(y0 > y1) {
		tmp = y0;
		y0 = y1;
		y1 = tmp;
	}
	
	if(y0 != y1) {
		LCD_DrawLine(x0, y0, x0, y1, color);
		LCD_DrawLine(x1, y1, x1, y0, color);
	}
	if(x0 != x1) {
		LCD_DrawLine(x0, y1, x1, y1, color);
		LCD_DrawLine(x0, y0, x1, y0, color);
	}
}

static void draw_rectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t border_color, uint16_t fill_color) {
	uint16_t tmp;
	
	if(x0 > x1) {
		tmp = x0;
		x0 = x1;
		x1 = tmp;
	}
	
	if(y0 > y1) {
		tmp = y0;
		y0 = y1;
		y1 = tmp;
	}
	
	draw_empty_rectangle(x0, y0, x1, y1, border_color);
	
	if(x0==x1 || y0==y1) return;
	
	for(tmp=x0+1; tmp<x1;++tmp) {
		LCD_DrawLine(tmp, y0+1, tmp, y1-1, fill_color);
	}
}

// ======== DRAWING FUNCTIONS ========

void draw_grid(uint8_t tokens_status[GRID_DIM_X][GRID_DIM_Y]){ 
	uint8_t i;
	
	for (i = 1; i <= GRID_DIM_Y; i++) {
		uint8_t j;
		for (j = 1; j <= GRID_DIM_X; j++) {
			uint8_t x0 = j*SPACE + (j-1)*CELL_X; 
			uint8_t y0 = i*SPACE + (i-1)*CELL_Y;
			if (tokens_status[j-1][i-1] == EMPTY) { // consider only cell not spaces in the tokens_status matrix
				draw_empty_rectangle(x0, y0, x0+CELL_X, y0+CELL_Y, White); 
			} else if (tokens_status[j-1][i-1] == PLAYER1) {
				draw_rectangle(x0, y0, x0+CELL_X, y0+CELL_Y, White, White);
			} else if (tokens_status[j-1][i-1] == PLAYER2) {
				draw_rectangle(x0, y0, x0+CELL_X, y0+CELL_Y, White, Red);
			}
		}
	}
}

void draw_stats(Player* Player1, Player* Player2){
	uint8_t P1_wall_text[] = "P1 Wall";
	uint8_t P2_wall_text[] = "P2 Wall";
	uint8_t P1_walls_available_str[2];
	uint8_t P2_walls_available_str[2];
	uint8_t time_str[5]; // XX_s + null char

  	// Get walls available strings and timer string
	snprintf((char*)P1_walls_available_str, 2, "%d", Player1->walls);
	snprintf((char*)P2_walls_available_str, 2, "%d", Player2->walls);
	
	if(Player1->current_player){
			snprintf((char*)time_str, 4, "%02d s", Player1->timer_time);
	} else {
			snprintf((char*)time_str, 4, "%02d s", Player2->timer_time);
	}
	
	// P1 Wall box
	draw_empty_rectangle(6, 260, 6+72, 310, White);
	GUI_Text(6+2, 262, P1_wall_text, White, Black);
	GUI_Text(6+2, 282, P1_walls_available_str, White, Black);
	// P2 Wall box
	draw_empty_rectangle(240-6, 260, 240-6-72, 310, White);
	GUI_Text(240-6-72+2, 262, P2_wall_text, White, Black);
	GUI_Text(240-6-72+2, 282, P2_walls_available_str, White, Black);
	// Timer box
	draw_empty_rectangle(6+72+6, 260, 6+72+6+72, 310, White);
	GUI_Text(6+72+6+2, 262, time_str, White, Black);
}

void update_time_stats(uint8_t timer_time){
	uint8_t time_str[5]; // XX_s + null char
	snprintf((char*)time_str, 5, "%02d s", timer_time);
	GUI_Text(6+72+6+2, 262, time_str, White, Black);
}

/** Color a specific cell in the 7x7 tokens matrix **/
void color_pos(int8_t* cell_to_color, uint16_t fill_color){
	// Cell position
	uint8_t x0 = (cell_to_color[x])*SPACE + (cell_to_color[x])*CELL_X + SPACE; 
	uint8_t y0 = (cell_to_color[y])*SPACE + (cell_to_color[y])*CELL_Y + SPACE;
	
	draw_rectangle(x0, y0, x0+CELL_X, y0+CELL_Y, White, fill_color);
}

/** Colors the available cells around the player **/
void draw_possible_movements(Player* player, uint16_t fill_color){
	// right
	if(player->right[2] == 1) {
		color_pos(player->right, fill_color);
	}
	// left
	if(player->left[2] == 1) {
		color_pos(player->left, fill_color);
	}
	// up
	if(player->up[2] == 1) {
		color_pos(player->up, fill_color);
	}
	// down
	if(player->down[2] == 1) {
		color_pos(player->down, fill_color);
	}
	// sright
	if(player->sright[2] == 1) {
		color_pos(player->sright, fill_color);
	}
	// sleft
	if(player->sleft[2] == 1) {
		color_pos(player->sleft, fill_color);
	}
}

void remove_possible_movements(Player* player){
	draw_possible_movements(player, Black);
}

void redraw_token_position(Player* player) {
	// Previous position must be colored again (always yellow 
	// because it is always a possible movement)
	color_pos(player->previous_pos, Yellow);
	if (player->id == PLAYER1) {
		color_pos(player->current_pos, White);
	} else {
		color_pos(player->current_pos, Red);
	}
}

void update_walls_stats(Player* player){
	uint8_t P1_walls_available_str[2];
	uint8_t P2_walls_available_str[2];

	// Get walls available strings and timer string
	if(player->id == PLAYER1) {
		snprintf((char*)P1_walls_available_str, 2, "%d", player->walls);
		GUI_Text(6+2, 282, P1_walls_available_str, White, Black);
	} else {
		snprintf((char*)P2_walls_available_str, 2, "%d", player->walls);
		GUI_Text(240-6-72+2, 282, P2_walls_available_str, White, Black);
	}
}

void draw_wall(uint8_t wall_x, uint8_t wall_y, uint8_t orientation, uint16_t color) {
	uint8_t matrix_position[2];
	uint8_t graphic_center_pos[2];
	uint8_t x0, y0, x1, y1;
	
	matrix_position[x] = wall_x;
	matrix_position[y] = wall_y;
	
	graphic_center_pos[x] = (matrix_position[x]+1)*(SPACE+CELL_X) + SPACE/2;
	graphic_center_pos[y] = (matrix_position[y]+1)*(SPACE+CELL_Y) + SPACE/2;
	// Compute graphical coordinates x0, y0, x1, y1
	if(orientation) {
		// it is horizontal
		x0 = graphic_center_pos[x] - WALL_L/2;
		y0 = graphic_center_pos[y] - WALL_H/2;
		x1 = x0 + WALL_L;
		y1 = y0 + WALL_H;
	} else {
		// it is vertical
		x0 = graphic_center_pos[x] - WALL_H/2;
		y0 = graphic_center_pos[y] - WALL_L/2;
		x1 = x0 + WALL_H;
		y1 = y0 + WALL_L;
	}
	draw_rectangle(x0+1, y0+1, x1-1, y1-1, color, color);
}

void draw_new_wall(uint8_t wall_x, uint8_t wall_y, uint8_t orientation) {
	draw_wall(3, 3, HORIZONTAL, Red);
}

void delete_wall(uint8_t wall_x, uint8_t wall_y, uint8_t orientation) {
	draw_wall(wall_x, wall_y, orientation, Black);
}

void move_wall(uint8_t wall_x, uint8_t wall_y, uint8_t orientation) {
	draw_wall(wall_x, wall_y, orientation, Magenta);
}

void redraw_old_walls(uint8_t walls_matrix[WALL_GRID_DIM_X][WALL_GRID_DIM_Y]) {
	uint8_t i, j; 
	uint8_t pos;
	for(i = 0; i < WALL_GRID_DIM_X; i++) {
		for(j = 0; j < WALL_GRID_DIM_Y; j++) {
			pos = walls_matrix[i][j];
			switch(pos) {
				case(HORIZONTAL_WALL):
					move_wall(i, j, HORIZONTAL);
				break;
				case(VERTICAL_WALL):
					move_wall(i, j, VERTICAL);
				break;
			}
		}
	}
}

void draw_victory(Player* player1, Player* player2){
	uint8_t victory_msg[39];
	LCD_Clear(White);
	if(player1->victory){
		snprintf((char*)victory_msg, 39, "PLAYER%d VICTORY, PRESS INT0 TO RESTART", player1->id);
	}
	else{
		snprintf((char*)victory_msg, 39, "PLAYER%d VICTORY, PRESS INT0 TO RESTART", player2->id);
	}
	GUI_Text(0, 150, victory_msg, Red, White);
}
