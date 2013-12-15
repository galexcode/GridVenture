





#define MILLISECONDS_PER_CELL 20
/// evaluate the physics of the player's motion.
void evaluate_player_movement(struct playerData *datplayer, int keyup, int keyleft, int keydown, int keyright){
	
	//----------------------------------------------------
	// time stuff
	//----------------------------------------------------
	static int timeSinceMotion = 0;				// the number of milliseconds since the player last moved
	static int previousTime = 0;				// this is the time when this function was evaluated last
	int currentTime = game_time();				// the current cycle's ticks (milliseconds since SDL library initialized)
	int millis = currentTime - previousTime;	// the difference in ticks (the time that has passed since the last cycle in milliseconds)
	if(millis <= 0) return; 					// don't check player movement when time stops or runs backwards.
	previousTime = currentTime;					// store current ticks in the previous ticks variable. it will be used next time.
	
	timeSinceMotion += millis;					// add to the cumulative number of milliseconds that have passed since last movement
	
	//----------------------------------------------------
	// standing on collision material checking
	//----------------------------------------------------
	// check to see if the player is standing on a collision type material
	if(mats[grid[datplayer->x_pos][datplayer->y_pos].mat].collision){
		datplayer->onCollMat = true;			// set the player's "standing on collision material" flag to true
		datplayer->y_accel = 0;					// set acceleration to 0.
		datplayer->y_vel   = 0;					// set velocity to 0.
	}
	else{
		datplayer->onCollMat = false;			// set the player's "standing on collision material" flag to false
		datplayer->y_accel = GRAVITY_ACCEL;		// set the player's y acceleration to acceleration due to gravity
	}
	
	//----------------------------------------------------
	// checking for user input
	//----------------------------------------------------
	if(keyup && datplayer->onCollMat){
		datplayer->y_accel = DEFAULT_PLAYER_JUMP_VELOCITY;
	}
	if( (keyright && keyleft) || (!keyright && !keyleft) ){
		datplayer->x_vel = 0;
	}
	else if(keyright){
		datplayer->x_vel = DEFAULT_PLAYER_WALK_VEL;
	}
	else if(keyleft){
		datplayer->x_vel = -DEFAULT_PLAYER_WALK_VEL;
	}
	//----------------------------------------------------
	// numerical position and velocity calculations
	//----------------------------------------------------
	// add acceleration*time to velocity
	datplayer->x_vel += datplayer->x_accel*millis;
	datplayer->y_vel += datplayer->y_accel*millis;
	// add speed*time to position
	datplayer->x_pos_f += datplayer->x_vel*millis;
	datplayer->y_pos_f += datplayer->y_vel*millis;
	
	//----------------------------------------------------
	// moving the player
	//----------------------------------------------------
	bool moved;
	do{
		moved = false;
		if(datplayer->x_pos_f > 0.5f){
			if( player_valid_position(datplayer, datplayer->x_pos+1, datplayer->y_pos)){
				datplayer->x_pos += 1;
				datplayer->x_pos_f -= 1.0f;
				moved=true;
			}
			else datplayer->x_pos_f = 0;
		}
		else if(datplayer->x_pos_f < -0.5f){
			if( player_valid_position(datplayer, datplayer->x_pos-1, datplayer->y_pos)){
				datplayer->x_pos -= 1;
				datplayer->x_pos_f += 1.0f;
				moved=true;
			}
			else datplayer->x_pos_f = 0;
		}
		if(datplayer->y_pos_f > 0.5f){
			if( player_valid_position(datplayer, datplayer->x_pos, datplayer->y_pos+1)){
				datplayer->y_pos += 1;
				datplayer->y_pos_f -= 1.0f;
				moved=true;
			}
			else datplayer->y_pos_f = 0;
		}
		else if(datplayer->y_pos_f < -0.5f){
			if( player_valid_position(datplayer, datplayer->x_pos, datplayer->y_pos-1)){
				datplayer->y_pos -= 1;
				datplayer->y_pos_f += 1.0f;
				moved=true;
			}
			else datplayer->y_pos_f = 0;
		}
	}while(moved==true);
	
	/*
	///---------------------------------------------------
	/// this section of the code is only temporary.
	/// The movement of the character is still under development.
	/// this was put here so that I don't have to focus on development of the player's motion right now.
	///---------------------------------------------------
	// these are used to see where the user is trying to get to.
	int moveHoriz=0, moveVert=0;
	//check to see if it is time to move
	while(timeSinceMotion >= MILLISECONDS_PER_CELL){
		// decrement timeSinceMotion because we are about to move.
		timeSinceMotion -= MILLISECONDS_PER_CELL;
		if(keyright){
			moveHoriz += 1;	// move right
		}
		if(keyleft){
			moveHoriz -= 1;	// move left
		}
		if(keyup){
			moveVert  -= 1;	// move up
		}
		if(keydown){
			moveVert  += 1;	// move down
		}
		//try to move (only if the destination is valid)
		if( player_valid_position(datplayer, datplayer->x_pos+moveHoriz, datplayer->y_pos+moveVert) ){
			datplayer->x_pos += moveHoriz;	// apply motion
			datplayer->y_pos += moveVert;	// apply motion
		}
	}
	*/
	
	//----------------------------------------------------
	// position boundaries checking and enforcement
	//----------------------------------------------------
	//check to see if the outside the bounds of the game screen.
	if(datplayer->x_pos < 0)
		datplayer->x_pos = 0;						// lower bound enforcement on x
	if(datplayer->x_pos >= GRID_WIDTH_ELEMENTS)
		datplayer->x_pos = GRID_WIDTH_ELEMENTS-1;	// upper bound enforcement on x
	if(datplayer->y_pos < 0)
		datplayer->y_pos = 0;						// lower bound enforcement on y
	if(datplayer->y_pos >= GRID_HEIGHT_ELEMENTS)
		datplayer->y_pos = GRID_HEIGHT_ELEMENTS-1;	// upper bound enforcement on y
}





