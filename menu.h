#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

/*
typedef struct Menu{
	SDL_Surface* bg_image;
	char** options_text;			//below
	int options_text_count;			//maybe dont store these, just the renderable surfaces
	TTF_Font* text_font;			//likewise once rendered it doesn't matter
	SDL_Color* text_color_unhighlight;	//same
	SDL_Color* text_color_highlight; 	//same
} Menu;*/


//TODO figure out how to add positioning data to the struct
typedef struct MenuOption{
	char* text;
	SDL_Surface* normal_text;
	SDL_Surface* highlight_text;
	SDL_Rect area;
	//TODO put sub menu or direction to go
} MenuOption;

typedef struct Menu{
	SDL_Surface* bg_image;
	int text_count;
	MenuOption* options;
	char* selected_option;
	//SDL_Surface** normal_text;
	//SDL_Surface** highlight_text;
} Menu;

//const char *unique_text[] = {"Single Player", "Multi Player", "Settings", "Quit"};

//char (*TOP_MENU_OPT_TEXT)[4] = TOP_TEXT;
//char (*SP_MENU_OPT_TEXT)[3] = {"Platform Player", "Tetris Player", "Back"};
//char (*MP_MENU_OPT_TEXT)[1] = {"Back"}; //TODO
//char (*SET_MENU_OPT_TEXT)[1] = {"Back"}; //TODO


Menu* create_menu(char *bg_image_path, SDL_Surface* render_dest, char** options_text, int text_count, int text_size_pt, char* font_path, SDL_Color text_color_unhighlight, SDL_Color text_color_highlight);

//same as above but uses default colors WHOOPS no type overloading
//Menu* create_menu(char *bg_image_path, char** options_text, int text_count, char* font_path);

void render_menu(Menu* menu, SDL_Surface* render_dest, int mouse_x, int mouse_y);

void free_menu(Menu* menu);