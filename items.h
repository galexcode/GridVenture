/// i == item
// item numbers are integer types
#define i_none		0
#define i_axe		1
#define i_hammer	2
#define i_sword		3
#define i_apple		
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
	
}items[MAX_ITEMS];





/// this fuction sets a certain item to default values 
void item_erase(struct itemData *datitem){
	datitem->itemType = it_consumable; 	// consumable by default.
	datitem->attack = 0;
	datitem->defense = 0;
	datitem->cooldown = ITEM_DEFAULT_COOLDOWN;
	datitem->durability = 1;
	datitem->heal = 0;
	datitem->stackAmount = DEFAULT_STACK_AMOUNT;
}


/// this initializes all of the item attributes
void init_items(){
	
	
	
}
