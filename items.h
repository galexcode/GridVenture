
// function prototype
void generate_item_sets();

/// i == item
// item numbers are integer types
#define i_none		0

#define i_sword_wood		3
#define i_sword_stone		4
#define i_sword_igneous		5

#define i_hammer_wood		7
#define i_hammer_stone		8
#define i_hammer_igneous	9

#define i_axe_wood			10
#define i_axe_stone			11
#define i_axe_igneous		12


/// it == itemType
//these are item types. short type data type
#define it_consumable		1
#define it_weapon			2
#define it_armor			3
#define it_special			4
#define it_tool				5


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
	items[i_sword_wood].name = "Wooden Sword";
	items[i_sword_wood].desc = "A simple sword made completely from wood. It isn't your best bet for protection.";
	items[i_sword_wood].attack = 2;
	items[i_sword_wood].cooldown = 500; // the sword can be used twice per second
	items[i_sword_wood].itemType = it_weapon;
	items[i_sword_wood].imagePos = 0x0001;
	items[i_sword_wood].stackAmount = 1;
	items[i_sword_wood].durability = dur_wood;
	
	// copy the basic wooden sword item attributes into the other sword items.
	item_copy(&items[i_sword_wood],&items[i_sword_stone]);
	item_copy(&items[i_sword_wood],&items[i_sword_igneous]);
	
	//slightly modify the better swords
	items[i_sword_stone].name = "Stone Sword";
	items[i_sword_stone].desc = "A sword made from a wooden hilt and a stone blade. It is sharper than a completely wooden blade. And quite a bit more durable too.";
	items[i_sword_stone].attack = 5;
	items[i_sword_stone].durability = dur_stone;
	items[i_sword_stone].imagePos = 0x0101;
	
	items[i_sword_igneous].name = "Igneous Sword";
	items[i_sword_igneous].desc = "A sword made from a igneous hilt and a wooden blade. The igneous adds a higher level of durability.";
	items[i_sword_igneous].attack = 10;
	items[i_sword_igneous].cooldown = 333;
	items[i_sword_igneous].durability = dur_igneous;
	items[i_sword_igneous].imagePos = 0x0201;
	
	//--------------------------------------------------------------------------
	// HAMMER STUFF
	//--------------------------------------------------------------------------
	items[i_hammer_wood].name = "Wooden Hammer";
	items[i_hammer_wood].attack = 2;
	items[i_hammer_wood].cooldown = 1000; // three attacks times per second
	items[i_hammer_wood].durability = dur_wood;
	items[i_hammer_wood].stackAmount = 1;
	items[i_hammer_wood].itemType = it_tool;
	items[i_hammer_wood].imagePos = 0x0002;
	
	items[i_hammer_stone].name = "Stone Hammer";
	items[i_hammer_stone].attack = 3;
	items[i_hammer_stone].cooldown = 750; // three attacks times per second
	items[i_hammer_stone].durability = dur_stone;
	items[i_hammer_stone].stackAmount = 1;
	items[i_hammer_stone].itemType = it_tool;
	items[i_hammer_stone].imagePos = 0x0102;
	
	items[i_hammer_igneous].name = "Igneous Hammer";
	items[i_hammer_igneous].attack = 4;
	items[i_hammer_igneous].cooldown = 500; // three attacks times per second
	items[i_hammer_igneous].durability = dur_igneous;
	items[i_hammer_igneous].stackAmount = 1;
	items[i_hammer_igneous].itemType = it_tool;
	items[i_hammer_igneous].imagePos = 0x0202;
	
	//--------------------------------------------------------------------------
	// HAMMER STUFF
	//--------------------------------------------------------------------------
	items[i_axe_wood].name = "Wooden Axe";
	items[i_axe_wood].attack = 3;
	items[i_axe_wood].cooldown = 1000; // three attacks times per second
	items[i_axe_wood].durability = dur_wood;
	items[i_axe_wood].stackAmount = 1;
	items[i_axe_wood].itemType = it_tool;
	items[i_axe_wood].imagePos = 0x0002;
	
	items[i_axe_stone].name = "Stone Axe";
	items[i_axe_stone].attack = 4;
	items[i_axe_stone].cooldown = 750; // three attacks times per second
	items[i_axe_stone].durability = dur_stone;
	items[i_axe_stone].stackAmount = 1;
	items[i_axe_stone].itemType = it_tool;
	items[i_axe_stone].imagePos = 0x0102;
	
	items[i_axe_igneous].name = "Igneous Axe";
	items[i_axe_igneous].attack = 6;
	items[i_axe_igneous].cooldown = 500; // three attacks times per second
	items[i_axe_igneous].durability = dur_igneous;
	items[i_axe_igneous].stackAmount = 1;
	items[i_axe_igneous].itemType = it_tool;
	items[i_axe_igneous].imagePos = 0x0202;
	
}


void init_different_item_set_sizes(){
	int s;
	int scalingFactor;
	
	// assume the [0] element has already been loaded with IMG_Load
	for(s=1; s<NUMBER_OF_GUI_SIZES; s++){
		scalingFactor = ITEM_SIZE[s]/ITEM_SIZE[0];
		item_set[s] = create_surface(item_set[0]->w*scalingFactor, item_set[0]->h*scalingFactor);
		scale_surface(item_set[0],item_set[s],scalingFactor);
	}
}





