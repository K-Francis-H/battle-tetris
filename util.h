//util.h

#include <math.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#define TRUE 1
#define FALSE 0

typedef char bool;

//probably switch to float
typedef struct Vector2{
	int x,y;
} Vector2;





int random_range(int min, int max);

SDL_Surface *load_png(char* img_path, SDL_PixelFormat* pixel_format);

int point_is_in_rect(int px, int py, int rx, int ry, int rw, int rh);



//int load_png(char* img_path, SDL_Surface* image_surface, SDL_Surface* rendering_surface);
