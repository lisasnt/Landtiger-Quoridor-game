#include "quoridor.h"
#include "move.h"
#include "walls.h"
#include "utils.h"
#include <string.h> 
#include "../joystick/joystick.h"
#include "../graphics/graphics.h"
#include "../GLCD/GLCD.h"
#include "../button_EXINT/button.h"
#include "../timer/timer.h"
#include "../common.h"

uint8_t tokens_status[GRID_DIM_X][GRID_DIM_X] = {0};
uint8_t walls_matrix[WALL_GRID_DIM_X][WALL_GRID_DIM_X] = {0};
Player Player1 = {0};
Player Player2 = {0};

extern uint8_t joystick_flag;
extern uint8_t btn_flag;

state_t state; // States of FSM

static void start_timer(void)
{
	//	init_timer_simplified:
	//		1: which timer
	// 		2: prescaler
	//		3: mr0
	// 		4: mr1
	//		5: configuration of MR0 (interrupt, reset and stop) ( | of TIMER_INTERRUPT_MR, TIMER_RESET_MR, TIMER_STOP_MR)
	//		6: configuration of MR1 (interrupt, reset and stop) ( | of TIMER_INTERRUPT_MR, TIMER_RESET_MR, TIMER_STOP_MR)

	// TIMER_0
	// f_timer_hz    = 25000000 Hz = 25MHz
	// prescaler     =
	// MR            =
	// f_counter_hz  = f_timer_hz / (prescaler + 1) =
	// T_timer       = MR / f_counter_hz =
	// MR = T_timer * f_timer_hz / (prescaler + 1) =

	init_timer_simplified(TIMER_0, 0, TIM_MS_TO_TICKS_SIMPLE(1000), 0, TIMER_INTERRUPT_MR | TIMER_RESET_MR, 0);
	enable_timer(TIMER_0, PRIO_3);
}

// Timer interrupt callback
void update_time(void)
{
	if (Player1.current_player)
	{
		Player1.timer_time -= 1;
		update_time_stats(Player1.timer_time);
	}
	else
	{
		Player2.timer_time -= 1;
		update_time_stats(Player2.timer_time);
	}
}

static void init_game(void)
{
	uint8_t welcome_msg[] = " PRESS INT0 TO START A MATCH";

	// Make sure arrays and structs are zeroed before starting
	memset(&Player1, 0, sizeof(Player1));
	memset(&Player2, 0, sizeof(Player2));
	memset(tokens_status, 0, sizeof(tokens_status));
	memset(walls_matrix, 0, sizeof(walls_matrix));
	
	Player1.id = PLAYER1;
	Player2.id = PLAYER2;

	// First player default is player1
	Player1.current_player = 1;
	Player2.current_player = 0;

	Player1.pos[x] = DEFAULT_X_P1;
	Player1.pos[y] = DEFAULT_Y_P1;
	Player2.pos[x] = DEFAULT_X_P2;
	Player2.pos[y] = DEFAULT_Y_P2;

	Player1.previous_pos[x] = Player1.pos[x];
	Player1.previous_pos[y] = Player1.pos[y];
	Player2.previous_pos[x] = Player2.pos[x];
	Player2.previous_pos[y] = Player2.pos[y];

	Player1.current_pos[x] = Player1.pos[x];
	Player1.current_pos[y] = Player1.pos[y];
	Player2.current_pos[x] = Player2.pos[x];
	Player2.current_pos[y] = Player2.pos[y];

	update_adiacent_positions(&Player1);
	update_adiacent_positions(&Player2);
	
	Player1.walls = NUMBER_OF_WALLS;
	Player2.walls = NUMBER_OF_WALLS;
	
	Player1.total_time = QUORIDOR_TIME;
	Player2.total_time = QUORIDOR_TIME;
	Player1.timer_time = Player1.total_time;
	Player2.timer_time = Player2.total_time;

	// Initialize tokens_status matrix with initial players positions
	tokens_status[Player1.pos[x]][Player1.pos[y]] = Player1.id;
	tokens_status[Player2.pos[x]][Player2.pos[y]] = Player2.id;

	// Welcome message
	LCD_Clear(White);
	GUI_Text(0, 150, welcome_msg, Red, White);
}

void start_game(void)
{
	while (1)
	{
		/*Finite State Machine*/
		switch (state){
			case STATE_INIT:
				init_game();
				while(1){ // Wait for Start BTN (INT0)
					if(btn_flag & FLAG_BUTTON_0) {
						btn_flag &= ~FLAG_BUTTON_0;
						// Start with current player (first player set in init_game)
						if (Player1.current_player){
							state = STATE_PLAYER1_MOVE;
						}
						else{
							state = STATE_PLAYER2_MOVE;
						}
						break;
					}
				}
				LCD_Clear(Black);
				draw_grid(tokens_status);
				draw_stats(&Player1, &Player2);
				//start_timer();
				break;
			case STATE_PLAYER1_MOVE:
				Player1.current_player = 1;
				Player2.current_player = 0;
				// State changes in player_move or by timeout
				player_move(&state, &Player1, tokens_status, walls_matrix);
				break;
			case STATE_PLAYER1_WALL:
				Player1.current_player = 1;
				Player2.current_player = 0;
				// State changes in player_place_wall or by timeout
				player_place_wall(&state, &Player1, tokens_status, walls_matrix);
				break;
			case STATE_PLAYER2_MOVE:
				Player1.current_player = 0;
				Player2.current_player = 1;
				// State changes in player_move or by timeout
				player_move(&state, &Player2, tokens_status, walls_matrix);
				break;
			case STATE_PLAYER2_WALL:
				Player1.current_player = 0;
				Player2.current_player = 1;
				// State changes in player_place_wall or by timeout
				player_place_wall(&state, &Player2, tokens_status, walls_matrix);
				break;
			case STATE_VICTORY:
				// disable timer interrupt
				disable_timer(TIMER_0);
				// Victory screen
				draw_victory(&Player1, &Player2);
				while(1){ // Wait for Start BTN (INT0) to reset
					if(btn_flag & FLAG_BUTTON_0) {
						btn_flag &= ~FLAG_BUTTON_0;
						state = STATE_INIT;
						break;
					}
				}
				break;
			default:
				break;
		}
	}
}
