
#define SHOW_WORLD_GEN 1

// these are the different world types that can be generated.
// these values are not meant to be combined through bit-wise ORing.
#define w_normal			1
#define w_hellscape			2


// these are the different worlds flags that you can send the world_gen function.
// these flags should be powers of 2 and thus are meant to be bit-wise ORed together.
// for example: wf_rains

// no world flags yet  xD  i will have to make some
#define wf_display_generation	1
//#define wf_


/// recursive function that generates a landscape inside an array
// the top_material is an array to a pointer that tell you the height of the landscape at any given point.
// the function will evaluate from lowIndex to highIndex in the top_material[] array.
// landscape material is only a necessary variable if you plan on showing the creation of the landscape.
// if you don't care about visually showing the landscape being built, then just send landscapeMaterial anything (e.g. m_air, m_earth, m_mud, etc...)
void gen_landscape(int *top_material, int lowIndex, int highIndex, int matLowBound, int matHighBound, float slope, int landscapeMaterial){
	
	// stop evaluating if there is no space between the lowIndex and the highIndex (or if, for some crazy reason, the lowIndex because HIGHER than the highIndex...)
	if(lowIndex+1 >= highIndex) return;
	// find the middleIndex between the low and high Indexes.
	int middleIndex = (lowIndex+highIndex)/2;
	
	//calculate the value at the midpoint
	top_material[middleIndex] = (top_material[lowIndex] + top_material[highIndex])/2;		// take the average value between the indexes (linear interpolation)
	top_material[middleIndex] += get_rand((int)(((float)(lowIndex-highIndex)*slope)/2.0), (int)(((float)(highIndex-lowIndex)*slope)/2.0f));		// generate some variance to the average value between the indexes.
	
	// if the new top_material value is out of bounds, try again
	if(top_material[middleIndex] >= matHighBound || top_material[middleIndex] < matLowBound){
		gen_landscape(top_material, lowIndex, highIndex, matLowBound, matHighBound, slope, landscapeMaterial);
		return;
	}
	// if the user wants to visually see the world getting built, then so be it.
	/* THIS IS THE OLD WAY OF DISPLAYING THE GENERATION OF THE WORLD
	#if(SHOW_WORLD_GEN)
	grid[middleIndex][GRID_HEIGHT_ELEMENTS-1-top_material[middleIndex]].mat = landscapeMaterial;
	if(highIndex-lowIndex > GRID_WIDTH_ELEMENTS/64){
		print_cells();
		SDL_Flip(screen);
	}
	#endif // SHOW_WORLD_GEN
	*/
	//now recursively preform the function on the two new midpoints
	gen_landscape(top_material, lowIndex, middleIndex, matLowBound, matHighBound, slope, landscapeMaterial);
	gen_landscape(top_material, middleIndex, highIndex, matLowBound, matHighBound, slope, landscapeMaterial);
}


void gen_landscape_relative(int *top_material, int top_material_relative, int lowIndex, int highIndex, int matLowBoundRelative, int matHighBoundRelative, int slope){
	
}


/// this tries to generate a tree in grid[][] with the base at [x][y].
// on success, it returns the height of the trunk.
// on failure, it returns false (0)
int gen_tree(int x, int y){
	
	
	//-------------------------------
	// generate logs
	//-------------------------------
	int trunkHeight = get_rand(4,8);
	int j;
	for(j=0; j<trunkHeight; j++){
		// check to see if the cell is anything other than air or if it is out of bounds
		if(!within_grid_bounds(x,y-j) || grid[x][y-j].mat != m_air ){
			j--; // decrement to the previous cell that was turned into m_log
			for(; j>=0; j--){		// cycle through all the log materials that were placed and delete them all.
				grid[x][y-j].mat = m_air;
			}
			return false;	// return a failure flag
		}
		// if everything checks out, turn this material to log.
		grid[x][y-j].mat = m_log;
	}
	
	
	
	//-------------------------------
	// generate leaves
	//-------------------------------
	int numberOfLeaves = get_rand(48,64);	// generate a random number of how many leaves need to be placed
	int l=0;								// this keeps track of how many leaves have been placed
	int failures = 0;						// this keeps track of how many times the loop has failed to find a suitable place to put leaves
	int MAX_LEAF_FAILURES = 20;				// this is the maximum tolerable amount of subsequent failures.
	int leafx,leafy;						// these are the indexes into the grid[][]
	
	while(l<numberOfLeaves){				// while the number of leaves placed is less than the number we need to place
		leafx = x;							// set leafx and leafy to the top of the tree
		leafy = y - j;
		while(1){							// loop until success or failure in finding a suitable place to set to leaves.
			leafx += get_rand(-1,1);		// randomly move leafx and leafy around
			leafy += get_rand(-1,1);
			if(!within_grid_bounds(leafx,leafy)){				// make sure the cell [leafx][leafy] is inside the valid grid area
				failures++;
				break;
			}
			if(grid[leafx][leafy].mat == m_leaves) continue;	// contniue searching if you are on leaves
			else if(grid[leafx][leafy].mat == m_air){			// found a suitable
				grid[leafx][leafy].mat = m_leaves;				// place leaves
				failures = 0;
				l++;											// increment the leaves count variable
				break;											// start the next itteration of the outer while() loop
			}
			else{												// found a material that is not air or other leaves. failure to place leaves on itteration l is here.
				failures++;
				break;											// break out of this loop to reset the leafx and leafy positions to right above the tree trunk.
			}
		}
		// if there were enough consecutive failuers in trying to place leaves, quit.
		if(failures >= MAX_LEAF_FAILURES)break;
	}
	return trunkHeight;
}



/// this function will overwrite the data in grid[][] and generate a world inside it!
// send it a world type and a worldflag and it should 
void gen_world(int worldType, long long  unsigned int worldFlag){
	
	// initial settings. zoom all the way out and check out the map in all its glory
	paused = 1;
	CELL_SIZE = 1;
	verify_grid_and_cell_size();
	
	//clean grid
	reset_cells();
	
	int i,j;
	/// all of these numbers are relative to the bottom cell in the grid (grid[i][GRID_WIDTH_ELEMENTS-1])
	
	// these just tell us the height band of where top of the stone layer can be generated
	// stoneline_max is exclusive, stoneline_min is inclusive. the domain would be [stoneling_min,stoneline_max) for integer numbers.
	int stoneline_max=500, stoneline_min=200;
	// this is an array that tells us for myColumn, the highest point where there is stone = top_stone[myColumn]
	int top_stone[GRID_WIDTH_ELEMENTS];
	// the larger the slope is,  the more steep and sharp the slopes can be.
	// the smaller the slope is, the smoother and duller the landscape will be.
	float stone_slope = 0.9;
	
	//set default values to top_stone array (this is just so that the elements are not left un-initialized. they should all be overwritten
	for(i=0; i<GRID_WIDTH_ELEMENTS; i++){
		top_stone[i] = 0; // default value. this will get overwritten.
	}
	
	//get initial points at the left and right sides of the map
	top_stone[0]						= get_rand(stoneline_min, stoneline_max-1);
	top_stone[GRID_WIDTH_ELEMENTS-1]	= get_rand(stoneline_min, stoneline_max-1);
	
	// generate the top_stone array
	gen_landscape(top_stone, 0, GRID_WIDTH_ELEMENTS-1, stoneline_min, stoneline_max, stone_slope, m_stone);
	
	/// this only works (well or at all) with a screen width of 1080 pixels
	if(worldFlag & wf_display_generation){
		SDL_Rect screenRect;
		screenRect.x = screenRect.y = 0;
		screenRect.w = SCREEN_WIDTH;
		screenRect.h = SCREEN_HEIGHT;
		// dist is the distance between the sample points in the grid[][] array
		int sections;
		
		for(sections = 1; sections<=SCREEN_WIDTH/2; sections<<=1){
			//blank screen
			SDL_FillRect(screen, &screenRect, 0x000000);
			//draw_line(screen, 0,GRID_HEIGHT_ELEMENTS-1-top_stone[0], GRID_WIDTH_ELEMENTS/sections, GRID_HEIGHT_ELEMENTS-1-top_stone[GRID_WIDTH_ELEMENTS/sections], 1, mats[m_stone].color);
			for(i=0; i<sections; i++){
				draw_line(screen, i*SCREEN_WIDTH/sections,  SCREEN_HEIGHT-1-top_stone[i*SCREEN_WIDTH/sections],  (i+1)*(SCREEN_WIDTH-1)/sections,  SCREEN_HEIGHT-1-top_stone[(i+1)*(SCREEN_WIDTH-1)/sections],  1, 0x0bf100);
				draw_line(screen, i*SCREEN_WIDTH/sections,  SCREEN_HEIGHT-1-top_stone[i*SCREEN_WIDTH/sections]-4,  (i+1)*(SCREEN_WIDTH-1)/sections,  SCREEN_HEIGHT-1-top_stone[(i+1)*(SCREEN_WIDTH-1)/sections]-4,  1, 0x0bf100);
			}
			
			// display the lines that were just printed
			SDL_Flip(screen);
			// wait for a bit
			SDL_Delay(100);
		}
	}
	
	
	//fill up the stone area.
	for(i=0; i<GRID_WIDTH_ELEMENTS; i++){
		for(j=GRID_HEIGHT_ELEMENTS-top_stone[i]-1; j<GRID_HEIGHT_ELEMENTS; j++){
			grid[i][j].mat = m_stone;
		}
	}
	
	//int top_earth[GRID_WIDTH_ELEMENTS]; // this keeps track of the top layer of earth.
	
	int earth_average_above_stone = 4; // average of 4 blocks of dirt above stone.
	
	for(i=0; i<GRID_WIDTH_ELEMENTS; i++){
		for(j=GRID_HEIGHT_ELEMENTS-top_stone[i]-2; j>=GRID_HEIGHT_ELEMENTS-top_stone[i]-2-earth_average_above_stone; j--){
			grid[i][j].mat = m_earth;
		}
	}
	
	i=0;j=0;
	while(i<GRID_WIDTH_ELEMENTS){
		i+= get_rand(5,60);
		for(j=SCREEN_WIDTH-1; j>=0; j--){
			if(grid[i][j].mat == m_air){
				gen_tree(i,j);	// if you find an open spot, generate a tree there
				break;
			}
		}
	}
	
}





