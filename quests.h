
/// q == quest
//this is enumeration of the quests. I do this so that I can plug these values into
#define q_none				0
#define q_demigod_sun		1
#define q_demigod_rain		2
#define q_demigod_moon		3
#define q_demigod_snow		4


// these are for the questData structure
#define MAX_QUESTS			256

#define MAX_PREREQ_QUESTS	10
#define MAX_PREREQ_ITEMS	10

struct questData{
	
	// these are quests that need to be completed before the user can attempt this quest.
	int prereqQuest[MAX_PREREQ_QUESTS];
	
	// these are items that the player must have in his/her inventory to complete the quest.
	int prereqItem[MAX_PREREQ_ITEMS];
	
	
	
	// these are measured in seconds since the start of the start of the player's game.
	// both are default to 0.
	Uint32 start;	// when did the user start working on this quest?
	Uint32 finish;	// when did the user comlete the quest?
	
	// this tells us if the user is currently on this quest
	bool inProgress;
	
} quests[MAX_QUESTS]; /// this holds all of the main quests


void quest_erase(struct questData *datquest){
	int pq, pi;
	for(pq=0; pq<MAX_PREREQ_QUESTS; pq++){		// all quest prereqs are q_none by default
		datquest->prereqQuest[pq] = q_none;
	}
	for(pi=0; pi<MAX_PREREQ_QUESTS; pi++){		// all item  prereqs are i_none by default
		datquest->prereqItem[pi] = i_none;
	}
	datquest->start = 0;						// start is 0 by default.  this means the player has not finished the quest
	datquest->finish = 0;					// finish is 0 by default. this means the player has not finished the quest yet.
	datquest->inProgress = false;			// the quest is default to not in progress.
}


void init_quests(){
	
	int i;
	for(i=0; i<MAX_QUESTS; i++){
		quest_erase(&quests[i]); // erase quest number i
	}
	
	
}



