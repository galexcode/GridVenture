
///-----------------------------------------------------------------------------
///------------ ALL PLAYER RELATED STRUCTURES DEFINED HERE ---------------------
///-----------------------------------------------------------------------------


/// this is a single slot of an inventory. this would be a user inventory or possibly a chest inventory.
struct inventorySlot{
	
	// this is the number that indexes into the items[] array.
	int item;
	
	// this is the number of uses left the item has.
	// breaks at 0 NOT after 0.
	int uses;
	
	// this is how many of this item are in this stack.
	Uint32 quantity;
};



// these describe the inventory maximum sizes.
#define MAX_INVENTORY_WIDTH		32
#define MAX_INVENTORY_HEIGHT	16
#define MAX_INVENTORY_LENGTH (MAX_INVENTORY_WIDTH*MAX_INVENTORY_HEIGHT)
#define DEFAULT_PLAYER_INVENTORY_WIDTH	8
#define DEFAULT_PLAYER_INVENTORY_HEIGHT	4

/// this is a general purpose inventory it is a collection of inventory slots.
// this is used for the player as well as storage chests.
struct inventoryData{
	unsigned char width;
	unsigned char height;
	struct inventorySlot slot[MAX_INVENTORY_LENGTH];
};





#define DEFAULT_PLAYER_WIDTH	1.0f
#define DEFAULT_PLAYER_HEIGHT	3.0f

// jump velocity in cells/millisecond (positive by SDL coordinate convention)
#define GRAVITY_ACCEL 0.00018f
// the fastest you can ever travel in cells/millsecond (positive by SDL coordinate convention)
#define TERMINAL_VELOCITY 0.33f

#define DEFAULT_PLAYER_WALK_SPEED 0.045f
#define DEFAULT_PLAYER_JUMP_VELOCITY -0.110f


struct playerData{
	
	// x and y POSITION		in the grid (using the SDL coordinate system)
	int x_pos; // on the left side of the avater
	int y_pos; // position of feet (lowest point on avatar)
	
	// x and y VELOCITY		in the grid (using the SDL coordinate system)
	float x_vel;
	float y_vel;
	
	// x and y ACCELERATION	in the grid (using the SDL coordinate system)
	float x_accel;
	float y_accel;
	
	// this is the size of the user's avatar
	float height;
	float width;
	
	//the base color of the player
	int color;
	
	// the speed in cells/millisecond at which the player ascends upwards when jumping. (negative by SDL coordinate convention)
	float jumpVelocity;
	float walkSpeed;
	
	// this keeps track of whether or not the player is on a collision material
	bool onCollMat;
	
	// this keeps track of what element of the user's hot bar he/she is on. This indexes through the user's hotbar
	Sint8 hotbarIndex;
	
	// this value is how much time the player has played in the sessions before the current sessions (in seconds)
	// when the player boots up a game, this data is loaded.
	// this data type will roll over after about 139.5 years. (nothing for anyone alive today to be concerned about)
	// needless to say, this value will be set to 0 by default when a new player is created.
	Uint32 playTime;
	
	//this is the player's unventory
	struct inventoryData inv;
	
};

// this is the player.
struct playerData player;


///-----------------------------------------------------------------------------
///------------ ALL PLAYER RELATED STRUCTURE FUNCTIONS DEFINED HERE ------------
///-----------------------------------------------------------------------------


/// this erases an inventory slot
void inventory_slot_erase(struct inventorySlot *datslot){
	datslot->item = i_none;
	datslot->quantity = 0;
	datslot->uses = 0;
}


void inventory_erase(struct inventoryData *datinv){
	int i;
	// loop through every inventory item and erase it.
	for(i=0; i<MAX_INVENTORY_LENGTH; i++){
			inventory_slot_erase(&datinv->slot[i]);
	}
}



// this sets up a blank character
void init_player_attributes(struct playerData *datplayer){
	
	
	datplayer->x_pos =			// set all x position...
	datplayer->x_vel =			// ...velocity...
	datplayer->x_accel =		// ...and acceleration to 0.
	datplayer->y_pos =			// set all y position...
	datplayer->y_vel =			// ...velocity...
	datplayer->y_accel = 0;		// ...and acceleration to 0.
	
	datplayer->width  = DEFAULT_PLAYER_WIDTH;					// set default player dimensions.
	datplayer->height = DEFAULT_PLAYER_HEIGHT;
	
	datplayer->jumpVelocity = DEFAULT_PLAYER_JUMP_VELOCITY;		// this is the speed at which the player jumps up from the ground.
																// it is negative by SDL coordinate system convention.
	
	datplayer->walkSpeed = DEFAULT_PLAYER_WALK_SPEED;			// set default walking speed. this is used for walking left and right.
																// so just enter a positive value. evaluate_player_movement() will do the rest.
	
	datplayer->onCollMat = false;								// the player is, by default, not on a material. this gets evaluated almost immidiately.
	
	datplayer->color = 0x987a5c;								// base color of the player
	
	datplayer->playTime = 0;									// default time the user has played the game in previous sessions.
	
	datplayer->hotbarIndex = 0;								// default to the first one (leftmost)
	
	inventory_erase(&datplayer->inv);							// erase new player's inventory
	datplayer->inv.width =  DEFAULT_PLAYER_INVENTORY_WIDTH;		// set default dimensions for width
	datplayer->inv.height = DEFAULT_PLAYER_INVENTORY_HEIGHT;	// and height
}




/// tests to see if the position is valid for the player to be
bool player_valid_position(struct playerData *datplayer, int x, int y){
	int i,j;
	// index through the places that the player occupies
	for(i=0; i<datplayer->width; i++){
		for(j=0; j<datplayer->height; j++){
			if(mats[grid[x+i][y-j-1].mat].collision) return false; // if you encounter any material inside the player's area, return false. the player is not in a valid position
		}
	}
	return true; // if there are no collision type materials found inside the player, then return true!  :D
}




