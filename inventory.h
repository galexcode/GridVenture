

///---------------------------------------------------------------------
///---------------------- INVENTORY STRUCTURES -------------------------
///---------------------------------------------------------------------

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
#define MAX_INVENTORY_WIDTH	32
#define MAX_INVENTORY_HEIGHT 16
#define MAX_INVENTORY_LENGTH (MAX_INVENTORY_WIDTH*MAX_INVENTORY_HEIGHT)
#define DEFAULT_PLAYER_INVENTORY_WIDTH 8
#define DEFAULT_PLAYER_INVENTORY_HEIGHT 4
#define MAX_INVENTORIES_OPEN 4

/// this is a general purpose inventory it is a collection of inventory slots.
// this is used for the player as well as storage chests and possibly even NPCs.
struct inventoryData{
	// this is where the inventory is on the screen.
	// (x,y) is the the upper left point of the inventory (not including the boundary)
	int x,y;
	// this is the number of item slots wide the inventory is.
	unsigned char width;
	// this is the number of item slots tall the inventory is.
	unsigned char height;
	// this holds all of the data of the items in the inventory
	struct inventorySlot slot[MAX_INVENTORY_LENGTH];
};

// this holds all of the inventories that are open (on the player's screen)
// this list helps us evaluate the player's mouse click events
struct inventoryData *openInvs[MAX_INVENTORIES_OPEN];


///---------------------------------------------------------------------
///---------------------- INVENTORY FUNCTIONS --------------------------
///---------------------------------------------------------------------


/// this erases an inventory slot
void inventory_slot_erase(struct inventorySlot *datslot){
	datslot->item = i_none;
	datslot->quantity = 0;
	datslot->uses = 0;
}


void inventory_erase(struct inventoryData *datinv){
	// set default inventory location to (0,0)
	datinv->x = 0;
	datinv->y = 0;
	int i;
	// loop through every inventory item and erase it.
	for(i=0; i<MAX_INVENTORY_LENGTH; i++){
			inventory_slot_erase(&datinv->slot[i]);
	}
}

// this is the thickness of the inventory border
#define INVENTORY_BORDER (ITEM_SIZE/4)
// this will evaluate mouseclicks for all the inventories open in the game.
/// returns whether or not the user clicked on an open inventory
bool evaluate_inventories(int x, int y){
	
	bool clickedAnInv = false;				// this keeps track of an inventory was clicked
	SDL_Rect invRect;						// this is a rectangle used to describe the size/area/location of the inventory
	int i;									// loop increment variable
	
	for(i=0; i<MAX_INVENTORIES_OPEN; i++){
		if(openInvs[i] == NULL) continue;								// don't evaluate a NULL inventory pointer.
		invRect.x = openInvs[i]->x - INVENTORY_BORDER;					// generate rectangle for the entire gui
		invRect.y = openInvs[i]->y - INVENTORY_BORDER;
		invRect.w = openInvs[i]->width*ITEM_SIZE  + 2*INVENTORY_BORDER;
		invRect.h = openInvs[i]->height*ITEM_SIZE + 2*INVENTORY_BORDER;
		if(within_rect(&invRect,x,y)){
			clickedAnInv = true;
		}
	}
	return clickedAnInv;
}


