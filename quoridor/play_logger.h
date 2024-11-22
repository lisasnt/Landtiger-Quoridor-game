#ifndef PLAY_LOGGER_H
#define PLAY_LOGGER_H

#include <inttypes.h>

/* Format: PLAYER_ID (8), PLAY_TYPE (4), ORIENTATION (4), Y (8), X (8) */
#define PLAYER_ID_SHIFT	24
#define PLAY_TYPE_SHIFT	20
#define ORIENT_SHIFT		16
#define Y_SHIFT					8
#define X_SHIFT					0

enum Play_type {
	PLAY_TYPE_MOVE,
	PLAY_TYPE_WALL,
};

void log_play(uint8_t player, enum Play_type play_type, uint8_t orientation, uint8_t new_pos_y, uint8_t new_pos_x);

#endif
