#ifndef GRID_H
#define GRID_H


#define GRID_WIDTH 60
#define GRID_HEIGHT 20
#define BLOCK_SIZE_PIXELS 10
#define MAX_BLOCK_HEIGHT GRID_HEIGHT - 5
#define MAX_DIFF 4

//Flag values for grid block contents
#define EMPTY_BLOCK    0x00
#define BULLET         0x01
#define MISSILE        0x02
#define PLAYER         0x04

#define PASSABLE EMPTY_BLOCK | BULLET | MISSILE | PLAYER 

#define MOVING_BLOCK   0x08
#define IMMOBILE_BLOCK 0x10

#define IMPASSABLE MOVING_BLOCK | IMMOBILE_BLOCK


//error return values
#define GRID_NOT_INITIATED  -1
#define INVALID_GRID_COORDS -2

typedef struct Grid{
	int w;
	int h;
	int** pos; //the actual grid coords (1d array) access like Grid->pos[i*w+j]
} Grid;

int grid[GRID_WIDTH][GRID_HEIGHT];

//function declarations

Grid *create_grid();

int init_grid();

void free_grid(Grid *grid);

//testing only
int* generate_random_walk();

#endif