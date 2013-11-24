#include <stdio.h>
#include "Windows.h"
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include "time.h"
#include <math.h>

//definitions
#define MAX_COUNT  150
#define false 0 // c++ adaption
#define true 1  // c++ adaption

#define DEBUG 1 // this is the varibale that tells us whether we want to debug or not.

//allowing us to efficiently use the 'bool' type
#define bool char // 1 byte of information for a bool. not perfect, but better than using 4 bytes! :D
#define false 0
#define FALSE 0
#define true 1
#define TRUE 1

//The CONSTANT initial size of the screen
#define DEFAULT_SCREEN_WIDTH 896
#define DEFAULT_SCREEN_HEIGHT 576
//The DYNAMIC  initial size of the screen
unsigned int SCREEN_WIDTH = DEFAULT_SCREEN_WIDTH;
unsigned int SCREEN_HEIGHT = DEFAULT_SCREEN_HEIGHT;


#define NUMBER_OF_GUI_SIZES 3
// this is the native image size (32x32 pixels)
#define ITEM_SIZE_BASE 0x20
// this is the gui size. it corresponds to how large the item sizes will be. 1=32, 2=64, 3=96, etc...
// this number indexes into the item_set[] array.
unsigned short GUI_SIZE = 2;
#define ITEM_SIZE (GUI_SIZE*ITEM_SIZE_BASE)
//unsigned short ITEM_SIZE[NUMBER_OF_GUI_SIZES] = {0x20, 0x40, 0x80, 0x100}; // 13,32,48, and 64
// item_set[0] is null. it is not inteneded to be used.
// ietm_set[1] is the normal size
// item_set[2] is twice the size
// item_set[3] is three times the size
// etc...
SDL_Surface *item_set[NUMBER_OF_GUI_SIZES+1] = 	{NULL,NULL,NULL,NULL};	// this is what holds all of the item sprites

#define SCREEN_BPP 32

//global variable that tells us if the game is paused or not
int paused = 0;
// global variable that tells you the FPS of the game.
int FPS = 0;

// The surfaces that will be used
SDL_Surface *screen = 		NULL;	// this is the surface the player sees.
SDL_Surface *gridSurface =	NULL;	// this holds the printout of the world grid.
SDL_Surface *skySurface =	NULL;	// this holds the gradient for the sky.
SDL_Surface *text = 		NULL;	// this is a general purpose text surface.
SDL_Surface *icon = 		NULL;	// this holds the icon for the game.
// The event structure that will be used
SDL_Event event;

// the fint that will be used
TTF_Font *font = NULL;
TTF_Font *font16=NULL;

//The color of the text
SDL_Color textColor = { 255, 255, 255 };

// id for changing materials
int currentMat = 1;

// mouse moddifier used for brushes
short mouseModifier = 0;

// necessary function prototypes.
// this is so that functions that use THESE functions can be compiled before THESE functions are implemented.
void setcell(int , int, int);
void apply_surface( int x, int y,  SDL_Surface* source, SDL_Surface* destination );
void apply_surface_clips( int x, int y,  SDL_Surface *source, SDL_Surface *destination, SDL_Rect *clip );
Uint32 get_pixel(SDL_Surface *surface, int x, int y);
void scale_surface(SDL_Surface *sour, SDL_Surface *dest, short scalingFactor);

// include all of the other header files that have proprietary functions and variables that will be used throughout the program.
#include "general_functions.h"
#include "materials_and_cells.h"
#include "npc.h"
#include "items.h"
#include "quests.h"
#include "player.h"
#include "player_movement.h"
#include "cell_engine.h"
#include "graphics.h"
#include "map_editor.h"
#include "world_gen.h"



