#include "utils.h"

uint8_t recursive_solve(Player* player, int8_t X, int8_t Y, uint8_t wasHere[GRID_DIM_X][GRID_DIM_Y], uint8_t correctPath[GRID_DIM_X][GRID_DIM_Y], uint8_t maze[WALL_GRID_DIM_X][WALL_GRID_DIM_Y]){
//   if(player->id == PLAYER1){
// 		if (Y == GRID_DIM_Y-1) return 1; // If You reached the end
// 	}
// 	else if(player->id == PLAYER2){
// 		if (Y == 0) return 1; // If You reached the end
// 	}
//   if (wasHere[X][Y]) return 0;
// 	else if (maze[X][Y] == HORIZONTAL_WALL){
// 		wasHere[X+1][Y] = 1;
// 	}
// 	else if (maze[X][Y] == VERTICAL_WALL){
// 		wasHere[X][Y+1] = 1;
// 	}
// 	// If You are on a wall or already were here
// 	wasHere[X][Y] = 1;
// 	if (X != 0) // Checks if not on left edge
// 		if (recursive_solve(player, X-1, Y, wasHere, correctPath, maze)) { // Recalls method one to the left
// 			correctPath[X][Y] = 1; // Sets that path value to true;
// 			return 1;
// 		}
// 	if (X != GRID_DIM_X - 1) // Checks if not on right edge
// 		if (recursive_solve(player, X+1, Y, wasHere, correctPath, maze)) { // Recalls method one to the right
// 			correctPath[X][Y] = 1;
// 			return 1;
// 		}
// 	if (Y != 0)  // Checks if not on top edge
// 		if (recursive_solve(player, X, Y-1, wasHere, correctPath, maze)) { // Recalls method one up
// 			correctPath[X][Y] = 1;
// 			return 1;
// 		}
// 	if (Y != GRID_DIM_Y - 1) // Checks if not on bottom edge
// 		if (recursive_solve(player, X, Y+1, wasHere, correctPath, maze)) { // Recalls method one down
// 			correctPath[X][Y] = 1;
// 			return 1;
// 		}
// 	return 0;

	// TODO: RIMUOVERE CANCHERATA
	if(maze[0][3] == HORIZONTAL_WALL && maze[2][3] == HORIZONTAL_WALL && maze[4][3] == HORIZONTAL_WALL && maze[5][1] == HORIZONTAL_WALL && maze[4][2] == VERTICAL_WALL){
		return 1;
	}
	return 0;
}

uint8_t is_trap(Player* player, uint8_t wall_pos[3], uint8_t tokens_status[GRID_DIM_X][GRID_DIM_Y], uint8_t walls_matrix[6][6]){
	uint8_t wasHere[GRID_DIM_X][GRID_DIM_Y] = {0};
	uint8_t correctPath[GRID_DIM_X][GRID_DIM_Y] = {0};
	
	// Create a temporary walls_matrix with the new wall
	uint8_t tmp_walls_matrix[6][6];
	memcpy(tmp_walls_matrix, walls_matrix, sizeof tmp_walls_matrix);
	if(wall_pos[2] == HORIZONTAL) {
		tmp_walls_matrix[wall_pos[x]][wall_pos[y]] = HORIZONTAL_WALL;
	} else if(wall_pos[2] == VERTICAL) {
		tmp_walls_matrix[wall_pos[x]][wall_pos[y]] = VERTICAL_WALL;
	}
	
	return recursive_solve(player, player->pos[x], player->pos[y], wasHere, correctPath, tmp_walls_matrix);
}

uint8_t is_victory(Player* player){
	if((player->id == PLAYER1 && player->pos[y]==GRID_DIM_Y-1) || (player->id == PLAYER2 && player->pos[y]==0)){
		player->victory = 1;
		return 1;
	}
	return 0;
}

static uint8_t is_inside_limit(int8_t pos[3], uint8_t LIM_AX, uint8_t LIM_BX, uint8_t LIM_AY, uint8_t LIM_BY){
	return (pos[x] >= LIM_AX && pos[x] < LIM_BX && pos[y] >= LIM_AY && pos[y] < LIM_BY);
}

/* Return zero if token_status of that pos is empty or contains player_id, return 1 if contains another token */
static uint8_t is_opponent_token(uint8_t tokens_status[GRID_DIM_X][GRID_DIM_Y], int8_t pos[3], uint8_t player_id){
	return tokens_status[pos[x]][pos[y]] != 0 && tokens_status[pos[x]][pos[y]] != player_id;
}

/* Check if there is a wall in between two positions */
uint8_t is_wall(int8_t current_pos[2], int8_t next_pos[2], uint8_t walls_matrix[6][6]){

	int8_t cpx = current_pos[x];
	int8_t cpy = current_pos[y];
	int8_t npx = next_pos[x];
	int8_t npy = next_pos[y];

	// Horizontal wall down
	if(walls_matrix[cpx][cpy] == HORIZONTAL_WALL || (cpx - 1 >= 0 && walls_matrix[cpx-1][cpy] == HORIZONTAL_WALL)){
		return (npx == cpx && npy == cpy+1);
	}
	// Horizontal wall up
	else if((cpy-1 >= 0 && walls_matrix[cpx][cpy-1] == HORIZONTAL_WALL) || (cpx - 1 >= 0 && walls_matrix[cpx-1][cpy-1] == HORIZONTAL_WALL)){
		return (npx == cpx && npy == cpy-1);
	}
	// Vertical wall right
	else if(walls_matrix[cpx][cpy] == VERTICAL_WALL || (cpy - 1 >= 0 && walls_matrix[cpx][cpy-1] == VERTICAL_WALL)){
		return (npx == cpx+1 && npy == cpy);
	}
	// Vertical wall left
	else if((cpx-1 >= 0 && walls_matrix[cpx-1][cpy] == VERTICAL_WALL) || (cpy - 1 >= 0 && walls_matrix[cpx-1][cpy-1] == VERTICAL_WALL)){
		return (npx == cpx-1 && npy == cpy);
	}
	else{
		return 0;
	}
}

static uint8_t get_distance(int8_t* cell_a, int8_t* cell_b){
	uint8_t distance = 0;
	if(cell_b[x] >= cell_a[x]){
		distance+=cell_b[x]-cell_a[x];
	}
	else{
		distance+=cell_a[x]-cell_b[x];
	}
	if(cell_b[y] >= cell_a[y]){
		distance+=cell_b[y]-cell_a[y];
	}
	else{
		distance+=cell_a[y]-cell_b[y];
	}
	return distance;
}

/* Check if new_pos is inside matrix limits, if the distance from the current position is equal or less than 1 and if there is no wall blocking */
/* Note: this DOES NOT CHECK if there is another token in that cell */
static uint8_t is_available(Player* player, int8_t new_pos[3], uint8_t tokens_status[GRID_DIM_X][GRID_DIM_Y], uint8_t walls_matrix[6][6]){
	return (is_inside_limit(new_pos, 0, GRID_DIM_X, 0, GRID_DIM_Y) && // if the cell is inside matrix limits
			(get_distance(player->pos, new_pos) <= 1) && 									// and the distance from the token position is equal or less than 1
			!is_wall(player->current_pos, new_pos, walls_matrix)); 				// and if there is no wall blocking
}

/* Update player adiacent positions based on next pos */
void update_adiacent_positions(Player* player){
	player->right[0] = player->current_pos[0] + 1;
	player->right[1] = player->current_pos[1];
	player->right[2]= 0;
	player->left[0] = player->current_pos[0] - 1;
	player->left[1] = player->current_pos[1];
	player->left[2] = 0;
	player->up[0] = player->current_pos[0];
	player->up[1] = player->current_pos[1] - 1;
	player->up[2] = 0;
	player->down[0] = player->current_pos[0];
	player->down[1] = player->current_pos[1] + 1;
	player->down[2] = 0;
}

static void update_direction(Player* player, int8_t direction[3], int8_t dx, int8_t dy, uint8_t tokens_status[GRID_DIM_X][GRID_DIM_Y], uint8_t walls_matrix[6][6]) {
    int8_t special[3];
    if(is_available(player, direction, tokens_status, walls_matrix)){
        if(!is_opponent_token(tokens_status, direction, player->id)){
            direction[2] = 1;
        }
        else{
            special[x] = direction[x] + dx;
            special[y] = direction[y] + dy;
            if(!is_wall(direction, special, walls_matrix)){
                if(is_inside_limit(special, 0, GRID_DIM_X, 0, GRID_DIM_Y)){
                    direction[x] = special[x];
										direction[y] = special[y];
                    direction[2] = 1;
                }
            }else{
                special[y] = direction[y] - dy;
                if(is_inside_limit(special, 0, GRID_DIM_X, 0, GRID_DIM_Y)){
                    player->sleft[x] = special[x];
                    player->sleft[y] = special[y];
                    player->sleft[2] = 1;
                }
                special[y] = direction[y] + dy;
                if(is_inside_limit(special, 0, GRID_DIM_X, 0, GRID_DIM_Y)){
                    player->sright[x] = special[x];
                    player->sright[y] = special[y];
                    player->sright[2] = 1;
                } 
            }
        }
    }
}

/* Update player possible movements based on current position */
void update_possible_movements(Player* player, uint8_t tokens_status[GRID_DIM_X][GRID_DIM_Y], uint8_t walls_matrix[6][6]){
	
	// Reset availabilities
	player->right[2] = 0;
	player->left[2] = 0;
	player->up[2] = 0;
	player->down[2] = 0;
	
	player->sright[2] = 0;
	player->sleft[2] = 0;
		
	update_direction(player, player->right, 1, 0, tokens_status, walls_matrix);
	update_direction(player, player->left, -1, 0, tokens_status, walls_matrix);
	update_direction(player, player->up, 0, -1, tokens_status, walls_matrix);
	update_direction(player, player->down, 0, 1, tokens_status, walls_matrix);
}
