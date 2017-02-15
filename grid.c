#include <math.h>
#include <stdlib.h>

#include "grid.h"
#include "util.h"

int grid[GRID_WIDTH][GRID_HEIGHT];
int is_init = FALSE;

//this doesn't work right I am not allocing enough...
Grid *create_grid(){
	printf("%d\n", (int)sizeof(Grid));
	Grid *grid = (Grid*)malloc(sizeof(Grid));
	grid->w = GRID_WIDTH;
	grid->h = GRID_HEIGHT;

	int *cols = generate_random_walk();

	//TODO warning here be wary
	grid->pos = malloc(sizeof(int*)*GRID_WIDTH);

	//printf("%d, %d, %d\n", grid->w, grid->h, (int) (sizeof(int)*GRID_WIDTH*GRID_HEIGHT)) ;

	int i,j,height;
	for(i=0; i < GRID_WIDTH; i++){
		height = cols[i];
		grid->pos[i] = malloc(sizeof(int)*GRID_HEIGHT);
		for(j=0; j < GRID_HEIGHT; j++){
			//printf("i=%d j=%d\n", i, j);
			if(j < height){
				grid->pos[i][j] = IMMOBILE_BLOCK;
			}
			else{
				grid->pos[i][j] = EMPTY_BLOCK;
			}
		}
	}

	free(cols);

	return grid;
}

int init_grid(){
	
	//create array of columns
	int *cols = generate_random_walk();

	//then load these into our grid
	int i,j,height;
	for(i=0; i < GRID_WIDTH; i++){
		height = cols[i];
		for(j=0; j < GRID_HEIGHT; j++){
			if(j < height){
				grid[i][j] = IMMOBILE_BLOCK;
			}
			else{
				grid[i][j] = EMPTY_BLOCK;
			}
		}
	}

	free(cols);
	is_init = TRUE;
	return 0;
}


void free_grid(Grid *grid){
	//free the second dimension of the grid->pos
	int i;
	for(i=0; i < grid->w; i++){
		free(grid->pos[i]);
	}
	//free the first dimension
	free(grid->pos);

	//free the rest of the object
	free(grid);
}

int *generate_random_walk(){
	int *col_heights = malloc(sizeof(int) * GRID_WIDTH);
	int i, last, next;
	for(i=0; i < GRID_WIDTH; i++){
		if(i == 0){
			col_heights[i] = random_range(0, MAX_BLOCK_HEIGHT);
		}else{
			last = col_heights[i-1];
			if(last - MAX_DIFF < 0)
				col_heights[i] = random_range(0, last+MAX_DIFF);
			else if(last + MAX_DIFF > MAX_BLOCK_HEIGHT)
				col_heights[i] = random_range(last-MAX_DIFF, MAX_BLOCK_HEIGHT);
			else
				col_heights[i] = random_range(last-MAX_DIFF, last+MAX_DIFF);
		}
	}
	return col_heights;
}