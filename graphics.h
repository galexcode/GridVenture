#define EXTRA_GRID_SURFACE_LENGTHS 2
void init_gridSurface(){
	gridSurface = create_surface(GRID_WIDTH_ELEMENTS+EXTRA_GRID_SURFACE_LENGTHS*MAX_CELL_SIZE,GRID_HEIGHT_ELEMENTS+EXTRA_GRID_SURFACE_LENGTHS*MAX_CELL_SIZE);
}

/// this will print to the screen each material in each cell.
void generate_grid_surface(SDL_Surface *surfaceToPrintTo){
    
   
    
    int i,j;
	SDL_Rect myRectangleMat;
	// constant cell sizes
	myRectangleMat.w = CELL_SIZE;
	myRectangleMat.h = CELL_SIZE;
	
	//this is the upper corner of where the screen starts displaying grid data.
	int i_start = (int)( (player.x_pos) - GRID_WIDTH/2  );
	int j_start = (int)( (player.y_pos) - GRID_HEIGHT/2 );
	// these are for indexing into the grid
	int ig,jg;
	//these are how far off the grid the player is.
	//float adjust_x = -(player.x_pos - ((int)player.x_pos) )*CELL_SIZE;
	//float adjust_y = -(player.x_pos - ((int)player.x_pos) )*CELL_SIZE;
	
	// print out the grid
	for(i = 0; i < GRID_WIDTH+EXTRA_GRID_SURFACE_LENGTHS; i++){
		// this is used to index into the grid
		ig = i+i_start;
		if(ig < 0) ig = 0; // if ig is less than the minimum bound, make it equal to the minimum array index value.
		if(ig > GRID_WIDTH_ELEMENTS+2) break; // if ig is larger than the largest array index value, quit.
        for(j = 0; j < GRID_HEIGHT+EXTRA_GRID_SURFACE_LENGTHS; j++){
        	// this is used to index into the grid
			jg = j+j_start;
			if(jg < 0) jg = 0; // if jg is less than the minimum bound, make it equal to the minimum bound
			if(jg > GRID_HEIGHT_ELEMENTS+EXTRA_GRID_SURFACE_LENGTHS) break; // if jg is larger than the largest array index, quit.
			
        	//only print the material if it is not air
			if(grid[ig][jg].mat != m_air){
				myRectangleMat.x = i*CELL_SIZE;
				myRectangleMat.y = j*CELL_SIZE;
				SDL_FillRect( gridSurface , &myRectangleMat , mats[grid[ig][jg].mat].color);
			}
        }
    }
}




//this draws a line on the screen from point (x1,y1) to point (x2,y2)
// this draws the line (mathematically) from the centers of the two pixels.
void draw_line(SDL_Surface *theSurface, int x1, int y1, int x2, int y2, int thickness, int lineColor){
	//this is the rectangle describing the pixel to color in.
	SDL_Rect myPixel;
	myPixel.w = 1;
	myPixel.h = 1;
	
	//handle lines with no x-difference
	if(x1 == x2){
		// this handles the special case that the line is a point
		if(y1 == y2){
			// draw a point
			myPixel.x = x1;
			myPixel.y = y1;
			SDL_FillRect(theSurface, &myPixel, lineColor);
			// done drawing a point. that's all folks!
			return;
		}
		//this handles the special case the the line is a verticle line (slope infinity)
		// this swaps the x and y values so that the function draws a line with a finite slope (zero)
		draw_line(theSurface,y1,x1,y2,x2,thickness,lineColor);
		return;
	}
	
	float slope = (float)(y2-y1)/(float)(x2-x1);
	//generate an absolute value of the slope
	float absval_slope = slope;
	if(slope < 1) absval_slope *= -1;
	float x,y;
	//this is used to mathematically determine where the line should be.
	float line_value;
	
	// if the absolute value of the slope is less than 1, index through the x values
	if(absval_slope < 1){
		/* OLD CODE FOR GRADUAL SLOPE LINES
		// swap the points around if x1 > x2
		if(x1 > x2){
			
			int tempval=x1;	// store x1
			x1 = x2;		// put x2 into x1
			x2 = tempval;	// put the value that used to be in x1 into x2.
			tempval = y1;	// store y1
			y1 = y2;		// put y2 into y1
			y2 = tempval;	// put the value that used to be in y1 into y2.
		}
		
		for(x=x1+0.5,myPixel.x=x1; x<x2; x+=1.0,myPixel.x++){
			line_value = slope*x +y1;
			myPixel.y = line_value; // integer assignment. truncate decimal places
			// if the line_value has decimal place value greater than or equal to 0.5, then round up.
			if(line_value-(int)line_value >= 0.5) myPixel.y++;
				
			SDL_FillRect(theSurface, &myPixel, lineColor);
		}
		*/
		//all of this assumes that x1 <= x2
		float pixel_offset = 0.5;
		//x2 is greater than x1
		int low_x = x1;
		int high_x = x2;
		int ref_y = y1;		// this is the reference y. this is where we start
		
		// this takes care of the special case of x1 being bigger than x2.
		if(x1 > x2){
			pixel_offset *= -1;
			//x2 is less than x1
			low_x = x2;
			high_x = x1;
			ref_y = y2;
		}
		
		
		for(x=low_x+pixel_offset,myPixel.x=low_x; x<high_x; x+=1,myPixel.x++){
			line_value = (x-low_x)*slope + ref_y;
			myPixel.y = line_value; // integer assignment. truncate decimal places
			// if the line_value has decimal place value greater than or equal to 0.5, then round up.
			if(line_value-(int)line_value > 0.5) myPixel.y++;
				
			SDL_FillRect(theSurface, &myPixel, lineColor);
		}
	}
	// otherwise, the absolute value of the slope is greater to or equal to one, so index through the y values
	else{
		/*
		// swap the points around if y1 > y2
		if(x1 > x2){
			int tempval=x1;	// store x1
			x1 = x2;		// put x2 into x1
			x2 = tempval;	// put the value that used to be in x1 into x2.
			tempval = y1;	// store y1
			y1 = y2;		// put y2 into y1
			y2 = tempval;	// put the value that used to be in y1 into y2.
		}
		*/
		
		
		//all of this assumes that y1 <= y2
		float pixel_offset = 0.5;
		//y2 is greater than y1
		int low_y = y1;
		int high_y = y2;
		int ref_x = x1;		// this is the reference x. this is where we start
		
		// this takes care of the special case of y1 being bigger than y2.
		if(y1 > y2){
			pixel_offset *= -1;
			//y2 is less than y1
			low_y = y2;
			high_y = y1;
			ref_x = x2;
		}
		
		
		for(y=low_y+pixel_offset,myPixel.y=low_y; y<high_y; y+=1,myPixel.y++){
			line_value = (y-low_y)/slope + ref_x;
			myPixel.x = line_value; // integer assignment. truncate decimal places
			// if the line_value has decimal place value greater than or equal to 0.5, then round up.
			if(line_value-(int)line_value > 0.5) myPixel.x++;
				
			SDL_FillRect(theSurface, &myPixel, lineColor);
		}
	}
	
}




