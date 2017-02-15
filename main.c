#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "grid.h"
#include "menu.h"
#include "platform_player.h"

#include "tetris_player.h"

//change the below to whatever the final brick size is mulitplied by GRID_DIMENSION
#define SCREEN_WIDTH GRID_WIDTH * 10 
#define SCREEN_HEIGHT GRID_HEIGHT * 10

#define FPS 30

SDL_Window *window = NULL;

SDL_Surface *screen_surface = NULL;

//SDL_Surface *load_png(char* img_path);

int init_sdl();
void cleanup_sdl();
void render_grid(Grid* g, SDL_Surface *png_surface, SDL_Surface *render_dest);

int main(int argc, char* args[]){

	if(init_sdl() != 0){
		return -1;
	}

	//prepare the grid
	Grid *g  = create_grid();
	init_grid();


	//load image
	SDL_Surface *png_surface = load_png("blue_brick.png", screen_surface->format);

	//TODO yay works now we need to integrate these into separate files font.h font.c etc
	//load font
	TTF_Font *font = TTF_OpenFont("FORCED SQUARE.ttf", 20);
	if(font == NULL){
		printf("Error loading font. SDL2_ttf Error: %s\n", TTF_GetError() );
	}
	SDL_Color text_color = {0xff, 0xff, 0xff};

	//create a surface we can render form the font
	SDL_Surface *text_surface = TTF_RenderText_Solid(font, "hello world!", text_color);
	if(text_surface == NULL){
		printf("Could not render font to texture. SDL2_ttf Error: %s\n", TTF_GetError() );
	}

	int text_count = 4;
	char** options_text = malloc(sizeof(char*)*text_count);
	options_text[0] = "Single Player";
	options_text[1] = "Multi Player";
	options_text[2] = "Settings";
	options_text[3] = "Quit";
	SDL_Color normal_color = {0xFF, 0xFF, 0xFF};
	SDL_Color highlight_color = {0xFF, 0xFF, 0x00};
	Menu* menu = create_menu("blue_brick.png", screen_surface, options_text, text_count, 20, "FORCED SQUARE.ttf", normal_color, highlight_color);

    char** sp_opt_text = malloc(sizeof(char*)*text_count);
	sp_opt_text[0] = "Platform Player";
	sp_opt_text[1] = "Tetris Player";
	sp_opt_text[2] = "Back";
	Menu* sp_menu = create_menu("blue_brick.png", screen_surface, sp_opt_text, 3, 20, "FORCED SQUARE.ttf", normal_color, highlight_color);
	//render grid
	//TODO need to add things for players, god blocks, bullets etc into rendering algorithm
	int i,j;
	struct SDL_Rect draw_pos;
	draw_pos.x = 0;
	draw_pos.y = SCREEN_HEIGHT;
	draw_pos.w = png_surface->w;
	draw_pos.h = png_surface->h;
	for(i=0; i < GRID_WIDTH; i++){
		for(j=0; j < GRID_HEIGHT; j++){
			if(g->pos[i][j] == IMMOBILE_BLOCK){ //if there is a block we draw
				SDL_BlitSurface(png_surface, NULL, screen_surface, &draw_pos);
				//SDL_FillRect(screen_surface, &draw_pos, SDL_MapRGB( screen_surface->format, 0x00, 0x00, 0xFF) );
			}
			draw_pos.y -= png_surface->h;; //move up the column
		}
		draw_pos.x += png_surface->w;; //move to the right
		draw_pos.y = SCREEN_HEIGHT; //reset at bottom of colummn
	}

	//testregion
	struct SDL_Rect rect;
	rect.x = 0;
	rect.y = 20;
	rect.w = 100;
	rect.h = 200;

	//render png to surface
	SDL_BlitSurface(text_surface, NULL, screen_surface,  &rect ); //renders the little cube thing, probably make bigger ones

	//SDL_FillRect(screen_surface, &rect, SDL_MapRGB( screen_surface->format, 0x00, 0x00, 0xFF) );


	//test main loop
	Menu* current_menu = menu;
	int quit = FALSE, mx = 0, my = 0;
	SDL_Rect clear_rect = {0,0,screen_surface->w, screen_surface->h};
	SDL_Color clear_color = {0x00, 0x00, 0x00};

	//test player
	PlatformPlayer* player = create_platform_player(g);

	//test tetris player
	TetrisPlayer* t_player = create_tetris_player(30, 14);

	int pressedFire = FALSE;

	SDL_Event e;
	long frame_count = 0;
	while(!quit){
		//eval queued events
		while( SDL_PollEvent(&e) != 0 ){
			const Uint8* key_states = SDL_GetKeyboardState(NULL);
			if(key_states[SDL_SCANCODE_UP] || key_states[SDL_SCANCODE_W]){
				move_player(g, player, UP);
				rotate_tetris_player(t_player, g);
			}

			if(key_states[SDL_SCANCODE_SPACE] || key_states[SDL_SCANCODE_SPACE]){
				//TODO make sure oonly hit once
				if(!pressedFire)
					fire(player);
				pressedFire = TRUE;
			}else{
				pressedFire = FALSE;
			}

			if(key_states[SDL_SCANCODE_LEFT] || key_states[SDL_SCANCODE_A]){
				set_move(player, LEFT);
				move_tetris_player(t_player, g, LEFT);
			}
			else if(key_states[SDL_SCANCODE_RIGHT] || key_states[SDL_SCANCODE_D]){
				set_move(player, RIGHT);
				move_tetris_player(t_player, g, RIGHT);
			}
			else{
				unset_move(player);
			}

			if(key_states[SDL_SCANCODE_DOWN] || key_states[SDL_SCANCODE_S]){ 
				move_tetris_player(t_player, g, DOWN);
			}

			switch(e.type){
				case SDL_QUIT:
					printf("received quit event. Exiting...\n");
					quit = TRUE;
					break;
/*				case SDL_KEYDOWN:
					switch(e.key.keysym.sym){
						case SDLK_UP:
						case SDLK_w:
							printf("Up arrow pressed.\n");
							move_player(g, player, UP);
							break;
						case SDLK_DOWN:
						case SDLK_s:
							printf("Down arrow pressed.\n");
							move_player(g, player, DOWN);
							break;
						case SDLK_LEFT:
						case SDLK_a:
							printf("Left arrow pressed.\n");
							//move_player(g, player, LEFT);
							set_move(player, LEFT);
							break;
						case SDLK_RIGHT:
						case SDLK_d:
							printf("Right arrow pressed.\n");
							//move_player(g, player, RIGHT);
							set_move(player, RIGHT);
							break;
						
					}
				case SDL_KEYUP:
					switch(e.key.keysym.sym){
						case SDLK_UP:
						case SDLK_w:
							//printf("Up arrow pressed.\n");
							//move_player(g, player, UP);
							break;
						case SDLK_DOWN:
						case SDLK_s:
							//printf("Down arrow pressed.\n");
							//move_player(g, player, DOWN);
							break;
						case SDLK_LEFT:
						case SDLK_a:
							printf("Left arrow released.\n");
							//move_player(g, player, LEFT);
							unset_move(player);
							break;
						case SDLK_RIGHT:
						case SDLK_d:
							printf("Right arrow released.\n");
							//move_player(g, player, RIGHT);
							unset_move(player);
							break;
						
					}*/
				case SDL_MOUSEMOTION:
					SDL_GetMouseState(&mx, &my);
					break;
				case SDL_MOUSEBUTTONDOWN:
					printf("Mouse button clicked.\n");
					printf("Option %s selected.\n", current_menu->selected_option);
					if(current_menu->selected_option != NULL){
						if(strcmp("Single Player", current_menu->selected_option) == 0){
							current_menu = sp_menu;
						}
						else if(strcmp("Multi Player", current_menu->selected_option) == 0){

						}
						else if(strcmp("Settings", current_menu->selected_option) == 0){

						}
						else if(strcmp("Quit", current_menu->selected_option) == 0){
							printf("User selected Quit option. Exiting...");
							quit = TRUE;
						}
						else if(strcmp("Back", current_menu->selected_option) == 0){
							current_menu = menu;
						}
					}
					break;
				case SDL_MOUSEBUTTONUP:
					printf("Mouse button released.\n");
					break;
				//TODO all the other ones
			}
		}

		//run updates from players
		update_platform_player(g, player);

		if(frame_count % FPS == 0)//every 1/2 second
			update_tetris_player(t_player, g); //TODO make your apis consistent

		//first we clear the screen
		SDL_FillRect(screen_surface, &clear_rect, SDL_MapRGB( screen_surface->format, 0x00, 0x00, 0x00) );

		render_grid(g, png_surface, screen_surface);
		render_player(player, screen_surface);
		render_tetris_player(t_player, screen_surface);
		render_menu(current_menu, screen_surface, mx, my);

		SDL_UpdateWindowSurface(window);

		//30 frames/second fixed loop
		//TODO improve to compute how long this frame took and decide if we can afford to wait or must skip ahead
		frame_count++;
		SDL_Delay(1000/FPS);
	}


	

	cleanup_sdl();

	free_grid(g);
	//free_menu(menu);

	//free our surface
	SDL_FreeSurface(png_surface);
	SDL_FreeSurface(text_surface);

	//free surfaces, probably make management files for them
	
	return 0;
}

int init_sdl(){
	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		printf("SDL2 could not initialize. Error: %s\n", SDL_GetError() );
		return -1;
	}

	
	window = SDL_CreateWindow("Battle Tetris", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if(window == NULL){
		printf("Window could not be created. Error: %s\n", SDL_GetError() );
		return -1;
	}

	//init PNG loading
	int img_flags = IMG_INIT_PNG;
	if( !( IMG_Init(img_flags) & img_flags) ){
		printf("SDL_image could not be initialized. Error: %s\n", IMG_GetError());
		return -1;
	}

	//init ttf loading
	if( TTF_Init() == -1){
		printf("SDL_ttf could not init. Error: %s\n", TTF_GetError() );
	}

	//init screen surface
	screen_surface = SDL_GetWindowSurface(window);

	return 0;
}

//TODO probably move this into grid.h, grid.c
void render_grid(Grid* g, SDL_Surface *png_surface, SDL_Surface *render_dest){
	int i,j;
	struct SDL_Rect draw_pos;
	draw_pos.x = 0;
	draw_pos.y = SCREEN_HEIGHT;
	draw_pos.w = png_surface->w;
	draw_pos.h = png_surface->h;
	for(i=0; i < GRID_WIDTH; i++){
		for(j=0; j < GRID_HEIGHT; j++){
			if(g->pos[i][j] == IMMOBILE_BLOCK){ //if there is a block we draw
				SDL_BlitSurface(png_surface, NULL, render_dest, &draw_pos);
				//SDL_FillRect(screen_surface, &draw_pos, SDL_MapRGB( screen_surface->format, 0x00, 0x00, 0xFF) );
			}
			draw_pos.y -= png_surface->h;; //move up the column
		}
		draw_pos.x += png_surface->w;; //move to the right
		draw_pos.y = SCREEN_HEIGHT; //reset at bottom of colummn
	}
}

void cleanup_sdl(){
	SDL_DestroyWindow(window);

	SDL_Quit();
}

/*TTF_Font *load_font(char* font_path, int pt){

	TTF_Font font = TTF_OpenFont(font_path, pt);
	if(font == NULL){
		printf("Failed to load font. SDL_ttf Error: %s\n", TTF_GetError() );
	}
	return font;
}*/



/*SDL_Surface *load_png(char* img_path){
	//img loading code put into function
	SDL_Surface *opt_surface = NULL;

	SDL_Surface *png_surface = IMG_Load(img_path);
	if(png_surface == NULL){
		printf("Unable to load image. Error: %s\n", IMG_GetError() );
	}

	//now optimize img surface
	SDL_Surface *image_surface = SDL_ConvertSurface(png_surface, screen_surface->format, 0); //no flags
	if(image_surface == NULL){
		printf("Unable to optimize surface. Error: %s\n", SDL_GetError() );
	}

	SDL_FreeSurface(png_surface);

	return image_surface;
	//END immg loading code wrap it in a function
}*/