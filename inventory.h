

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
// [0] is the top-most inventory			(TOP position in the stack)
// [1] is the second-top-most inventory		(TOP-1 position in the stack)
// ...
// [N-1] is the bottom inventory			(BOTTOM position in the stack)
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

///this organizes the open inventories
// returns the index into the openInvs[] array that is open and the lowest in the stack (the greatest numerical value)
// return value is -1 if there are no open slots
int organize_open_inventories(){
	int i,j;
	
	// this organizes the openInventories
	for(i=MAX_INVENTORIES_OPEN-1; i>=0; i--){		// loop through all of the inventories
		if(openInvs[i] == NULL){					// if an inventory is NULL...
			for(j=i-1; j>=0; j--){					// loop through all of the inventories above it to find one to move down.
				if(openInvs[j] != NULL){			// if the next inventory is valid...
					openInvs[i] = openInvs[j];		// move the valid inventory to a position lower in the stack.
					openInvs[j] = NULL;
				}
			}
		}
	}
	// this finds out where the open inventory is
	for(i=MAX_INVENTORIES_OPEN-1; i>=0; i--){
		if(openInvs[i] == NULL)						// if you find an open inventory slot...
			return i;								// return that index
	}
	return -1;										// if there are no open inventory slots, return -1
}




#define minv_open 1
#define minv_close 0

// this will open/close inventories 
void manage_inventories(Uint8 action, struct inventoryData *inv){
	
	if(action == minv_open){
		int i;
		int invIndex = organize_open_inventories();
		
		if(invIndex >= 0){								// if there is an open slot...
			openInvs[invIndex] = inv;					// fill it up
		}
		else if(invIndex == -1){						// if there is NO open slot...
			for(i=MAX_INVENTORIES_OPEN-1; i>=1; i--)	// loop through the open invs
				openInvs[i] = openInvs[i-1];			// shift all of the inventories down in the stack
			openInvs[0] = inv;							// put the newest inventory on the top of the stack
		}
		#if(ERROR_MANAGE_INVENTORIES)//					// if an unexpected invIndex value is returned from the organize_open_inventories()
		else{											// print error info
			fprintf(errorFile,"\n\nmanage_inventories:\n\t bad invIndex return value from organize_open_inventories()\n\tinvIndex = %d",invIndex);
		}
		#endif
	}
	else if(action == minv_close){
		// WIP
	}
}


