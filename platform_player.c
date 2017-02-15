#include <math.h>

#include "grid.h"
#include "util.h"
#include "platform_player.h"

static bool can_move_east(PlatformPlayer* player, Grid* grid);
static bool can_move_west(PlatformPlayer* player, Grid* grid);
bool can_fall(PlatformPlayer* player, Grid* grid);

PlatformPlayer* create_platform_player(Grid* grid){
	//maybe in future we can sorta random assign but for now we'll just do leftmost column

	//find first occupiable spot on leftmost column of grid

	//TODO maybe have this all match the logical way of doing things
	int i, y;
	for(i=0; i < grid->h; i++){
		if(grid->pos[0][i] == EMPTY_BLOCK){
			printf("y=%d\n", i);
			y = i;//grid->h - i;
			break;
		}
	}

	PlatformPlayer* player = malloc(sizeof(PlatformPlayer));
	player->gx = 0;
	player->gy = y;
	player->rx = 0;
	player->ry = y*BLOCK_SIZE_PIXELS;
	//player->direction = (Vector2){1,0}; 					//horizontal direction
	player->weapon = random_range(GUN, DRILL); 	//pick a weapon from the set {GUN, MISSILE_LAUNCHER, DRILL}
	player->projectile_count = 0;
	player->is_moving_left = FALSE;
	player->is_moving_right = FALSE;
	//the facing depends on starting side, but foor now it is always right
	player->direction = RIGHT;
	//player->is_facing_right = TRUE;
	//player->is_facing_left = FALSE;
	player->is_jumping = FALSE;
	player->jump_velocity = 0;
	player->grid = grid;

	return player;
}

int get_max_fall(Grid* grid, PlatformPlayer* player){
	
	//first get column map

	
}

//TODO need to check for how far the pixel jump goes
void update_platform_player(Grid* grid, PlatformPlayer* player){
	//TODO more stuff like hit points, etc but just gravity for now

	printf("player real: x=%d y=%d\n", player->rx, player->ry);
	printf("player grid: x=%d y=%d\n", player->gx, player->gy);

	AvailableMoves moves = get_available_moves(player, grid);
	/*if(moves.s || player->is_jumping){
		int max_fall = get_max_fall(grid, player);
		if(max_fall == player->jump_velocity+1)
			player->ry += (player->jump_velocity++);
		else{
			player->ry += max_fall;
			player->is_jumping = FALSE; 
		}
	}else{
		player->is_jumping = FALSE;
	}*/

	//TODO update every projectile
	int i;
	for(i=0; i < player->projectile_count; i++){
		//eval position on grid and act accordingly
		Projectile* proj = &(player->projectiles[i]);
		if(proj->direction == LEFT){
			proj->rx -= 5;
		}
		else{
			proj->rx += 5;
		}

		proj->gx = (proj->rx) / BLOCK_SIZE_PIXELS;

		if(proj->gx >= 0 && proj->gx < grid->w){
			if(grid->pos[proj->gx][proj->gy] == IMMOBILE_BLOCK){
				//destroy
				grid->pos[proj->gx][proj->gy] = EMPTY_BLOCK;
				//TODO then also destroy this projectile....

				int index = i;
				int j;
				for(j=index; j < player->projectile_count-1; j++){
					player->projectiles[j] = player->projectiles[j+1];
				}
				player->projectile_count--;
			}
		}else{
			//TODO
			//destroy this one somehow...
			//linked list would work better for this sorta thing else we have to rearrange the whole array here
			int index = i;
			int j;
			for(j=index; j < player->projectile_count-1; j++){
				player->projectiles[j] = player->projectiles[j+1];
			}
			player->projectile_count--;
		}
	}

	printf("player LEFT: %d\n", player->is_moving_left);
	printf("player RIGHT: %d\n", player->is_moving_right);

	if(player->is_moving_left){
		if(can_move_west(player, grid)){
			player->rx -= 5;
		}
	}

	if(player->is_moving_right){
		if(can_move_east(player, grid)){
			player->rx += 5;
		}
	}

	//clamp to player grid spot
	int half = BLOCK_SIZE_PIXELS/2;
	player->gx = (player->rx) / BLOCK_SIZE_PIXELS; //integer division
	player->gy = (player->ry) / BLOCK_SIZE_PIXELS;

	if(player->is_jumping || can_fall(player, grid)){
		//printf("max_fall = %d\n", get_max_fall(grid, player));

		printf("player->rx = %d\n", player->rx);

		//check column height
		int future_y = player->ry + player->jump_velocity;
		printf("future_y = %d\n", future_y);
		int grid_y = future_y / BLOCK_SIZE_PIXELS;
		//printf("grid_y = %d\n",  grid_y);



		if(player->rx % BLOCK_SIZE_PIXELS > 0){
			//printf("player->rx mod BLOCK_SZIE_PIXELS > 0 ");
			//recompute the player->gx here because otherwise it may use the last value and int he edge case gx=59 it will crash because it may check gx=60 (outside the array)
			//player->gx = player->rx / BLOCK_SIZE_PIXELS;

			if(grid_y >= 0 && grid_y < grid->h && grid->pos[player->gx][grid_y] != IMMOBILE_BLOCK && grid->pos[player->gx+1][grid_y] != IMMOBILE_BLOCK){
				player->ry += player->jump_velocity;//get_max_fall(grid, player);
				player->jump_velocity -= 1;
			}else{
				//get first empty block and set y to it
				/*int i=0;
				while(i < grid->h && (grid->pos[player->gx][i] != EMPTY_BLOCK || grid->pos[player->gx+1][i] != EMPTY_BLOCK) ){
					i++;
					printf("A i=%d i < grid->h : %d gp: gp1:\n", i, i < grid->h);
				}*/

				//do the above but from the top
				int i=player->gy;//grid->h-1;
				while(i > 1 && (grid->pos[player->gx][i] == EMPTY_BLOCK && grid->pos[player->gx+1][i] == EMPTY_BLOCK) ){
					i--;
				}
				i++;

				//its rendering at the top of the screen cause the loop goes to the end...
				player->ry = (i) * BLOCK_SIZE_PIXELS; //if you add one to i here it produces a bouncing effect
				player->is_jumping = FALSE;
				player->jump_velocity = 0;
			}
		}else{
			printf("player->rx mod BLOCK_SZIE_PIXELS == 0");
			if(grid_y >= 0 && grid_y < grid->h && grid->pos[player->gx][grid_y] != IMMOBILE_BLOCK){
				player->ry += player->jump_velocity;//get_max_fall(grid, player);
				player->jump_velocity -= 1;
			}else{
				//get first empty block and set y to it
				/*int i=0;
				while(i < grid->h && grid->pos[player->gx][i] != EMPTY_BLOCK){
					i++;
					printf("B i=%d i < grid->h : %d\n", i, i < grid->h);
				}*/

				int i = player->gy;//grid->h-1;
				while(i > 1 && grid->pos[player->gx][i] == EMPTY_BLOCK || future_y < i   ){
					//if(grid->pos[player->gx][i] == EMPTY_BLOCK || future_y < i)
						i--;

				}
				i++;

				player->ry = (i) * BLOCK_SIZE_PIXELS;
				player->is_jumping = FALSE;
				player->jump_velocity = 0;
			}
		}
	}
	//printf("here!!");
	if(!can_fall(player, grid)){
		//player->ry -= 2;
		player->is_jumping = FALSE;
		player->jump_velocity = 0;
	}
	

	


	printf("gx = %d gy = %d\n", player->gx, player->gy);
}

void fire(PlatformPlayer* player){
	if(player->projectile_count+1 <= MAX_PROJECTILES){//otherwise they're reloading until the projectiles disapppear or hit something
		int index = player->projectile_count++;

		//don't need to alloc it is included
		//player->projectiles[index] = (Projectile)malloc(sizeof(Projectile));

		//now set stuff on the projectile
		Projectile* proj = &(player->projectiles[index]);
		proj->gy = player->gy;
		proj->ry = player->ry - BLOCK_SIZE_PIXELS/2;
		proj->type = PROJ_BULLET; //TODO just for testing determine this first
		if(player->direction == LEFT){
			proj->gx = player->gx-1;
			proj->rx = player->rx;
			proj->direction = LEFT;
		}else{
			proj->gx = player->gx+1;
			proj->rx = player->rx + BLOCK_SIZE_PIXELS;
			proj->direction = RIGHT;
		}
	}
}



void set_move(PlatformPlayer* player, int direction){
	if(direction == LEFT){
		player->is_moving_left = TRUE;
		player->is_moving_right = FALSE;
		player->direction = LEFT;
	}
	else if(direction == RIGHT){
		player->is_moving_right = TRUE;
		player->is_moving_left = FALSE;
		player->direction = RIGHT;
	}

	printf("player LEFT: %d\n", player->is_moving_left);
	printf("player RIGHT: %d\n", player->is_moving_right);
}

void unset_move(PlatformPlayer* player){
	if(player->is_moving_left)
		player->is_moving_left = FALSE;
	if(player->is_moving_right)
		player->is_moving_right = FALSE;
}

void move_player(Grid* grid, PlatformPlayer* player, int direction){
	AvailableMoves moves = get_available_moves(player, grid);
	printf("n=%d s=%d e=%d w=%d\n", moves.n, moves.s, moves.e, moves.w);
	switch(direction){
		case UP:
			if(moves.n && !player->is_jumping){
				player->is_jumping = TRUE;
				player->jump_velocity = JUMP_START_VELOCITY;
				//player->ry -= JUMP_START_VELOCITY;
			}
			//if(moves.n){
			//	player->ry += 5;
			//}
			break;
		case DOWN:
			if(moves.s){
				player->ry -= 5;
			}
			break;
		/*case LEFT:
			if(can_move_west(player, grid)){
				player->rx -= 5;
			}
			break;
		case RIGHT:
			if(can_move_east(player, grid)){
				player->rx += 5;
			}
			break;*/
		case JUMP:
			break;
		default:
			break;
	}

	printf("player->rx=%d player->ry=%d\n", player->rx, player->ry);

	//TODO update the grid position for logical ops
	//TODO bug when (between blocks.... On the 5 or something)
	int half = BLOCK_SIZE_PIXELS/2;
	player->gx = (player->rx) / BLOCK_SIZE_PIXELS; //integer division
	player->gy = (player->ry) / BLOCK_SIZE_PIXELS;
}

void render_player(PlatformPlayer* player, SDL_Surface* render_dest){
	//printf("player->rx=%d player->ry=%d\n", player->rx, player->ry);
	SDL_Rect player_rect = {player->rx, 
							GRID_HEIGHT*BLOCK_SIZE_PIXELS - player->ry,
							BLOCK_SIZE_PIXELS,	//this is defined in grid.h
							BLOCK_SIZE_PIXELS};
	SDL_FillRect(render_dest, &player_rect, SDL_MapRGB(render_dest->format, 0xFF, 0xFF, 0xFF));

	//render each of the projectiles as well
	int i;
	for(i=0; i < player->projectile_count; i++){
		Projectile* proj = &(player->projectiles[i]);
		SDL_Rect proj_rect = {proj->rx,
							  GRID_HEIGHT*BLOCK_SIZE_PIXELS - proj->ry,
							  1,
							  1};
		SDL_FillRect(render_dest, &proj_rect, SDL_MapRGB(render_dest->format, 0xFF, 0xFF, 0x00));
	}
}

//for each of these do the math of moving 5 in that direction then decide
bool can_jump(PlatformPlayer* player, Grid* grid){

}

//TODO add error checking
static bool can_move_east(PlatformPlayer* player, Grid* grid){
	printf("can_move_east");
	int future_x = player->rx+10; //10 is player size... note this somewhere

	int gfx = future_x / BLOCK_SIZE_PIXELS;

	if(gfx < 0 || gfx > grid->w-1)
		return FALSE;
	else
		return grid->pos[gfx][player->gy] != IMMOBILE_BLOCK;
}
//TODO error checking, works otherwise
static bool can_move_west(PlatformPlayer* player, Grid* grid){
	printf("can_move_west");
	int future_x = player->rx-5;

	int gfx = future_x / BLOCK_SIZE_PIXELS;

	if(gfx < 0 || gfx > grid->w-1)
		return FALSE;
	else
		return grid->pos[gfx][player->gy] != IMMOBILE_BLOCK;
}

bool can_fall(PlatformPlayer* player, Grid* grid){
	int future_y = player->ry-2 ;

	int gfy = future_y / BLOCK_SIZE_PIXELS;

	if(gfy <= 0)
		return FALSE;

	if(player->rx % BLOCK_SIZE_PIXELS > 0)//then check the next column as well (player split between)
		return grid->pos[player->gx][gfy] != IMMOBILE_BLOCK && grid->pos[player->gx+1][gfy] != IMMOBILE_BLOCK;

	return grid->pos[player->gx][gfy] != IMMOBILE_BLOCK;
}

//TODO make this compute future spot then decide (its much more accurate)
AvailableMoves get_available_moves(PlatformPlayer* player, Grid* grid){
	int gx = player->gx;
	int gy = player->gy; //have to flip the y coord to match y logical coords
	AvailableMoves moves;

	printf("gx=%d gy=%d\n", gx, gy);
	printf("IMMOBILE_BLOCK=%d MOVING_BLOCK=%d\n", IMMOBILE_BLOCK, MOVING_BLOCK);
	//printf("[gx][gy+1]=%d [gx][gy-1]=%d [gx-1][gy]=%d [gx+1][gy]=%d\n",grid->pos[gx][gy+1],grid->pos[gx][gy-1],grid->pos[gx+1][gy],grid->pos[gx-1][gy]);

	//check north
	if(gy < grid->h-1 && grid->pos[gx][gy+1] != IMMOBILE_BLOCK){
		printf("setting n to TRUE\n");
		moves.n = TRUE;
	}else{
		printf("setting n to FALSE\n");
		moves.n = FALSE;
	}

	//BUG seems to return true when gy is at a limit

	//check south
	if(gy > 0 && grid->pos[gx][gy-1] != IMMOBILE_BLOCK){
		printf("setting s to TRUE\n");
		moves.s = TRUE;
	}else{
		printf("setting s to FALSE\n");
		moves.s = FALSE;
	}

	//check east (right)
	if(gx < grid->w-1 && grid->pos[gx+1][gy] != IMMOBILE_BLOCK){
		printf("setting e to TRUE\n");
		moves.e = TRUE;
	}else{
		printf("setting e to FALSE\n");
		moves.e = FALSE;
	}

	//check west (left)
	if(gx > 0 && grid->pos[gx-1][gy] != IMMOBILE_BLOCK){
		printf("setting w to TRUE\n");
		moves.w = TRUE;
	}else{
		printf("setting w to FALSE\n");
		moves.w = FALSE;
	}

	//TODO the other directions

	return moves;
}

//may get more complex if I change this struct
void free_player(PlatformPlayer* player){
	free(player);
}