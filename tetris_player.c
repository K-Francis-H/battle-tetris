#include <math.h>

#include "grid.h"
#include "util.h"
#include "tetris_player.h"

static int rotation_counts[] = {/*SQUARE  :*/ 0,
							    /*S_RIGHT :*/ 2,
							    /*S_LEFT  :*/ 2,
							    /*L_RIGHT :*/ 4,
							    /*L_LEFT  :*/ 4,
							    /*T       :*/ 4,
							    /*I       :*/ 2	
							   };

TetrisPlayer *create_tetris_player(int cx, int cy){ 
	int block_type = random_range(SQUARE, I); //pick a block randomly from SQUARE (0) to I (6)

	TetrisPlayer* t_player = (TetrisPlayer*)malloc(sizeof(TetrisPlayer));

	t_player->center_pos.x = cx;
	t_player->center_pos.y = cy;

	t_player->block.type = block_type;
	t_player->block.rot_index = 0;
	t_player->block.rot_count = rotation_counts[block_type];

	return t_player;
}

void destroy_tetris_player(TetrisPlayer *t_player){ 
	free(t_player);
}

Vector2 *get_grid_spots(int cx, int cy, int type, int rotation){
	Vector2* gs = (Vector2*)malloc(sizeof(Vector2)*NUM_BLOCKS);
	gs[0] = (Vector2){cx, cy};

	switch(type){
		case SQUARE:
			gs[1] = (Vector2){cx+1, cy};				//0 1
			gs[2] = (Vector2){cx, cy-1};				//2 3
			gs[3] = (Vector2){cx+1, cy-1};				//
			break;
		case S_RIGHT:
			switch(rotation){
				case 0:
					gs[1] = (Vector2){cx+1, cy};		//  0 1
					gs[2] = (Vector2){cx-1, cy-1};		//2 3
					gs[3] = (Vector2){cx, cy-1};		//
					break;
				case 1:
					gs[1] = (Vector2){cx, cy-1};		//  1
					gs[2] = (Vector2){cx-1, cy+1};		//2 0
					gs[3] = (Vector2){cx-1, cy};		//3
					break;
			}
			break;
		case S_LEFT:
			switch(rotation){ 
				case 0:
					gs[1] = (Vector2){cx-1, cy};		//1 0 
					gs[2] = (Vector2){cx, cy-1};		//  2 3
					gs[3] = (Vector2){cx+1, cy-1};		//
					break;
				case 1:
					gs[1] = (Vector2){cx, cy+1};		//  1
					gs[2] = (Vector2){cx-1, cy};		//2 0
					gs[3] = (Vector2){cx-1, cy-1};		//3
					break;
			}
			break;
		case L_RIGHT:
			switch(rotation){ 
				case 0:
					gs[1] = (Vector2){cx, cy+1};		//1
					gs[2] = (Vector2){cx, cy-1};		//0
					gs[3] = (Vector2){cx+1, cy-1};		//2 3
					break;
				case 1:
					gs[1] = (Vector2){cx+1, cy};		//2 0 1
					gs[2] = (Vector2){cx-1, cy};		//3
					gs[3] = (Vector2){cx-1, cy-1};		//
					break;
				case 2:
					gs[1] = (Vector2){cx, cy-1};		//3 2
					gs[2] = (Vector2){cx, cy+1};		//  0
					gs[3] = (Vector2){cx-1, cy+1};		//  1
					break;
				case 3:
					gs[1] = (Vector2){cx-1, cy};		//    3
					gs[2] = (Vector2){cx+1, cy};		//1 0 2
					gs[3] = (Vector2){cx+1, cy+1};		//
					break;
			}
			break;
		case L_LEFT:
			switch(rotation){ 
				case 0:
					gs[1] = (Vector2){cx, cy+1};		//  1
					gs[2] = (Vector2){cx, cy-1};		//  0
					gs[3] = (Vector2){cx-1, cy-1};		//3 2
					break;
				case 1:
					gs[1] = (Vector2){cx+1, cy};		//3
					gs[2] = (Vector2){cx-1, cy};		//2 0 1
					gs[3] = (Vector2){cx+1, cy+1};		//
					break;
				case 2:
					gs[1] = (Vector2){cx, cy-1};		//2 3
					gs[2] = (Vector2){cx, cy+1};		//0
					gs[3] = (Vector2){cx+1, cy+1};		//1
					break;
				case 3:
					gs[1] = (Vector2){cx-1, cy};		//1 0 2
					gs[2] = (Vector2){cx+1, cy};		//    3
					gs[3] = (Vector2){cx+1, cy-1};		//
					break;
			}
			break;
		case T:
			switch(rotation){ 
				case 0:
					gs[1] = (Vector2){cx-1, cy};		//  2
					gs[2] = (Vector2){cx, cy+1};		//1 0 3
					gs[3] = (Vector2){cx+1, cy};		//
					break;
				case 1:
					gs[1] = (Vector2){cx, cy+1};		//1
					gs[2] = (Vector2){cx+1, cy};		//0 2
					gs[3] = (Vector2){cx, cy-1};		//3
					break;
				case 2:
					gs[1] = (Vector2){cx+1, cy};		//3 0 1
					gs[2] = (Vector2){cx, cy-1};		//  2
					gs[3] = (Vector2){cx-1, cy};		//
					break;
				case 3:
					gs[1] = (Vector2){cx, cy-1};		//  3
					gs[2] = (Vector2){cx-1, cy};		//2 0
					gs[3] = (Vector2){cx, cy+1};		//  1
					break;
			}
			break;
		case I:
			switch(rotation){ 
				case 0:
					gs[1] = (Vector2){cx, cy+1};	//1
					gs[2] = (Vector2){cx, cy-1};	//0
					gs[3] = (Vector2){cx, cy-2};	//2
					break;							//3
				case 1:
					gs[1] = (Vector2){cx-1, cy};	//
					gs[2] = (Vector2){cx+1, cy};	//1 0 2 3
					gs[3] = (Vector2){cx+2, cy};	//
					break;
			}
			break;
	}
	return gs;
}

//TODO these all need to do grid bound checking or get prepared for some seg faults
static bool can_move_south(TetrisPlayer* t_player, Grid* grid){
	Vector2* fgs = get_grid_spots(t_player->center_pos.x,
								  t_player->center_pos.y-1,
								  t_player->block.type,
								  t_player->block.rot_index);

	int i;
	bool return_val = TRUE;
	for(i=0; i < NUM_BLOCKS; i++){ 
		if(fgs[i].y == 0 || grid->pos[fgs[i].x][fgs[i].y] == IMMOBILE_BLOCK)
			return_val = FALSE;
	}

	free(fgs);

	return return_val;
}

static bool can_move_east(TetrisPlayer* t_player, Grid* grid){
	Vector2* fgs = get_grid_spots(t_player->center_pos.x+1,
								  t_player->center_pos.y,
								  t_player->block.type,
								  t_player->block.rot_index);

	int i;
	bool return_val = TRUE;
	for(i=0; i < NUM_BLOCKS; i++){ 
		if(fgs[i].x == GRID_WIDTH || grid->pos[fgs[i].x][fgs[i].y] == IMMOBILE_BLOCK){
			return_val = FALSE;
			break;
		}
	}

	free(fgs);

	return return_val;
}

static bool can_move_west(TetrisPlayer* t_player, Grid* grid){
	Vector2* fgs = get_grid_spots(t_player->center_pos.x-1,
								  t_player->center_pos.y,
								  t_player->block.type,
								  t_player->block.rot_index);

	int i;
	bool return_val = TRUE;
	for(i=0; i < NUM_BLOCKS; i++){ 
		if(fgs[i].x < 0 || grid->pos[fgs[i].x][fgs[i].y] == IMMOBILE_BLOCK){
			return_val = FALSE;
			break;
		}
	}

	free(fgs);

	return return_val;
}

bool can_rotate(TetrisPlayer* t_player, Grid* grid){

	int future_rotation = t_player->block.rot_index+1 < t_player->block.rot_count ? 
						  t_player->block.rot_index+1 : 0;

	//fgs short for "future grid spots"
	Vector2* fgs = get_grid_spots(t_player->center_pos.x,
										 t_player->center_pos.y,
										 t_player->block.type,
										 future_rotation);

	//now we check if any of these spots are already IMMOBILE_BLOCKS
	int i;
	bool return_val = TRUE;
	for(i=0; i < NUM_BLOCKS; i++){ 
		if(fgs[i].x < 0 || fgs[i].x >= GRID_WIDTH || fgs[i].y < 0 || fgs[i].y >= GRID_HEIGHT){ 
			return_val = FALSE;
			break;
		}
		else if(grid->pos[fgs[i].x][fgs[i].y] == IMMOBILE_BLOCK){
			return_val = FALSE;
			break;
		}
	}

	free(fgs); 

	return return_val;
}

void rotate_tetris_player(TetrisPlayer* t_player, Grid* grid){ 
	if(can_rotate(t_player, grid)){ 
		if(t_player->block.rot_index+1 < t_player->block.rot_count)
			t_player->block.rot_index++;
		else
			t_player->block.rot_index = 0;
	}
}

void move_tetris_player(TetrisPlayer* t_player, Grid* grid, int direction){ 
	switch(direction){ 
		case LEFT:
			if(can_move_west(t_player, grid))
				t_player->center_pos.x--;
			break;
		case RIGHT:
			if(can_move_east(t_player, grid))
				t_player->center_pos.x++;
			break;
		case DOWN:
			if(can_move_south(t_player, grid))
				t_player->center_pos.y--;
			break;
	}
}

//TODO this only does color make it work with bitmaps as well for texturing
void render_tetris_player(TetrisPlayer* t_player, SDL_Surface *render_dest){ 

	Vector2 *blocks = get_grid_spots(t_player->center_pos.x,
									 t_player->center_pos.y,
									 t_player->block.type,
									 t_player->block.rot_index);

	SDL_Rect rect;
	int i;
	for(i=0; i < NUM_BLOCKS; i++){ 
		rect = (SDL_Rect){blocks[i].x*BLOCK_SIZE_PIXELS,
						   (GRID_HEIGHT - blocks[i].y)*BLOCK_SIZE_PIXELS,
			    		   BLOCK_SIZE_PIXELS,
			    		   BLOCK_SIZE_PIXELS};
		SDL_FillRect(render_dest, &rect, SDL_MapRGB(render_dest->format, 0xFF, 0x00, 0x00));
	}

	free(blocks); 
}

void update_tetris_player(TetrisPlayer *t_player, Grid *grid){ 
	//TODO

	//if we cant move down stick the spot and replace this tetris player with a new one
	if(!can_move_south(t_player, grid)){ 
		Vector2 *gs = get_grid_spots(t_player->center_pos.x,
									 t_player->center_pos.y,
									 t_player->block.type,
									 t_player->block.rot_index);

		int i;
		for(i=0; i < NUM_BLOCKS; i++){ 
			grid->pos[gs[i].x][gs[i].y] = IMMOBILE_BLOCK;
		}

		//now we reset the t_player

		//TODO these need to be more random or based off of some other value, right now its kinda arbitrary
		t_player->center_pos.x = 30;
		t_player->center_pos.y = 15;

		t_player->block.type = random_range(SQUARE, I);
		t_player->block.rot_index = 0;
		t_player->block.rot_count = rotation_counts[t_player->block.type];

		free(gs);
	}else{ 
		move_tetris_player(t_player, grid, DOWN);
	}
}