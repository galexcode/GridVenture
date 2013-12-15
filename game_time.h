


/// this function updates the playingTime and pausedTime variables.
// this function returns the updated playingTime variable
Uint32 game_time(){
	// this keeps track of how much time the player has been playing his/her game. 
	// this ignores time spent in pause states
	// milliseconds
	static Uint32 playingTime = 0;
	// this is how much time the game has been paused since the SDL library was initialized
	// milliseconds
	static Uint32 pausedTime = 0;
	
	// this variable will be keeping track of the last time this function was called.
	static Uint32 timeLast = 0;
	// get current time
	Uint32 cTime = SDL_GetTicks();
	// calculate the difference in time between now and the last time this function was called.
	Uint32 timeElapsed = cTime - timeLast;
	
	
	if(paused){
		pausedTime += timeElapsed;
	}
	else{
		playingTime += timeElapsed;
	}
	
	// store the current time in the timeLast variable.
	// the next time this function is evaluted, we will know how much time has elapsed
	timeLast = cTime;
	return playingTime;
}



