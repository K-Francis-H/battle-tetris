#include "menu.h"
#include "util.h"

Menu* create_menu(char *bg_image_path, SDL_Surface* render_dest, char** options_text, int text_count, int font_size_pt, char* font_path, SDL_Color text_color_unhighlight, SDL_Color text_color_highlight){

	//load image surface
	SDL_Surface *bg_img = load_png(bg_image_path, render_dest->format);

	//load font
	TTF_Font *font = TTF_OpenFont(font_path, font_size_pt);

	int center_x = render_dest->w/2;

	//create text surfaces from font, options text
	MenuOption *menu_options = malloc(sizeof(MenuOption)*text_count);
	//SDL_Surface **text_normal = malloc(sizeof(SDL_Surface*)*text_count);
	//SDL_Surface **text_highlight = malloc(sizeof(SDL_Surface*)*text_count);
	int i;
	for(i=0; i < text_count; i++){
		//for each text create a surface that can be rendered
		menu_options[i].text = options_text[i];
		printf("Setting option text to %s for option %d.\n", menu_options->text, i);
		menu_options[i].normal_text = TTF_RenderText_Solid(font, options_text[i], text_color_unhighlight);
		menu_options[i].highlight_text = TTF_RenderText_Solid(font, options_text[i], text_color_highlight);
		SDL_Rect area;
		area.x = center_x-menu_options[i].normal_text->w/2;
		area.y = i*menu_options[i].normal_text->h;
		area.w = menu_options[i].normal_text->w;
		area.h = menu_options[i].normal_text->h;
		menu_options[i].area = area;
	}

	Menu* menu = malloc(sizeof(Menu));
	menu->bg_image = bg_img;
	menu->text_count = text_count;
	menu->options = menu_options;
	menu->selected_option = NULL;
	//menu->normal_text = text_normal;
	//menu->highlight_text = text_highlight;

	return menu;
}

//other create_menu

//render menu
void render_menu(Menu* menu, SDL_Surface* render_dest, int mouse_x, int mouse_y){
	//first render the bg_image
	struct SDL_Rect bg_image_rect;
	bg_image_rect.x=0;
	bg_image_rect.y=0;
	bg_image_rect.w = render_dest->w;
	bg_image_rect.h = render_dest->h;

	SDL_BlitSurface(menu->bg_image, NULL, render_dest, &bg_image_rect);

	//compute the center of thee render_dest
	int center_x = render_dest->w/2;

	struct SDL_Rect pos;
	int i;
	for(i=0; i < menu->text_count; i++){
		pos.y = i * menu->options[i].normal_text->h;
		pos.x = center_x - menu->options[i].normal_text->w/2;
		pos.w = menu->options[i].normal_text->w;
		pos.h = menu->options[i].normal_text->h;
		if(point_is_in_rect(mouse_x, mouse_y, pos.x, pos.y, pos.w, pos.h)){
			//set the selected option so that we can properly handle click events
			menu->selected_option = menu->options[i].text;
			printf("Selected option set to %s.\n", menu->selected_option);
			SDL_BlitSurface(menu->options[i].highlight_text, NULL, render_dest, &pos);
		}
		else{
			SDL_BlitSurface(menu->options[i].normal_text, NULL, render_dest, &pos);
		}
	}

}



void free_menu(Menu* menu){
	free(menu->bg_image);

	int i;
	for(i=0; i < menu->text_count; i++){
		free(menu->options[i].text);
		free(menu->options[i].normal_text);
		free(menu->options[i].highlight_text);
	}

	free(menu->options);
	//free(menu->highlight_text);

	free(menu);
}
