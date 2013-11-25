
/// q == quest
//this is enumeration of the quests. I do this so that I can plug these values into
#define q_none				0
#define q_demigod_sun		1
#define q_demigod_moon		3

#define q_demigod_rain		2
#define q_demigod_snow		4



/// qs = quest state
// these are basically flags idicating what state the quest is currently in.
// these can be combined using bitwise ORing as they are powers of 2.
#define qs_unstarted		0
#define qs_started			1
#define qs_done				2

// these are for the questData structure
#define MAX_QUESTS			256

#define MAX_PREREQ_QUESTS	10
#define MAX_PREREQ_ITEMS	10


// this holds data concerning which quests you need to have completed in order to start a new one.
struct prereqQuestData{
	// this is the quest that is a prerequisite
	// stuff a value into it like
	int quest;
	
	// this is what state the quest has to be in for it to be counted as a satisfied prerequisite.
	// qs_unstarted requires the user has not completed or even started the quest.
	// qs_started 	requires the player is currently in the middle of the quest.
	// qs_done 		requires the player to have completed the quest.
	// you can bitwise OR these flags together to allow any circumstance.
	// for example, you might choose qs_started | qs_unstarted.
	// that would requre that the player is either currently playing through the quest to has not started it yet.
	// the flag qs_done would require that the played have completed it.
	int state;
	
	
};


struct questData{
	
	///----------------------------------------------
	/// prerequisites
	///----------------------------------------------
	// these are quests that need to be completed before the user can attempt this quest.
	struct prereqQuestData prereqQuest[MAX_PREREQ_QUESTS];
	
	// these are items that the player must have in his/her inventory to complete the quest.
	// default to m_none (no required item)
	int prereqItem[MAX_PREREQ_ITEMS];
	// this is how many of each prereqItem you will need to satisfy the quest prerequisites.
	Uint32 prereqItemQuan[MAX_PREREQ_ITEMS];
	
	///----------------------------------------------
	/// completion requirements
	///----------------------------------------------
	// these are items that the player must have in his/her inventory to complete the quest.
	// default to m_none (no required item)
	int compItem[MAX_PREREQ_ITEMS];
	// this is how many of each prereqItem you will need to satisfy the quest prerequisites.
	// default to 1.
	Uint32 compItemQuan[MAX_PREREQ_ITEMS];
	
	///----------------------------------------------
	/// metadata
	///----------------------------------------------
	// these are measured in seconds since the start of the start of the player's game.
	// both are default to 0.
	// these will be really nice for player statistics. I forsee a map of the player's quest history.
	Uint32 start;	// when did the user start working on this quest?
	Uint32 finish;	// when did the user comlete the quest?
	
	// this tells us if the user is currently on this quest
	bool progress;
	
	// this indicates if we should display a notification to the player when it has completed or started the quest
	// this is a true of false value.
	bool notify;
	
} quests[MAX_QUESTS]; /// this holds all of the main quests


void quest_erase(struct questData *datquest){
	int pq, pi;
	for(pq=0; pq<MAX_PREREQ_QUESTS; pq++){
		datquest->prereqQuest[pq].quest = q_none;	// default set the quest to q_none (requires no quest)
		datquest->prereqQuest[pq].state = qs_done;	// default set the quest state to qs_done
													// this allows the programmer to set prereqQuest.quest to a valid quest and automatically require it to be completed.
	}
	for(pi=0; pi<MAX_PREREQ_QUESTS; pi++){
		datquest->prereqItem[pi] = i_none;			// default the item required to i_none (doesn't require an item)
		datquest->prereqItemQuan[pi]=1;				// default required number of this item is 1.
													// this is so that if you change the prereqItem to a valid item (not i_none) then you will automatically require only one of those items.
	}
	datquest->start = 0;							// 0 seconds by default
	datquest->finish = 0;							// 0 seconds by default
	datquest->progress = qs_unstarted;				// the quest is unstarted by default.
	datquest->notify = true;						// notify the user about the quest by default.
}


void init_quests(){
	
	int i;
	for(i=0; i<MAX_QUESTS; i++){
		quest_erase(&quests[i]); // erase quest number i
	}
	
	
}



