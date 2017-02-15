//tetris_player.h
#include <SDL2/SDL.h>
#include "grid.h"

#define CLOCKWISE 0
#define COUNTER_CLOCKWISE 1
//#define SQUARE(x,y)

#define PIVOT_INDEX 0 //index for rotating on

#define NUM_BLOCKS 4

#define UP    1
#define DOWN  2
#define LEFT  3
#define RIGHT 4

//TODO maybe track rotation states so that we can have an easier time determining what can be done


#define SQUARE 0	//01
					//23


typedef struct Tetromino{
	int type;
	int rot_index;
	int rot_count;
} Tetromino;




#define S_RIGHT 1	// 01	2	
					//23	30	
					//		 1
	

#define S_LEFT 2	//10	 1
					// 23	20
					//		3

#define L_RIGHT 3	//1		201		32		  3
					//0		3		 0		102
					//23			 1

#define L_LEFT 4	// 1	3		23		102
					// 0	201		0		  3
					//32			1

#define T 5			// 2	1		301		 3
					//103	02		 2		20
					//		3				 1

#define I 6			//1
					//0		1 0 2 3
					//2
					//3



typedef struct TetrisPlayer{
	Vector2 center_pos;
	Tetromino block;
} TetrisPlayer;

//TODO these can just be defined statically inside the tetris_player.c file
//bool can_move_south(TetrisPlayer* t_player, Grid* grid);
//bool can_move_east(TetrisPlayer* t_player, Grid* grid);
//bool can_move_west(TetrisPlayer* t_player, Grid* grid);
//bool can_rotate(TetrisPlayer* t_player, Grid* grid); 

TetrisPlayer* create_tetris_player(int cx, int cy);

void rotate_tetris_player(TetrisPlayer* t_player, Grid* grid);

void move_tetris_player(TetrisPlayer* t_player, Grid* grid, int direction);

void update_tetris_player(TetrisPlayer* t_player, Grid* grid);

void render_tetris_player(TetrisPlayer* t_player, SDL_Surface *render_dest);

void destroy_tetris_player(TetrisPlayer* t_player);
