#ifndef QUORIDOR_H
#define QUORIDOR_H

#include <inttypes.h>

#define EMPTY 0U
#define PLAYER1 1U
#define PLAYER2 2U
#define WALL 3U
// Token coordinates [x,y]
#define x 0
#define y 1

#define GRID_DIM_X 7
#define GRID_DIM_Y 7

#define WALL_GRID_DIM_X GRID_DIM_X - 1
#define WALL_GRID_DIM_Y GRID_DIM_Y - 1

#define QUORIDOR_TIME 20

#define NUMBER_OF_WALLS 8

#define DEFAULT_X_P1 3
#define DEFAULT_Y_P1 0
#define DEFAULT_X_P2 3
#define DEFAULT_Y_P2 6

typedef enum
{
	STATE_INIT,
	STATE_PLAYER1_MOVE,
	STATE_PLAYER2_MOVE,
	STATE_PLAYER1_WALL,
	STATE_PLAYER2_WALL,
	STATE_VICTORY,
} state_t;

struct player
{
	uint8_t id;
	
	uint8_t victory;

	// Is this player the current player?
	uint8_t current_player;

	// X and Y coordinates
	// Real confirmed position from previous turn
	int8_t pos[2];
	// Previous unconfimed position of current turn
	int8_t previous_pos[2];
	// Current unconfirmed position of current turn
	int8_t current_pos[2];
	
	// index 0 and 1: X and Y coordinates, index 2: availability for movement (1) or not (0)
	// Note: these are position relative to current_pos
	// Note: availability considers:
	// - walls (e.g. position not available because there is a wall in between)
	// - matrix limits (e.g. position not available because it is outside the matrix)
	// - other tokens (e.g. position not available because there is another token in that cell)
	// - distance from real previous turn position (e.g. position not available because it is not one cell away from previous turn position)
	int8_t right[3];
	int8_t left[3];
	int8_t up[3];
	int8_t down[3];
	// Special cells available when opponent in front and wall behind, see quoridor rules
	int8_t sleft[3];
	int8_t sright[3];

	// Number of walls available
	uint8_t walls;

	uint8_t total_time;
	// Time left for current play
	uint8_t timer_time;
};

typedef struct player Player ;

void start_game(void);
void update_time(void);

#endif // QUORIDOR_H
