#include "play_logger.h"

void log_play(uint8_t player, enum Play_type play_type, uint8_t orientation, uint8_t new_pos_y, uint8_t new_pos_x){
	volatile uint32_t play = ((player - 1) << PLAYER_ID_SHIFT) + 
									(play_type << PLAY_TYPE_SHIFT) + 
									(orientation << ORIENT_SHIFT) + 
									(new_pos_y << Y_SHIFT) + 
									(new_pos_x << X_SHIFT);
}
