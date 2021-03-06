//--------------------------------------------------------------------------------------------------------------------------------------------------------------

			//if a key was pressed
			if( event.type == SDL_KEYDOWN ){
				//nothing
			}
			//if a key was released
			if( event.type == SDL_KEYUP ){
				//nothing
			}
			
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

		//keeps track of how long you have been pressing the button.
		if(keyup == 1) countup++;
		if(keydown == 1) countdown++;
		if(keyleft == 1) countleft++;
		if(keyright == 1) countright++;

		if(countup >= MAX_COUNT) { y-=20; countup = 0; }
		if(countdown >= MAX_COUNT) { y+=20; countdown = 0; }
		if(countleft >= MAX_COUNT) { x-=20; countleft = 0; }
		if(countright >= MAX_COUNT) { x+=20; countright = 0; }

		//apply white background
		SDL_Rect my_rect;
		my_rect.x = 0;
		my_rect.y = 0;
		my_rect.w = 640;
		my_rect.h = 480;
		SDL_FillRect( screen, &my_rect, 0xffffff );
		SDL_FreeSurface( screen );

		//apply images
		apply_surface(x, y, image, screen ); //apply image
		apply_surface(x + 20, y, image, screen ); //apply image
		apply_surface(x - 20, y, image, screen ); //apply image
		apply_surface(x, y + 20, image, screen ); //apply image
		apply_surface(x, y - 20, image, screen ); //apply image
		// flip screen
		SDL_Flip( screen ); // update screen
		//SDL_FreeSurface( screen ); // saves memory
		
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

	/// OLD original GRAVITY
	
	/// 1. GRAVITY - this is where the gravity gets CHECKED and APPLIED.
	for(j=GRID_HEIGHT-1 ; j>=0 ; j--){
		for(i=0 ; i<GRID_WIDTH ; i++){

			// if gravity affects this material...
			if(mats[grid[i][j].mat].gravity){
				// dissapears through the bottom
				if(j >= GRID_HEIGHT-1){ // if the gravity material is at the bottom of the screen, get rid of it.
					grid[i][j].mat = m_air;
				}
				// falls down
				else if(grid[i][j+1].mat == m_air || grid[i][j+1].mat == m_air){
					grid[i][j+1].mat = grid[i][j].mat;
					grid[i][j].mat = m_air;
				}
				// if the material could EITHER fall to the LEFT or the RIGHT
				else if( i>0 && grid[i-1][j].mat == m_air && grid[i-1][j+1].mat == m_air   &&   i<GRID_WIDTH-1 && grid[i+1][j].mat == m_air && grid[i+1][j+1].mat == m_air ){
					// randomly choose whether to...
					if(get_rand(0,1)){
						grid[i-1][j+1].mat = grid[i][j].mat; // go to the left or...
						grid[i][j].mat = m_air;
					}
					else{
						grid[i+1][j+1].mat = grid[i][j].mat; // go to the right.
						grid[i][j].mat = m_air;
					}
				}
				// if the material can ONLY fall to the LEFT
				else if( i>0 && grid[i-1][j].mat == m_air && grid[i-1][j+1].mat == m_air){
					grid[i-1][j+1].mat = grid[i][j].mat; // go to the left or...
					grid[i][j].mat = m_air;
				}
				//if the material can ONLY fall the the RIGHT
				else if( i <GRID_WIDTH-1 && grid[i+1][j].mat == m_air && grid[i+1][j+1].mat == m_air){
					grid[i+1][j+1].mat = grid[i][j].mat; // go to the right.
					grid[i][j].mat = m_air;
				}
			} // if gravity
		} // for columns
	} // for rows
	

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

/// FIRST ATTEMPT AT TUNNELING MATERIALS

 OLD GRADED GRAVITY
					// the minimum slope that the material can fall down is a 1/1 or less steep
					else{
						for(ig=1; (-ig>=currentGrav); ig++){
							// if the material can fall to the right and has not yet been shown to be an invalid path
							if( (i+ig<GRID_WIDTH) && (grid[i+ig+camera_x][j+camera_y].mat==m_air)  &&  !(invalidSlope&SLOPE_RIGHT) ){
								if(grid[i+ig+camera_x][j+1+camera_y].mat == m_air)
									validSlope |= SLOPE_RIGHT;
							}else invalidSlope |= SLOPE_RIGHT;
							
							// if the material can fall to the left and has not yet been shown to be an invalid path
							if( !(holdOffLeft) && (i-ig>=0) && (grid[i-ig+camera_x][j+camera_y].mat==m_air)  &&  !(invalidSlope&SLOPE_LEFT) ){
								if(grid[i-ig+camera_x][j+1+camera_y].mat == m_air)
									validSlope |= SLOPE_LEFT;
							}else invalidSlope |= SLOPE_LEFT;
							
							if(invalidSlope == SLOPE_EITHER){		//if the slope is invalid in both directions, quit trying to evaluate the slope.
								break;
							}
							if(validSlope == SLOPE_RIGHT){				//if the material has a valid slope on the right
								grid[i+camera_x][j+camera_y].mat = m_air; 				// remove the material from its current location
								grid[i+ig+camera_x][j+1+camera_y].mat = currentMat; 		// place the material in its new location to the right
								grid[i+ig+camera_x][j+1+camera_y].sat = currentSat;	//copy saturation into new place
								grid[i+camera_x][j+camera_y].sat = m_no_saturation;		//remove old saturation
								break;
							}
							else if(validSlope == SLOPE_LEFT){		//if the material has a valid slope on the left
								grid[i+camera_x][j+camera_y].mat = m_air; 				// remove the material from its current location
								grid[i-ig+camera_x][j+1+camera_y].mat = currentMat; 		// place the material in its new location to the left
								grid[i-ig+camera_x][j+1+camera_y].sat = currentSat;	//copy saturation into new place
								grid[i+camera_x][j+camera_y].sat = m_no_saturation;		//remove old saturation
								holdOffLeft=currentGrav;
								break;
							}
							else if(validSlope == SLOPE_EITHER){	//if the material has a valid slope on either side
								if(get_rand(0,1)){					// goes to the right
									grid[i+camera_x][j+camera_y].mat = m_air;
									grid[i+ig+camera_x][j+1+camera_y].mat = currentMat;
									grid[i+ig+camera_x][j+1+camera_y].sat = currentSat;	//copy saturation into new place
									grid[i+camera_x][j+camera_y].sat = m_no_saturation;		//remove old saturation
									break;
								}
								else{								// goes to the left
									grid[i+camera_x][j+camera_y].mat = m_air;
									grid[i-ig+camera_x][j+1+camera_y].mat = currentMat;
									grid[i-ig+camera_x][j+1+camera_y].sat = currentSat;	//copy saturation into new place
									grid[i+camera_x][j+camera_y].sat = m_no_saturation;		//remove old saturation
									holdOffLeft=currentGrav;
									break;
								}
							}
						}
					}
				else{ // if all else fails, try to TUNNEL
					// if the material has not declined, and if the material one cell down is the same as the one being evaluated...
					if(currentGrav < 0 && grid[i+camera_x][j+1+camera_y].mat == currentMat){
						
						//these valuse will be overwritten as the cell is evaluated.
						validTunnel = TUNNEL_NONE;
						invalidTunnel = TUNNEL_NONE;
						foundObstacleRight = false;
						foundObstacleLeft  = false;
						
						for(ig=1; ig<=-currentGrav; ig++){
							
							// check to see if there valid tunnel RIGHT
							if( (invalidTunnel&TUNNEL_RIGHT) == 0 && grid[i+camera_x+ig][j+camera_y+1].mat == m_air) validTunnel |= TUNNEL_RIGHT;
							// if there is no where to tunnel yet, check to see if there is an invalid material blocking the way.
							else if( (validTunnel&TUNNEL_RIGHT) == 0 && grid[i+camera_x+ig][j+camera_y+1].mat != currentMat) invalidTunnel |= TUNNEL_RIGHT;
							//check to see if there is an obstacle to the right
							
							//check to see if there is any valid obstruction to the right
							if(grid[i+camera_x+ig][j+camera_y].mat != m_air) foundObstacleRight = true;
							
							// check to see if there valid tunnel LEFT
							if( (invalidTunnel&TUNNEL_LEFT) == 0 && grid[i+camera_x-ig][j+camera_y+1].mat == m_air) validTunnel |= TUNNEL_LEFT;
							// if there is no where to tunnel yet, check to see if there is an invalid material blocking the way.
							else if( (validTunnel&TUNNEL_LEFT) == 0 && grid[i+camera_x-ig][j+camera_y+1].mat != currentMat) invalidTunnel |= TUNNEL_LEFT;
							
							//check to see if there is any valid obstruction to the left
							if(grid[i+camera_x-ig][j+camera_y].mat != m_air) foundObstacleLeft = true;
							
							
							// if there is no valid tunnel, quit
							if( invalidTunnel == TUNNEL_EITHER) break;
							
							// if the material can tunnel left or right.
							if(validTunnel == TUNNEL_EITHER && foundObstacleLeft && foundObstacleRight){
								//decide to go left or right.
								if(get_rand(0,1)) ig *= -1;
								grid[i+camera_x+ig][j+camera_y+1].mat = currentMat;
								grid[i+camera_x+ig][j+camera_y+1].sat = currentSat;
								// erase old cell data
								grid[i+camera_x][j+camera_y].mat = m_air;
								grid[i+camera_x][j+camera_y].sat = m_no_saturation;
								break;
								
								break;
							}
							else if(validTunnel == TUNNEL_RIGHT && foundObstacleRight){
								// overwrite new cell data
								grid[i+camera_x+ig][j+camera_y+1].mat = currentMat;
								grid[i+camera_x+ig][j+camera_y+1].sat = currentSat;
								// erase old cell data
								grid[i+camera_x][j+camera_y].mat = m_air;
								grid[i+camera_x][j+camera_y].sat = m_no_saturation;
								break;
							}
							else if(validTunnel == TUNNEL_LEFT && foundObstacleLeft){
								// overwrite new cell data
								grid[i+camera_x-ig][j+camera_y+1].mat = currentMat;
								grid[i+camera_x-ig][j+camera_y+1].sat = currentSat;
								// erase old cell data
								grid[i+camera_x][j+camera_y].mat = m_air;
								grid[i+camera_x][j+camera_y].sat = m_no_saturation;
								break;
							}
							
						} // for (check for tunneling)
					} // if tunneling if possible
				} // if all else fails, try to tunnel
				

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

/// OLD PANNING
void pan(int direction){
	switch(direction){
	case D_UP:
		camera_y -= MAX_CELL_SIZE/CELL_SIZE + 6;
		break;
	case D_LEFT:
		camera_x -= MAX_CELL_SIZE/CELL_SIZE + 6;
		break;
	case D_DOWN:
		camera_y += MAX_CELL_SIZE/CELL_SIZE + 6;
		break;
	case D_RIGHT:
		camera_x += MAX_CELL_SIZE/CELL_SIZE + 6;
		break;
	}
	verify_camera();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

///UNNECESSARY STUFF THAT I DON'T NEED


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

#define ip_hatchet_head_igneous 	0x0100
#define ip_hatchet_head_stone		0x0200
#define ip_hatchet_handle_wood		0x0300

#define ip_sword_blade_igneous	0x0400
#define ip_sword_blade_wood		0x0401
#define ip_sword_blade_stone	0x0500
#define ip_sword_hilt_igneous	0x0600
#define ip_sword_hilt_wood		0x0700

/// proprietary use in only the below function
//#define apply_item_part(sour, dest) itemClip.x = (sour/0x100)*ORIGINAL_ITEM_SET_SIZE; itemClip.y = (sour%0x100)*ORIGINAL_ITEM_SET_SIZE; apply_surface_clips((dest/0x100)*ORIGINAL_ITEM_SET_SIZE,(dest%0x100)*ORIGINAL_ITEM_SET_SIZE, item_parts_set, item_set, &itemClip);

// 
void apply_item_part(Uint16 sour, Uint16 dest){
	
}

void generate_item_sets(){
	
	SDL_Rect itemClip;
	itemClip.w = itemClip.h = ORIGINAL_ITEM_SET_SIZE; // set the width and height permanently
	
	item_set = create_surface(1024, 1024);
	// set correct alpha properties
	SDL_SetAlpha(item_set, 0, 0xff);
	// set correct alpha properties
	SDL_SetAlpha(item_parts_set, SDL_SRCALPHA, 0x0);
	
	
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
			fprintf(debugFile,"%8x  ",get_pixel(item_parts_set,i,j));
		}
		fprintf(debugFile,"\n");
	}
	fprintf(debugFile,"\n\n\n\n\n\n");
	
	for(j=0; j<32; j++){
		for(i=32; i<128+32; i++){
			fprintf(debugFile,"%8x  ",get_pixel(item_set,i,j));
		}
		fprintf(debugFile,"\n");
	}
	#endif
	
	
	SDL_Rect screenRect;
	screenRect.x = screenRect.y = 0;
	screenRect.h = SCREEN_HEIGHT;
	screenRect.w = SCREEN_WIDTH;
	//SDL_FillRect( screen , &screenRect , 0xffffffff);
	
	itemClip.x = 32; itemClip.w = 128;
	itemClip.y =  0; itemClip.h =  32;
	apply_surface_clips(32,0,item_parts_set,screen, &screenRect);	// apply the item_set surface to the screen for debugging
	SDL_Flip(screen);
	SDL_Delay(2000);
}







