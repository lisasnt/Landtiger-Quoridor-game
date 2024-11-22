#include "walls.h"
#include "quoridor.h"
#include "play_logger.h"
#include "../joystick/joystick.h"
#include "../button_EXINT/button.h"
#include "../graphics/graphics.h"
#include "utils.h"

extern uint8_t joystick_flag;
extern uint8_t btn_flag;

uint8_t wall_pos[3]; // keeps track of a wall until it is confirmed with JOY_SELECT

static uint8_t check_x_collisions_rotating(uint8_t walls_matrix[6][6], uint8_t wall_x, uint8_t wall_y) {
	if((wall_x-1 >= 0 && walls_matrix[wall_x-1][wall_y] == HORIZONTAL_WALL) || (wall_x+1 < 5 && walls_matrix[wall_x+1][wall_y] == HORIZONTAL_WALL)) {
			return WALL_COLLISION;
	}
	return !(WALL_COLLISION);
}

static uint8_t check_y_collisions_rotating(uint8_t walls_matrix[6][6], uint8_t wall_x, uint8_t wall_y) {
	if((wall_y-1 >= 0 && walls_matrix[wall_x][wall_y-1] == VERTICAL_WALL) || (wall_y+1 < 5 && walls_matrix[wall_x][wall_y+1] == VERTICAL_WALL)) {
			return WALL_COLLISION;
	}
	return !(WALL_COLLISION);
}

void player_place_wall(state_t* state, Player* player, uint8_t tokens_status[GRID_DIM_X][GRID_DIM_Y], uint8_t walls_matrix[WALL_GRID_DIM_X][WALL_GRID_DIM_Y]){
	//check joystick for wall movements
	//check JOY SELECT to confirm and switch to other STATE_PLAYER(~x)_MOVE
	//check KEY2 to rotate the wall
	//check KEY1 to return to STATE_PLAYER(x)_MOVE and delete the new-wall created
	
	// New wall defaut position	
	wall_pos[x] = WALL_DEFAULT_X;
	wall_pos[y] = WALL_DEFAULT_Y;
	wall_pos[2] = WALL_DEFAULT_ORIENT;
	draw_new_wall(wall_pos[x], wall_pos[y], wall_pos[2]);
	
	while(1) {
		
		// If time expired change player
		if(player->timer_time == 0){
			delete_wall(wall_pos[x], wall_pos[y], wall_pos[2]);
			player->timer_time = player->total_time;
			if (*state == STATE_PLAYER1_MOVE || *state == STATE_PLAYER1_WALL){
				*state = STATE_PLAYER2_MOVE;
			}
			else {
				*state = STATE_PLAYER1_MOVE;
			}
			break;
		}
		
		// KEY1 to return to STATE_PLAYER(x)_MOVE
		if(btn_flag & FLAG_BUTTON_1) {
			btn_flag &= ~FLAG_BUTTON_1;
			delete_wall(wall_pos[x], wall_pos[y], wall_pos[2]);
			if(player->id == PLAYER1) { 
				*state = STATE_PLAYER1_MOVE;
				break;
			}
			else if(player->id == PLAYER2) {
				*state = STATE_PLAYER2_MOVE;
				break;
			} 
		}
		
		// KEY2 Rotate wall
		if(btn_flag & FLAG_BUTTON_2) {
			btn_flag &= ~FLAG_BUTTON_2;
			delete_wall(wall_pos[x], wall_pos[y], wall_pos[2]);
			// check eventual collisions before rotating the wall
			if((wall_pos[2] == HORIZONTAL && check_y_collisions_rotating(walls_matrix, wall_pos[x], wall_pos[y]) != WALL_COLLISION) ||
					(wall_pos[2] == VERTICAL && check_x_collisions_rotating(walls_matrix, wall_pos[x], wall_pos[y]) != WALL_COLLISION)) {
				wall_pos[2] = !(wall_pos[2]);
			}
			move_wall(wall_pos[x], wall_pos[y], wall_pos[2]);
		}
		// JOYSTICK UP --------------------------------
		if(joystick_flag & FLAG_JOYSTICK_UP) {	
			joystick_flag &= ~FLAG_JOYSTICK_UP;
			delete_wall(wall_pos[x], wall_pos[y], wall_pos[2]);
			redraw_old_walls(walls_matrix);
			if((wall_pos[y] > 0) && (walls_matrix[wall_pos[x]][wall_pos[y]-1] == NO_WALL))	{
				wall_pos[y] -= 1;
			}
			/*if(wall_pos[2] == HORIZONTAL) {
				if( ((wall_pos[y] == 1) && walls_matrix[wall_pos[x]][wall_pos[y-1]] != HORIZONTAL_WALL ) || ((wall_pos[y] > 0) && (walls_matrix[wall_pos[x]][wall_pos[y-2]] != VERTICAL_WALL))) {
					wall_pos[y] -= 1;
				}
			} else if(wall_pos[2] == VERTICAL) {
				if( ((wall_pos[y] == 1) && walls_matrix[wall_pos[x]][wall_pos[y-1]] != HORIZONTAL_WALL ) || ((wall_pos[y] > 0) && (walls_matrix[wall_pos[x]][wall_pos[y-2]] != VERTICAL_WALL))) {
					wall_pos[y] -= 1;
				}
			}*/
			move_wall(wall_pos[x], wall_pos[y], wall_pos[2]);
		}
		
		// JOYSTICK DOWN ------------------------------
		if(joystick_flag & FLAG_JOYSTICK_DOWN) {		
			joystick_flag &= ~FLAG_JOYSTICK_DOWN;	
			delete_wall(wall_pos[x], wall_pos[y], wall_pos[2]);
			redraw_old_walls(walls_matrix);
			if((wall_pos[y] < 5) && (walls_matrix[wall_pos[x]][wall_pos[y]+1] == NO_WALL))	{
				wall_pos[y] += 1;
			}
			move_wall(wall_pos[x], wall_pos[y], wall_pos[2]);
		}
		
		// JOYSTICK LEFT ------------------------------
		if(joystick_flag & FLAG_JOYSTICK_LEFT) {
			joystick_flag &= ~FLAG_JOYSTICK_LEFT;
			delete_wall(wall_pos[x], wall_pos[y], wall_pos[2]);
			redraw_old_walls(walls_matrix);
			if((wall_pos[x] > 0) && (walls_matrix[wall_pos[x]-1][wall_pos[y]] == NO_WALL))	{
				wall_pos[x] -= 1;
			}
			move_wall(wall_pos[x], wall_pos[y], wall_pos[2]);
		}
		
		// JOYSTICK RIGHT ------------------------------
		if(joystick_flag & FLAG_JOYSTICK_RIGHT) {
			joystick_flag &= ~FLAG_JOYSTICK_RIGHT;
			delete_wall(wall_pos[x], wall_pos[y], wall_pos[2]);
			redraw_old_walls(walls_matrix);
			if((wall_pos[x] < 5) && (walls_matrix[wall_pos[x]+1][wall_pos[y]] == NO_WALL))	{
				wall_pos[x] += 1;
			}
			move_wall(wall_pos[x], wall_pos[y], wall_pos[2]);
		}
		
		// JOYSTICK SELECT -----------------------------
		if(joystick_flag & FLAG_JOYSTICK_SELECT) {
			joystick_flag &= ~FLAG_JOYSTICK_SELECT;
			if(!is_trap(player, wall_pos, tokens_status, walls_matrix)){
				// Reset timer
				player->timer_time = player->total_time;
				// Decrement wall number
				player->walls -= 1;
				// Refresh wall stats
				update_walls_stats(player);
				// save wall position in walls_status: 1 if horizontal,	2 if vertical
				move_wall(wall_pos[x], wall_pos[y], wall_pos[2]); // needed only to confirm the not moved new-wall
				if(wall_pos[2] == HORIZONTAL) {
					walls_matrix[wall_pos[x]][wall_pos[y]] = HORIZONTAL_WALL;
				} else if(wall_pos[2] == VERTICAL) {
					walls_matrix[wall_pos[x]][wall_pos[y]] = VERTICAL_WALL;
				}
				// Log play
				log_play(player->id, PLAY_TYPE_WALL, wall_pos[2], wall_pos[y], wall_pos[x]);
				// wall is confirmed, now switch player
				if(player->id == PLAYER1) {
					*state = STATE_PLAYER2_MOVE;
					break;
				}
				else if(player->id == PLAYER2) {
					*state = STATE_PLAYER1_MOVE;
					break;
				}
			}
		}
	}
}

