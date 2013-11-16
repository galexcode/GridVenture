
// function prototype
void generate_item_sets();

/// i == item
// item numbers are integer types
#define i_none		0
#define i_axe		1
#define i_hammer	2
#define i_sword_wood_wood		3
#define i_sword_wood_stone		4
#define i_sword_igneous_stone	5
#define i_sword_igneous_igneous	6

#define i_axe_wood_stone		10
#define i_axe_wood_igneous		11
/// it == itemType
//these are item types. short type data type
#define it_consumable	1
#define it_weapon		2
#define it_armor		3
#define it_special		4

// this is how many unique items there are.
// not items the player is carrying.
// This tells you how many different items there are.
#define MAX_ITEMS 1024

// this is the default speed at which items can be used
#define ITEM_DEFAULT_COOLDOWN 1000;
#define DEFAULT_STACK_AMOUNT 256;
struct itemData{
	
	// this is what type of item the item is. for example, it could be it_consumable, it_weapon
	short itemType;
	
	// this is how much damage the weapon deals
	short attack;
	// this is how much defense the armor has.
	short defense;
	// this is the delay between uses of this item (in milliseconds)
	short cooldown;
	
	// this is how durable the item is. it tells you how many uses it can endure. positive integer
	unsigned short durability;
	
	// this is how many HP the item will heal you. negative or positive integer
	short heal;
	
	// this is how many items can be in a single stack in the player's inventory.
	// 0 is invalid and will be interpreted as 1.
	Uint32 stackAmount;
	
	char *name;	// the name of the item
	char *desc;	// the description of the item
	
	//this tells us where this item's image is on the item_parts_set sprite sheet.
	// this is a hexadecimal address for indexing.
	// 0x0000 corresponds to the "no item" image.
	// 0x070f means that it is in column 7 row 15.
	Uint16 imagePos;
	
}items[MAX_ITEMS];




#define dur_wood 0x40
#define dur_stone 0x100
#define dur_igneous 0x400
/// this fuction sets a certain item to default values 
void item_erase(struct itemData *datitem){
	datitem->name = "!Invalid item!";				// default invalid name
	datitem->desc = "This is not a valid item!";		// default description
	datitem->itemType = it_consumable; 				// consumable by default.
	datitem->attack = 0;							//
	datitem->defense = 0;							//
	datitem->cooldown = ITEM_DEFAULT_COOLDOWN;		//
	datitem->durability = 1;						//
	datitem->heal = 0;								//
	datitem->stackAmount = DEFAULT_STACK_AMOUNT;	//
	datitem->imagePos = 0x0000;						// default imagePos no item
}

/// copy all the attributes of one item over into another
void item_copy(struct itemData *sour, struct itemData *dest){
	dest->name =		sour->name;
	dest->desc =		sour->desc;
	dest->itemType =	sour->itemType;
	dest->attack =		sour->attack;
	dest->defense =		sour->defense;
	dest->cooldown =	sour->cooldown;
	dest->durability =	sour->durability;
	dest->heal =		sour->heal;
	dest->stackAmount =	sour->stackAmount;
	dest->imagePos =	sour->imagePos;
}



/// this initializes all of the item attributes
void init_items(){
	
	int i;
	for(i=0; i<MAX_ITEMS; i++)	// erase all items to default states.
		item_erase(&items[i]);
	//--------------------------------------------------------------------------
	// SWORD STUFF
	//--------------------------------------------------------------------------
	items[i_sword_wood_wood].name = "Wooden Sword";
	items[i_sword_wood_wood].desc = "A simple sword made completely from wood. It isn't your best bet for protection.";
	items[i_sword_wood_wood].attack = 4;
	items[i_sword_wood_wood].cooldown = 500; // the sword can be used twice per second
	items[i_sword_wood_wood].itemType = it_weapon;
	items[i_sword_wood_wood].imagePos = 0x0100;
	items[i_sword_wood_wood].stackAmount = 1;
	items[i_sword_wood_wood].durability = dur_wood;
	
	// copy the basic wooden sword item attributes into the other sword items.
	item_copy(&items[i_sword_wood_wood],&items[i_sword_wood_stone]);
	item_copy(&items[i_sword_wood_wood],&items[i_sword_igneous_stone]);
	item_copy(&items[i_sword_wood_wood],&items[i_sword_igneous_igneous]);
	
	//slightly modify the better swords
	items[i_sword_wood_stone].name = "Wooden-Stone Sword";
	items[i_sword_wood_stone].desc = "A sword made from a wooden hilt and a stone blade. It is sharper than a completely wooden blade. And quite a bit more durable too.";
	items[i_sword_wood_stone].attack = 6;
	items[i_sword_wood_stone].durability = (dur_stone+dur_wood)/2;
	items[i_sword_wood_stone].imagePos = 0x0200;
	
	items[i_sword_igneous_stone].name = "Igneous-Stone Sword";
	items[i_sword_igneous_stone].desc = "A sword made from a igneous hilt and a stone blade. The igneous adds a higher level of durability.";
	items[i_sword_igneous_stone].attack = 6;
	items[i_sword_igneous_stone].cooldown = 450;
	items[i_sword_igneous_stone].durability = (dur_stone+dur_igneous)/2;
	items[i_sword_igneous_stone].imagePos = 0x0300;
	
	items[i_sword_igneous_igneous].name = "Igneous Sword";
	items[i_sword_igneous_igneous].desc = "A sword made completely from igneous rock. The weapon is sharper than anything made of stone. The weapon can also be used quicker than anything made of stone.";
	items[i_sword_igneous_igneous].attack = 8;
	items[i_sword_igneous_igneous].cooldown = 333;
	items[i_sword_igneous_igneous].durability = dur_igneous;
	items[i_sword_igneous_igneous].imagePos = 0x0400;
	
	//--------------------------------------------------------------------------
	// HAMMER
	//--------------------------------------------------------------------------
	
	items[i_hammer].name = "Hammer";
	items[i_hammer].attack = 2;
	items[i_hammer].cooldown = 333; // three attacks times per second
	items[i_hammer].durability = 64;
	items[i_hammer].stackAmount = 1;
	items[i_hammer].itemType = it_weapon;
	items[i_hammer].imagePos = 0x0500;
	
	generate_item_sets();	//generate the item images on the item_set surface using item_parts_set 
}

///-----------------------------------------------------------------------------
/// ITEM_PART COMBINATION
///-----------------------------------------------------------------------------
// this section will combine item parts from the item_parts_set SDl_Surface and it will piecemeal them together to form the correct images for all items.

#define DEBUG_ITEM_SET_SURFACES 1

#define ORIGINAL_ITEM_SET_SIZE	32	// pixels
#define ITEM_SET_WIDTH 	32			// number of items wide the item_set is (maximum)
#define ITEM_SET_HEIGHT 32			// number of items high the item_set is (maximum)
#define ITEM_SET_WIDTH_PIXELS	(ORIGINAL_ITEM_SET_SIZE*ITEM_SET_WIDTH)		// this is t
#define ITEM_SET_HEIGHT_PIXELS	(ORIGINAL_ITEM_SET_SIZE*ITEM_SET_HEIGHT)

/// these are definitions of where items parts are located on the original item_parts_set that gets loaded in
#define ip_none					0x0000

#define ip_axe_head_igneous 	0x0100
#define ip_axe_head_stone		0x0200
#define ip_axe_handle_wood		0x0300

#define ip_sword_blade_igneous	0x0400
#define ip_sword_blade_wood		0x0401
#define ip_sword_blade_stone	0x0500
#define ip_sword_hilt_igneous	0x0600
#define ip_sword_hilt_wood		0x0700

/// proprietary use in only the below function
#define apply_item_part(sour, dest) itemClip.x = (sour/0x100)*ORIGINAL_ITEM_SET_SIZE; itemClip.y = (sour%0x100)*ORIGINAL_ITEM_SET_SIZE; apply_surface_clips((dest/0x100)*ORIGINAL_ITEM_SET_SIZE,(dest%0x100)*ORIGINAL_ITEM_SET_SIZE, item_parts_set, item_set, &itemClip);

void generate_item_sets(){
	
	SDL_Rect itemClip;
	itemClip.w = itemClip.h = ORIGINAL_ITEM_SET_SIZE; // set the width and height permanently
	
	item_set = create_surface(1024, 1024);
	// set correct alpha properties
	SDL_SetAlpha(item_set, 0, 0xff);
	// set correct alpha properties
	SDL_SetAlpha(item_parts_set, SDL_SRCALPHA, 0xff);
	
	
	if(item_set==NULL){
		handle_error(e_surface_creation, "item_set surface could not be fabricated. Quitting Game.");		// if there is an error, report it.
		quit_game(quit_surface_error,NULL);
	}
	
	
	
	
	
	
	//generate wooden sword
	apply_item_part(ip_sword_hilt_wood,  items[i_sword_wood_wood].imagePos);
	apply_item_part(ip_sword_blade_wood, items[i_sword_wood_wood].imagePos);
	
	//generate wooden-stone sword
	apply_item_part(ip_sword_hilt_wood,   items[i_sword_wood_stone].imagePos);
	apply_item_part(ip_sword_blade_stone, items[i_sword_wood_stone].imagePos);
	
	//generate igneous-stone sword
	apply_item_part(ip_sword_hilt_igneous, items[i_sword_igneous_stone].imagePos);
	apply_item_part(ip_sword_blade_stone,  items[i_sword_igneous_stone].imagePos);
	
	//generate igneous-igneous sword
	apply_item_part(ip_sword_hilt_igneous,  items[i_sword_igneous_igneous].imagePos);
	apply_item_part(ip_sword_blade_igneous, items[i_sword_igneous_igneous].imagePos);
	
	#if(DEBUG_ITEM_SET_SURFACES)
	int i,j;
	for(j=0; j<32; j++){
		for(i=32; i<128; i++){
			printf("%8x  ",get_pixel(item_parts_set,i,j));
		}
		printf("\n");
	}
	printf("\n\n\n\n\n\n");
	
	for(j=0; j<32; j++){
		for(i=32; i<128; i++){
			printf("%8x  ",get_pixel(item_set,i,j));
		}
		printf("\n");
	}
	#endif
	
	
	SDL_Rect screenRect;
	screenRect.x = screenRect.y = 0;
	screenRect.h = SCREEN_HEIGHT;
	screenRect.w = SCREEN_WIDTH;
	SDL_FillRect( screen , &screenRect , 0xffffffff);
	
	itemClip.x = 32; itemClip.w = 128;
	itemClip.y =  0; itemClip.h =  32;
	apply_surface_clips(32,0,item_set,screen, &itemClip);	// apply the item_set surface to the screen for debugging
	SDL_Flip(screen);
	SDL_Delay(2000);
}













