#include <SDL2/SDL.h>


#include "grid.h"

#define INCLUDED_PLATFORM_PLAYER 1

#define MAX_PROJECTILES 5

#define PROJ_BULLET 1
#define PROJ_MISSILE 2

#define GUN 1
#define MISSILE_LAUNCHER 2
#define DRILL 3

#define UP    1
#define DOWN  2
#define LEFT  3
#define RIGHT 4
#define JUMP  5

#define JUMP_START_VELOCITY 10;

typedef struct AvailableMoves{
	bool n,s,e,w,ne,nw,se,sw;
} AvailableMoves;

typedef struct Projectile{
	int gx, gy;
	int rx, ry;
	//Vector2 direction;
	int direction;
	int type;
} Projectile;

typedef struct Projectile_LL_Node{
	Projectile cur;
	Projectile* next;
} Projectile_LL_Node;

typedef struct PlatformPlayer{
	int gx,gy;	//position on grid
	int rx,ry;	//position on screen (pixel coords)
	//Vector2 direction;
	int weapon;
	Projectile projectiles[MAX_PROJECTILES];
	Projectile_LL_Node* head;
	int projectile_count;
	int is_moving_left;
	int is_moving_right;
	int direction;
	int is_jumping;
	int jump_velocity;
	Grid* grid;
} PlatformPlayer;

PlatformPlayer* create_platform_player(Grid *grid);

void update_platform_player(Grid* grid, PlatformPlayer *player); //gonna need more here

void render_player(PlatformPlayer *player, SDL_Surface *render_dest);

void free_player(PlatformPlayer *player);

AvailableMoves get_available_moves(PlatformPlayer* player, Grid* grid);

//projectile functions
Projectile *create_projectile(PlatformPlayer *player);


void render_projectile(); //TODO