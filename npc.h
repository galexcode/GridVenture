/// npc = non-player character
/// int data type

#define npc_none	0		// this is a flag. it tells us that this is an invald npc.

#define npc_god_sun	 1		// the god of sun
#define npc_god_rain 2		// the god of rain
#define npc_god_moon 3		// the god of moon
#define npc_god_hail 4		// the god of hail

// this is the max number of distinct non-player characters there can be in this game.
// for now.
#define MAX_NPCS 512

struct npcData{
	// this is the name of the npc
	char *name;
	
	// this is a short biography of the npc.
	char *bio;
	// this tells you if you have unlocked the character's bio
	bool bioUnlocked;
	
	// this variable will point to a string. that is what the character says.
	// the pointer is usually updated when you complete a quest.
		// e.g.
		// if you have not completed any quests, he says, "Hello. I'm Tim. I run the local pub."
		// if you have completed a dragon slaying quest, he might say, "Great work! I could have never done what you did!"
	char *speech;
	
};
