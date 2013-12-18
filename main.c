#include "headers_and_globals.h"




int main( int argc, char* args[] ) 
{
	//get a random seed.
	srand(time(NULL));
	CELL_SIZE = 16; // set starting cell size	
	
	//----------------------------------------------------
	// VARIABLES USED IN MAIN()
	//----------------------------------------------------
    int x, y;									// this is the location of the player's mouse
    
    int mouseLeft[2] = {0,0};					// this keeps track of the user's left  mouse button state. [0]=current, [1]=previous
    int mouseRight[2] = {0,0};					// this keeps track of the user's right mouse button state. [0]=current, [1]=previous
	int keyw=0,keya=0,keys=0,keyd=0,keyspace=0;	// these keep track of the user's WASD keys
	bool keyF3=true;							// this keeps track of the state of the F3 key
	bool inventoryView=0;						// this keeps track of the inventory key's state (E)
	unsigned char ctrl=0;						// this keeps track of if either of the ctrl keys are pressed.
	
	int ticksSinceLastFPSUpdate=0;				// time since last FPS update (ideally, goes from 0 to 1000 milliseconds and then resets)
	int cumulativeFrames = 0;					// this counts how many Frames there have been since the last
	int currentTicks = 0;						// this is how many 
	int quit = false;							//make sure the program waits for a quit
	
	//----------------------------------------------------
	// initialize lots of stuff
	//----------------------------------------------------
    if( init() == false ) return 1;				// make sure you can boot up the necessary libraries
    if( load_files() == false ) return 2;		// make sure all files are loaded correctly
    if( SDL_Flip( screen ) == -1 ) return 3;	// make sure the screen works
    init_cell_stuff();							// initialize the cells
    init_items();								// initialize items
    init_different_item_set_sizes();			// initialize different sizes of item_set surfaces
    init_quests();								// initialize the quests
    
    //----------------------------------------------------
	// surfaces and rects
	//----------------------------------------------------
    SDL_Rect screenRect;														// this rect is used for displaying the grid and the sky
    screenRect.x = 0;
	screenRect.y = 0;
	screenRect.w = SCREEN_WIDTH;
	screenRect.h = SCREEN_HEIGHT;
    gridSurface = create_surface(GRID_WIDTH_ELEMENTS,GRID_HEIGHT_ELEMENTS);		// create the surface that the grid data will be printed to.
    skySurface = create_surface(GRID_WIDTH_ELEMENTS,GRID_HEIGHT_ELEMENTS);		//create the skySurface
    generate_sky(skySurface, SCREEN_WIDTH, SCREEN_HEIGHT);						//generate sky gradient
    
    //----------------------------------------------------
	// generate world; initialize player; blast off
	//----------------------------------------------------
	gen_world(w_normal,0);						// generate a world on startup
	int i;
	#if(DEBUG)
		for(i=0; i<GRID_WIDTH_ELEMENTS; i++) grid[i][500].mat = m_plant;	// generate stone line for testing
	#endif
	init_player_attributes(&player);			// get default player data.
	player.x_pos = GRID_WIDTH_ELEMENTS/2;		// set player location x
	player.y_pos = 500;					// set player location y
	verify_grid_and_cell_size();				// final grid and cell verification
	#if(1)//									// this is just for testing the user's inventory
		player.inv.slot[24].item = i_hatchet_igneous;
		player.inv.slot[25].item = i_sword_igneous;
		player.inv.slot[26].item = i_torch;
		player.inv.slot[16].item = i_hatchet_igneous;
		player.inv.slot[8].item = i_hatchet_stone;
		player.inv.slot[17].item = i_sword_wood;
		player.inv.slot[18].item = i_hammer_igneous;
	#endif
	#if(1)//									// testing inventory adding
		struct inventoryData testingInv1;
		inventory_erase(&testingInv1);
		manage_inventories(inv_open,&testingInv1);
		struct inventoryData testingInv2;
		inventory_erase(&testingInv2);
		manage_inventories(inv_open,&testingInv2);
	#endif
	
	//----------------------------------------------------
	// this simply tests the paragra_to_lines() function
	//----------------------------------------------------
	#if(0)
		char *testString = "Hello World. ABC This is Ryan Jensen. I'm writing on behalf of the GridVenture Liberation organization.";
		const int width = 16;
		const int height = 8;
		char testLines[height][width+1];
		int rw, rh; // resulting widths, resulting height
		paragraph_to_lines(testString,width,height,&testLines[0][0],&rw,&rh);
		int j,i;
		for(j=0; j<height; j++){
			for(i=0; i<width+1; i++){
				fprintf(debugFile,"%c",testLines[j][i]);
			}
			fprintf(debugFile,"\n");
		}
		fprintf(debugFile,"\nresultingWidth = %d\nresultingHeight = %d\n",rw,rh);
	#endif
    //----------------------------------------------------
	// enter the main while loop of the game.
	//----------------------------------------------------
    while(1){
		
    	//While there's an event to handle
    	while( SDL_PollEvent( &event ) ){
			
    		//If the user has Xed out the window
    		if( event.type == SDL_QUIT || quit == true ){
				//Quit the program
				quit_game(0,NULL);
			}
			
            if( event.type == SDL_MOUSEBUTTONDOWN ){						/// mouse down
				x = event.motion.x;
				y = event.motion.y;
                if( event.button.button == SDL_BUTTON_LEFT ){
                    mouseLeft[0] = 1;
                }
                else if( event.button.button == SDL_BUTTON_RIGHT ){
                    mouseRight[0] = 1;
                }
                
                else if( event.button.button == SDL_BUTTON_WHEELUP ){
					if(ctrl){
						zoom_in(x,y);
					}
					else{
						player.hotbarIndex--;
						if(player.hotbarIndex < 0) player.hotbarIndex = player.inv.width-1;
					}
                }
				else if( event.button.button == SDL_BUTTON_WHEELDOWN ){
					if(ctrl){
						zoom_out(x,y);
					}
					else{
						player.hotbarIndex++;
						if(player.hotbarIndex >= player.inv.width) player.hotbarIndex = 0;
					}
				}
				
            }
            else if(event.type == SDL_MOUSEBUTTONUP){						/// mouse up
				x = event.motion.x;
				y = event.motion.y;
                if( event.button.button == SDL_BUTTON_LEFT ){
                    mouseLeft[0] = 0;
                }
                else if( event.button.button == SDL_BUTTON_RIGHT ){
                    mouseRight[0] = 0;
                }
            }
            else if( event.type == SDL_MOUSEMOTION ){						/// mouse motion
				x = event.motion.x;
				y = event.motion.y;
            }
            else if(event.type == SDL_VIDEORESIZE){							/// window resize
				
//				float new_cell_size = CELL_SIZE * event.resize.h/((float)SCREEN_HEIGHT); // adjust the pixel size.
//				if(new_cell_size - ((int)new_cell_size) >= 0.5f) CELL_SIZE = new_cell_size + 1;
//				else CELL_SIZE = new_cell_size;
				SCREEN_WIDTH = event.resize.w;
				SCREEN_HEIGHT = event.resize.h;
				verify_grid_and_cell_size(); // make sure the window isn't too big for the cell size
				set_window_size(event.resize.w, event.resize.h);		// set window to correct dimensions
				generate_sky(skySurface, SCREEN_WIDTH, SCREEN_HEIGHT);	// render a new sky background
				screenRect.w = SCREEN_WIDTH;							// update the screen rectangle
				screenRect.h = SCREEN_HEIGHT;
			}
			
            if( event.type == SDL_KEYDOWN ){		///keyboard event
                switch( event.key.keysym.sym ){
				case SDLK_c:		reset_cells();  break;	//clear the screen
				case SDLK_ESCAPE:	if(paused == 0) {paused = 1;} else if(paused == 1) {paused = 0;} break; 	//pause the game
				//case SDLK_ESCAPE: quit = true; 		// quit with escape
				case SDLK_F1:		gen_world(w_normal,wf_display_generation); break; // generate a world
				case SDLK_F3:		keyF3 ^= 1; break;	// toggle printing debugging information
				case SDLK_w:		keyw=1; break; 		// store key state
				case SDLK_a:		keya=1; break;
				case SDLK_s:		keys=1; break;
				case SDLK_d:		keyd=1; break;
				case SDLK_SPACE:	keyspace=1; break;
				case SDLK_1:
				case SDLK_2:
				case SDLK_3:
				case SDLK_4:
				case SDLK_5:
				case SDLK_6:
				case SDLK_7:
				case SDLK_8:		player.hotbarIndex = event.key.keysym.sym - '1'; break; // set the user's hotbar location
				case SDLK_e:		inventoryView^=1; manage_inventories(inventoryView, &player.inv); break;	// toggle inventory view
				case SDLK_RCTRL: case SDLK_LCTRL:	ctrl++; break;	// ctrl was pressed
				
				default: break;
				}
			}
			if( event.type == SDL_KEYUP ){								///keyboard event
                switch( event.key.keysym.sym ){
				case SDLK_w: 		keyw=0; break; // store key state
				case SDLK_a: 		keya=0; break;
				case SDLK_s: 		keys=0; break;
				case SDLK_d: 		keyd=0; break;
				case SDLK_SPACE:	keyspace=0; break;
				case SDLK_RCTRL: case SDLK_LCTRL: ctrl--; break; // ctrl was released
				default: break;
				}
			}
                
		
    	} // end while(event)
		//no more events to handle at the moment.
		
		
		//evaluate the player's movements.
		evaluate_player_movement(&player, keyspace, keya, keys, keyd);
		// keep the compiler quiet
		if(keyw);
		
		//evaluate the player's inventory interaction
		evaluate_inventories(x,y,mouseLeft[0],mouseRight[0]);
		
		//apply/remove material (test feature for debugging)
		#if(1)
		int xcell,ycell;
		xcell = x/CELL_SIZE+(int)player.x_pos-GRID_WIDTH/2;
		ycell = y/CELL_SIZE+(int)player.y_pos-GRID_HEIGHT/2;
		if(within_grid_bounds(xcell,ycell)){
			//checks if the mouse is held or not
			if(mouseLeft[0] == 1 && mouseModifier == 0){
				grid[xcell][ycell].mat = m_air;
			}
			if(mouseRight[0] == 1 && grid[xcell][ycell].mat == m_air){
				gen_tree(xcell,ycell);
			}
		}
		#endif
		
		//evaluate_gravity(0,0,200,200);
		
        /// --------------------------------------------------------------------
        /// GRID PRINTING STUFF
        /// --------------------------------------------------------------------
        
        // apply initial black background to wipe the screen's slate clean.
		SDL_FillRect( screen , &screenRect , 0x000000);
		
		//apply sky
		apply_surface(0,0,skySurface,screen);
		// generate the grid image
		SDL_FillRect( gridSurface , &screenRect , 0x000000);
        generate_grid_surface(gridSurface);
		// apply the grid surface to the screen
        apply_surface(0,0, gridSurface, screen);
        
        
        //generate player rectangles
        SDL_Rect playerRect;
		playerRect.x = CELL_SIZE*(GRID_WIDTH/2);
		playerRect.y = CELL_SIZE*(GRID_HEIGHT/2 - player.height);
		playerRect.w = CELL_SIZE*player.width;
		playerRect.h = CELL_SIZE*player.height;
        // print the character
        SDL_FillRect(screen, &playerRect, player.color);
        // print the inventory
        if(inventoryView){
			inventory_display(&player.inv, screen);
			inventory_display(&testingInv2, screen);
			inventory_display(&testingInv1, screen);
        }
        //print the hotbar
        hotbar_display(&player, ITEM_SIZE/2, SCREEN_HEIGHT-1.5*ITEM_SIZE, screen);
        
        // print the debugging information to the screen.
        if(keyF3) print_debugging_information(x,y);
        
        
        
        //updates the screen
        SDL_Flip( screen );
        
        //----------------------------------------------------------------------
		// FPS calculation and variable handling
		//----------------------------------------------------------------------
        currentTicks = SDL_GetTicks();
        // it is officially the next second
        if(currentTicks >= ticksSinceLastFPSUpdate + 1000){
			// calculate the FPS
			FPS = cumulativeFrames;//(cumulativeFrames*1000 ) / (currentTicks-ticksSinceLastFPSUpdate);
			cumulativeFrames=0;				// reset cumulative amount of frames
			ticksSinceLastFPSUpdate = currentTicks;	// reset the last FPS update to the number of ticks now.
        }
        cumulativeFrames++;
        mouseRight[1] = mouseRight[0];	// make the previous mouse state equal the current one (for the next loop iteration)
        mouseLeft[1]  = mouseLeft[0];	// ^
        
        
    }// end while(quit == false)


    // quit the game
    quit_game(0,NULL);

    return 0;
}
