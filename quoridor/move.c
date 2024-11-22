#include "move.h"
#include "quoridor.h"
#include "play_logger.h"
#include "../button_EXINT/button.h"
#include "../joystick/joystick.h"
#include "../GLCD/GLCD.h"
#include "../graphics/graphics.h"
#include "utils.h"
#include "../timer/timer.h"
#include "../common.h"

// TODO: jump special left right not working
// TODO: bug sometimes one direction is not marked as available until moving

extern uint8_t joystick_flag;
extern uint8_t btn_flag;

static void update_grid_pos(Player* player, uint8_t tokens_status[GRID_DIM_X][GRID_DIM_Y]){
		tokens_status[player->pos[x]][player->pos[y]] = 0;
		player->pos[x] = player->current_pos[x];
		player->pos[y] = player->current_pos[y];
		tokens_status[player->pos[x]][player->pos[y]] = player->id;
}

static void update_position(Player* player, uint8_t direction, uint8_t tokens_status[GRID_DIM_X][GRID_DIM_Y], uint8_t walls_matrix[WALL_GRID_DIM_X][WALL_GRID_DIM_Y]) {
	uint8_t updated = 0;
	
	// Update previous position
	player->previous_pos[x] = player->current_pos[x];
	player->previous_pos[y] = player->current_pos[y];

	switch(direction) {
		case JOYSTICK_UP:
			if(player->up[2] == 1) {
				player->current_pos[x] = player->up[x];
				player->current_pos[y] = player->up[y];
				updated=1;
			}
			break;
		case JOYSTICK_DOWN:
			if(player->down[2] == 1) {
				player->current_pos[x] = player->down[x];
				player->current_pos[y] = player->down[y];
				updated=1;
			}
			break;
		case JOYSTICK_LEFT:
			if(player->left[2] == 1) {
				player->current_pos[x] = player->left[x];
				player->current_pos[y] = player->left[y];
				updated=1;
			}
			break;
		case JOYSTICK_RIGHT:
			if(player->right[2] == 1) {
				player->current_pos[x] = player->right[x];
				player->current_pos[y] = player->right[y];
				updated=1;
			}
			break;
		// Default is reset to initial position
		default:
			player->current_pos[x] = player->pos[x];
			player->current_pos[y] = player->pos[y];
			updated=1;
			break;
	}
	if(updated){
		remove_possible_movements(player); 															// Remove previous cell possible movements
		update_adiacent_positions(player); 															// Update positions: right, left, up, down relative to new current_pos
		redraw_token_position(player);																	// Redraw pos (and color previous cell yellow)
		update_possible_movements(player, tokens_status, walls_matrix); // Update possible movements according to the new current_pos
		draw_possible_movements(player, Yellow);												// Draw new possibile movements
	}
}

void player_move(state_t* state, Player* player, uint8_t tokens_status[GRID_DIM_X][GRID_DIM_Y], uint8_t walls_matrix[WALL_GRID_DIM_X][WALL_GRID_DIM_Y]){
	
	joystick_flag = 0; // Reset to forget previous movements
	
	update_possible_movements(player, tokens_status, walls_matrix);
	draw_possible_movements(player, Yellow);
	
	//TODO: flag_movement to avoid oblique dir, first one called enable it
	while(1) {
		
		// If time expired change player
		if(player->timer_time == 0){
			update_position(player, 0, tokens_status, walls_matrix);  // move token to intial pos
			remove_possible_movements(player);												// clear all positions
			// Reset time
			player->timer_time = player->total_time;
			if (*state == STATE_PLAYER1_MOVE || *state == STATE_PLAYER1_WALL){
				*state = STATE_PLAYER2_MOVE;
			}
			else {
				*state = STATE_PLAYER1_MOVE;
			}
			break;
		}

		// JOYSTICK UP --------------------------------
		if(joystick_flag & FLAG_JOYSTICK_UP) {
			joystick_flag &= ~FLAG_JOYSTICK_UP;
			update_position(player, JOYSTICK_UP, tokens_status, walls_matrix); // Update pos
		}
		
		// JOYSTICK DOWN ------------------------------
		if(joystick_flag & FLAG_JOYSTICK_DOWN) {		
			joystick_flag &= ~FLAG_JOYSTICK_DOWN;	
			update_position(player, JOYSTICK_DOWN, tokens_status, walls_matrix);
		}
		
		// JOYSTICK LEFT ------------------------------
		if(joystick_flag & FLAG_JOYSTICK_LEFT) {
			joystick_flag &= ~FLAG_JOYSTICK_LEFT;
			update_position(player, JOYSTICK_LEFT, tokens_status, walls_matrix);
		}
		
		// JOYSTICK RIGHT ------------------------------
		if(joystick_flag & FLAG_JOYSTICK_RIGHT) {
			joystick_flag &= ~FLAG_JOYSTICK_RIGHT;
			update_position(player, JOYSTICK_RIGHT, tokens_status, walls_matrix);
		}
		
		// JOYSTICK SELECT -----------------------------
		if(joystick_flag & FLAG_JOYSTICK_SELECT) {
			joystick_flag &= ~FLAG_JOYSTICK_SELECT;
			if(player->current_pos[x] != player->pos[x] || player->current_pos[y] != player->pos[y]){
				// Remove yellow possible movements of current cell
				remove_possible_movements(player);
				// Finally update player position in tokens_status
				update_grid_pos(player, tokens_status);
			 	// Reset timer
				player->timer_time = player->total_time;
				// Log play
				log_play(player->id, PLAY_TYPE_MOVE, 0, player->pos[y], player->pos[x]);
				if(is_victory(player)){
					*state = STATE_VICTORY;
					break;
				}
				// Change player
				if(player->id == PLAYER1) {
					*state = STATE_PLAYER2_MOVE;
				}
				else if (player->id == PLAYER2){
					*state = STATE_PLAYER1_MOVE;
				}
				break;
			}
		}
		
		// KEY1 Insert wall
		if(btn_flag & FLAG_BUTTON_1) {
			btn_flag &= ~FLAG_BUTTON_1;
			if(player->walls > 0){
				remove_possible_movements(player);
				if(player->id == PLAYER1) { 
					*state = STATE_PLAYER1_WALL;
					break;
				}
				else if (player->id == PLAYER2){
					*state = STATE_PLAYER2_WALL;
					break;
				}
			}else{
				uint8_t no_walls_available[29];
				snprintf((char*)no_walls_available, 29, "P%d:No walls available, move!", player->id);
				GUI_Text(6+2, 240, no_walls_available, Red, Black);
			}
		}
	}
}
