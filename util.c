//util.c
#ifndef UTIL_H
#define UTIL_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>



#include "util.h"





int random_range(int min, int max){
	static int is_rand_init = FALSE;
	if(is_rand_init == FALSE){
		srand(time(NULL));
		is_rand_init = TRUE;
	}
	return rand() % (max - min + 1) + min;
}

int point_is_in_rect(int px, int py, int rx, int ry, int rw, int rh){
	if(px >= rx && px <= rx+rw && py >= ry && py <= ry+rh){
		return TRUE;
	}
	else{
		return FALSE;
	}
}

SDL_Surface *load_png(char* img_path, SDL_PixelFormat* pixel_format){
	//img loading code put into function
	SDL_Surface *opt_surface = NULL;

	SDL_Surface *png_surface = IMG_Load(img_path);
	if(png_surface == NULL){
		printf("Unable to load image. Error: %s\n", IMG_GetError() );
	}

	//now optimize img surface
	SDL_Surface *image_surface = SDL_ConvertSurface(png_surface, pixel_format, 0); //no flags
	if(image_surface == NULL){
		printf("Unable to optimize surface. Error: %s\n", SDL_GetError() );
	}

	SDL_FreeSurface(png_surface);

	return image_surface;
	//END immg loading code wrap it in a function
}

/*int clamp_real_pos_to_grid_pos(int rx, int ry, int gw, int gh, int gblock_size){
	if(rx >=0 && rx < gw*gblock_size && ry >= 0 && ry < gh*gblock_size){

	}
	else{
		//return some kind of error
	}
}*/



/*
int load_png(char* img_path, SDL_Surface* image_surface, SDL_Surface* rendering_surface){
	//img loading code put into function
	SDL_Surface *opt_surface = NULL;

	SDL_Surface *png_surface = IMG_Load(img_path);
	if(png_surface == NULL){
		printf("Unable to load image. Error: %s\n", IMG_GetError() );
		return -1;
	}

	//now optimize img surface
	image_surface = SDL_ConvertSurface(png_surface, rendering_surface->format, 0); //no flags
	if(image_surface == NULL){
		printf("Unable to optimize surface. Error: %s\n", SDL_GetError() );
		return - 1;
	}

	SDL_FreeSurface(png_surface);

	return 0;
	//END immg loading code wrap it in a function
}*/
#endif