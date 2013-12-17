
/// this returns the pixel data of a pixel at a certain point on a surface (color and alpha in an Uint32)
Uint32 get_pixel(SDL_Surface *surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
	
    switch(bpp) {
    case 1:
        return *p;
        break;
	
    case 2:
        return *(Uint16 *)p;
        break;
	
    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;
        break;
	
    case 4:
        return *(Uint32 *)p;
        break;
	
    default:
        return 0;       /* shouldn't happen, but avoids warnings */
    }
}
/// this sets's a pixel's data (color and alpha)
void set_pixel(SDL_Surface *surf, int x, int y, Uint32 pixel){
    Uint32 *p = (Uint32 *)( surf->pixels + (surf->pitch * y) + x*surf->format->BytesPerPixel );
	*p = pixel;
}

void apply_surface_alpha_blend(int x, int y, SDL_Surface *source, SDL_Surface *dest){
	
	// nothing yet
	
}


void apply_surface( int x, int y,  SDL_Surface* source, SDL_Surface* destination ){
    //make a temporary rectangle to hold offsets
    SDL_Rect offset;
    
    //Give the offsets to the rectangle
    offset.x = x;
    offset.y = y;
    
    //Blit surface
    SDL_BlitSurface( source, NULL, destination, &offset );
}

// return 1 if applied surface
// returns 0 otherwise
/// this applies text at point (x,y) using the given font, message, and color.
int apply_text(SDL_Surface *datsurface, int x, int y, TTF_Font *theFont, char *theString, SDL_Color theColor){
	SDL_Surface *theSurface = NULL;
	if(strcmp(theString, " ") != 0)
		theSurface = TTF_RenderText_Blended(theFont, theString, theColor); // attempt rendering text
	if(theSurface == NULL){
		SDL_FreeSurface(theSurface); // clean the surface
		return 0; // rendering text didn't work. return 0
	}
	apply_surface(x, y, theSurface, datsurface); // apply surface at x, y
	SDL_FreeSurface(theSurface); // clean the surface
	return 1;
}


/// this generates a surface for a tooltop given proper parameters and returns a pointer to that surface.
SDL_Surface *generate_tootip(char *lines, int width, int height){
	// nothing here atm.
	
	//if all else fails, return 
	return NULL;
}


/// this applies a source surface (with a clipping rectangle) to a destination at point (x,y)
void apply_surface_clips( int x, int y,  SDL_Surface *source, SDL_Surface *destination, SDL_Rect *clip ){
    //make a temporary rectangle to hold offsets
    SDL_Rect offset;
    
    //Give the offsets to the rectangle
    offset.x = x;
    offset.y = y;
    
    //Blit surface
    SDL_BlitSurface( source, clip, destination, &offset );
}


// this will apply an outline to a box area described by an input SDL_Rectangle
void apply_outline(SDL_Surface *dest, SDL_Rect *box, unsigned short thickness, Uint32 fillColor){
	SDL_Rect border;
	
	// print top border
	border.x = box->x;
	border.y = box->y;
	border.w = box->w;
	border.h = thickness;
	SDL_FillRect(dest, &border, fillColor);
	
	// print bottom border
	border.y = box->y + box->w - thickness;
	SDL_FillRect(dest, &border, fillColor);
	
	// print right border
	border.x = box->x;
	border.y = box->y;
	border.w = thickness;
	border.h = box->h;
	SDL_FillRect(dest, &border, fillColor);
	
	// print left border
	border.x = box->x + box->w - thickness;
	SDL_FillRect(dest, &border, fillColor);
}


/// this will take a surface, scale it by a positive, non-zero integer and stuff it into another surface.
void scale_surface(SDL_Surface *sour, SDL_Surface *dest, short scalingFactor){
	
	if(scalingFactor < 1){
		handle_error(e_scalingFactor, "Could not scale image. scalingFactor is invalid.");
		return;
	}
	
	int i,j;	// i and j indexes into the original surface
	int it,jt; // i transform and j transform indexes
	Uint32 pixel;
	
	for(i=0; i<sour->w; i++){
		for(j=0; j<sour->h; j++){
			pixel = get_pixel(sour, i, j);
			
			for(it = 0; it<scalingFactor; it++){
				if(it+i*scalingFactor >= dest->w) return; // quit if you are out of bounds
				for(jt=0; jt<scalingFactor; jt++){
					if(jt+j*scalingFactor >= dest->h) break; // move to the next column if you are out of bounds
					set_pixel(dest, i*scalingFactor + it, j*scalingFactor + jt, pixel);
				}
			}
		}
	}
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
	for(i = 0; i <= GRID_WIDTH; i++){
		// this is used to index into the grid
		ig = i+i_start;
		if(ig < 0) continue; // if ig is less than the minimum bound, make it equal to the minimum array index value.
		if(ig >= GRID_WIDTH_ELEMENTS) break; // if ig is larger than the largest array index value, quit.
        for(j = 0; j <= GRID_HEIGHT; j++){
        	// this is used to index into the grid
			jg = j+j_start;
			if(jg < 0) continue; // if jg is less than the minimum bound, make it equal to the minimum bound
			if(jg >= GRID_HEIGHT_ELEMENTS) break; // if jg is larger than the largest array index, quit.
			
        	//only print the material if it is not air
			if(grid[ig][jg].mat != m_air){
				myRectangleMat.x = i*CELL_SIZE;
				myRectangleMat.y = j*CELL_SIZE;
				SDL_FillRect( surfaceToPrintTo , &myRectangleMat , mats[grid[ig][jg].mat].color);
			}
        }
    }   
}



#define grad_linear 0
#define grad_radial 1


///this function produces a gradient on datsurface.
// the gradient will only be printed inside of the gradClip rectangle.
// at point 1 (x1,y1) there will be color1.
// at ponit 2 (x2,y2) there will be color2.
///horizontal gradients (y1=y2) and verticle gradients (x1=x2) are very fast compared to gradients at different slopes (where neither x1=x2 or y1=y2)
///if you are looking for speed, verticle/horizontal gradients are the way to go.
void gradient(SDL_Surface *datsurface, SDL_Rect *gradClip, int x1, int y1, int x2, int y2, Uint32 color1, Uint32 color2, unsigned int gradientType){
	
	// there is no point in printing a gradient with only a single point
	if(x1 == x2 && y1 == y2){
		return;
	}
	
	// this is the integer used for the color of (i,j) pixel in the gradient
	long unsigned int color;
	// these are all one-time calculations. these values will be used in the loop.
	unsigned int alp1 = color1/0x1000000;
	unsigned int red1 = (color1/0x10000)%0x100;
	unsigned int gre1 =(color1/0x100)%0x100;
	unsigned int blu1 = color1%0x100;
	unsigned int alp2 = color2/0x1000000;
	unsigned int red2 = (color2/0x10000)%0x100;
	unsigned int gre2 =(color2/0x100)%0x100;
	unsigned int blu2 = color2%0x100;
	
	// this is for displaying the pixel at (i,j)
	SDL_Rect pixelRect;
	pixelRect.w = 1;
	pixelRect.h = 1;
	
	
	
	float m, b1,b2,diff,b,x,y;
	
	int i,j; // variables used to loop through the columns and row respectively.
	
	// if the gradient has no slope
	if(x1 == x2){
		diff = (y2-y1);
		pixelRect.x = gradClip->x;
		pixelRect.w = gradClip->w;
		// we will print rectangles that are 1-pixel tall horizontal line segments
		for(j=gradClip->y; j<gradClip->y+gradClip->h; j++){	// loop through the rows    of the gradient clip
			y = (float)j;	// calculate y-intercept for this point (i,j)
			if(y < y1) 					// the pixel is not between the two points
				color = color1;			// choose color1
			else if( y > y2) 			// the pixel is not between the two points
				color = color2; 		// choose color2
			else{
				color = (int)((alp1*(y2-y) + alp2*(y-y1))/diff)*0x1000000;	// calculate alpha color component
				color+= (int)((red1*(y2-y) + red2*(y-y1))/diff)*0x10000;	// calculate red   color component
				color+= (int)((gre1*(y2-y) + gre2*(y-y1))/diff)*0x100;		// calculate green color component
				color+= (int)((blu1*(y2-y) + blu2*(y-y1))/diff);			// calculate blue  color component
			}
			pixelRect.y = y;
			SDL_FillRect(datsurface, &pixelRect, color);
		}
	}
	// if the gradient has infinite(very large slope
	else if(y1 == y2){
		// we will print rectangels that are 1-pixel wide verticle line segments
		diff = (x2-x1);
		pixelRect.y = gradClip->y;
		pixelRect.h = gradClip->h;
		// we will print rectangles that are 1-pixel tall horizontal line segments
		for(i=gradClip->x; i<gradClip->x+gradClip->w; i++){	// loop through the rows    of the gradient clip
			x = (float)i;	// calculate y-intercept for this point (i,j)
			if(x < x1) 					// the pixel is not between the two points
				color = color1;			// choose color1
			else if( x > x2) 			// the pixel is not between the two points
				color = color2; 		// choose color2
			else{
				color = (int)((alp1*(x2-x) + alp2*(x-x1))/diff)*0x1000000;	// calculate alpha color component
				color+= (int)((red1*(x2-x) + red2*(x-x1))/diff)*0x10000;	// calculate red   color component
				color+= (int)((gre1*(x2-x) + gre2*(x-x1))/diff)*0x100;		// calculate green color component
				color+= (int)((blu1*(x2-x) + blu2*(x-x1))/diff);			// calculate blue  color component
			}
			pixelRect.x = x;
			SDL_FillRect(datsurface, &pixelRect, color);
		}
	}
	// if the slope is a non-zero finite value
	else{
		//this is for the lines perpendicular to the original line. it is -1/original_line_slope.
		//this is the slope that will be used when calculating the color of each pixel
		m = ((float)(x1-x2))/((float)(y2-y1));	// calculate slope of the line perpendicular to the gradient line
		b1 = y1 - m*x1;							// rearranged form of  y = mx + b  to calculate the the y-intercept for b1 
		b2 = y2 - m*x2;							// similar shenanigans for b2
		diff = b2 - b1;						// calculate the difference in the y-intercept terms
	
		for(i=gradClip->x; i<gradClip->x+gradClip->w; i++){			// loop through the columns of the gradient clip
			for(j=gradClip->y; j<gradClip->y+gradClip->h; j++){		// loop through the rows    of the gradient clip
				pixelRect.x = i;
				pixelRect.y = j;
				b = (float)j - m*(float)i;	// calculate y-intercept for this point (i,j)
				if(b < b1) 					// the pixel is not between the two points
					color = color1;			// choose color1
				else if( b > b2) 			// the pixel is not between the two points
					color = color2; 		// choose color2
				else{
					color = (int)((alp1*(b2-b) + alp2*(b-b1))/diff)*0x1000000;	// calculate alpha color component
					color+= (int)((red1*(b2-b) + red2*(b-b1))/diff)*0x10000;	// calculate red   color component
					color+= (int)((gre1*(b2-b) + gre2*(b-b1))/diff)*0x100;		// calculate green color component
					color+= (int)((blu1*(b2-b) + blu2*(b-b1))/diff);			// calculate blue  color component
				}
				SDL_FillRect(datsurface, &pixelRect, color);
				
			}
		}
	}
}


void generate_sky(SDL_Surface *datsurface, int width, int height){
	
	SDL_Rect screenRect;
	screenRect.x = 0;
    screenRect.y = 0;
    screenRect.w = width;
    screenRect.h = height;
    
    SDL_FillRect(datsurface, &screenRect, 0); // blank the sky
    //create verticle gradient from top to bottom with sky type colors
    gradient(datsurface, &screenRect, 0, 0, 0, height, 0xff172092, 0xff6cb8f6, 0);
}


void print_debugging_information(int mousex, int mousey){
	//----------------------------------------------------
	// PRINT PLAYER POSITION INFORMATION
	//----------------------------------------------------
	static char textMessage[100]; // this holds the string that tells you what the player's position is.
	static char textDummy[100]; // this holds misc text values generated by the itoa() function.
	
	static SDL_Color white;
	white.r = white.g = white.b = 0xff; // set white color
	short row1 = -4, rowoffset = 20;
	short row2 = row1 + rowoffset;
	short row3 = row2 + rowoffset;
	
	strcpy(textMessage,"x_pos=");							// "x_pos="
	itoa((int)player.x_pos,textDummy,10);					// generate integer portion of x pos in string form
	strcat(textMessage,textDummy);							// "x_pos=1234"
	strcat(textMessage,".");								// "x_pos=1234."
	itoa(player.x_pos_f*1000,textDummy,10);					// generate the first three decimal places of x_pos
	strcat(textMessage,textDummy);							// "x_pos=1234.567"
	apply_text(screen, 3, row1, font16, textMessage, white);	// apply text to screen
	
	strcpy(textMessage," y_pos=");							// "y_pos="
	itoa((int)player.y_pos,textDummy,10);					// generate integer portion of y_pos in string form
	strcat(textMessage,textDummy);							// "y_pos=1234"
	strcat(textMessage,".");								// "y_pos=1234."
	itoa(player.y_pos_f*1000,textDummy,10);					// generate the first three decimal places of y_pos
	strcat(textMessage,textDummy);							// "y_pos=1234.567"
	apply_text(screen,145,row1, font16, textMessage, white);// apply text to screen
	
	//----------------------------------------------------
	// PRINT MOUSE LOCATION INFO
	//----------------------------------------------------
	strcpy(textMessage, "x=");								// "x="
	itoa(mousex, textDummy, 10);							// generate string of the mousex value
	strcat(textMessage, textDummy);							// "x=1920"
	apply_text(screen,300,-2, font16, textMessage, white);	// apply text to screen
	
	strcpy(textMessage, "y=");								// "y="
	itoa(mousey, textDummy, 10);							// generate string of the mousey value
	strcat(textMessage, textDummy);							// "y=1080"
	apply_text(screen,372,row1, font16, textMessage, white);// apply text to screen
	//----------------------------------------------------
	// PRINT TICKS and FPS INFORMATION
	//----------------------------------------------------
	strcpy(textMessage, "gt=");								// "gt=" means "game time equals"
	itoa(game_time(), textDummy, 10);					// generate string of the number of ticks
	strcat(textMessage, textDummy);							// "Ticks=123456789"
	apply_text(screen,100,row2, font16, textMessage, white);// apply text to screen
	
	strcpy(textMessage, "FPS=");							// "FPS="
	itoa(FPS, textDummy, 10);								// generate string of the current FPS
	strcat(textMessage, textDummy);							// "FPS=31"
	apply_text(screen,3,row2, font16, textMessage, white);	// apply text to screen
	
	//----------------------------------------------------
	// PRINT onCollMat value of the player
	//----------------------------------------------------
	strcpy(textMessage,"onMat=");
	if(player.onCollMat)
		strcat(textMessage,"1");
	else
		strcat(textMessage,"0");
	apply_text(screen,3,row3, font16, textMessage, white);	// apply text to screen
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



Uint32 INVENTORY_COLOR =	0xffaaaaaa;
Uint32 INVENTORY_BORDER_COLOR =	0xff222222;
// this is the thickness of the inventory border
#define INVENTORY_BORDER (itemSize/4)

void inventory_display(struct inventoryData *inv, int x, int y, short itemSize, short guiSize, SDL_Surface *dest){
	
	SDL_Rect guirect;								// this is the rectangle that the items are printed into
	guirect.w=itemSize*inv->width+2*INVENTORY_BORDER;	// calculate the gui's width  in pixels
	guirect.h=itemSize*inv->height+2*INVENTORY_BORDER;// calculate the gui's height in pixels
	guirect.x = x-INVENTORY_BORDER;						// calculate the x position in pixels
	guirect.y = y-INVENTORY_BORDER;						// calculate the y position in pixels
	SDL_FillRect(dest, &guirect, INVENTORY_BORDER_COLOR);	// fill the border around the inventory
	
	guirect.w = itemSize*inv->width;				// calculate the gui's width  in pixels
	guirect.h = itemSize*inv->height;				// calculate the gui's height in pixels
	guirect.x = x;									// calculate the x position in pixels
	guirect.y = y;									// calculate the y position in pixels
	SDL_FillRect(dest, &guirect, INVENTORY_COLOR);	// fill the gui rect.
	
	int i,j; 										// used for indexing 
	SDL_Rect itemclip;								// this is used for selecting the item from the item_set
	itemclip.w = itemclip.h = itemSize;			// set the width and height of the item clip
	
	// loop through all the user's items and print them all in the inventory.
	for(j=0; j<inv->height; j++){
		for(i=0; i<inv->width; i++){
			if(inv->slot[j*inv->width + i].item == i_none)continue;	// don't print empty item slots
			// apply the texture of the item at the right location within the inventory
			apply_item(inv->slot[j*inv->width + i].item, i*itemSize+guirect.x, j*itemSize+guirect.y, itemSize, guiSize, dest);
		}
	}
}

// this is the area of the hotbar
SDL_Rect hotbarRect;
#define HOTBAR_SELECTION_COLOR 0xffff0000

void hotbar_display(struct playerData *datplayer, int x, int y, short itemSize, short guiSize, SDL_Surface *dest){
	
	struct inventoryData *datinv = &datplayer->inv;				// store a pointer to the user's inventory for quick and painless reference
	
	short i;													// this will index through the player's hotbar row.
	short row = datinv->height-1;								// pick the row you want to display from the user's inventory
	
	// this stuff is for printing the rectangle of the hotbar.
	hotbarRect.x = x;									// x position
	hotbarRect.y = y;									// y position
	hotbarRect.w = itemSize*datinv->width;				// width 
	hotbarRect.h = itemSize;							// height
	SDL_FillRect(dest, &hotbarRect, INVENTORY_COLOR);
	
	int itemType;
	
	// this prints all of the items in the hotbar
	for(i=0; i<datinv->width; i++){
		itemType = datinv->slot[row*datinv->width+i].item;		// get item type
		if(itemType == i_none)continue;							// don't print empty inventory slots
		apply_item(itemType, hotbarRect.x + i*itemSize, hotbarRect.y, itemSize, guiSize, dest);
	}
	
	// print a selectoin box around the item the user is currently selecting
	int thickness = 1<<(GUI_SIZE-1);							// generate the line thickness
	
	SDL_Rect outline;											// generate the box where the highlight needs to be printed
	outline.x = hotbarRect.x+itemSize*datplayer->hotbarIndex;	// ^
	outline.h = itemSize;										// ^
	outline.y = hotbarRect.y;									// ^
	outline.w = itemSize;										// ^
	
	apply_outline(dest, &outline, thickness, HOTBAR_SELECTION_COLOR);		// apply the outline to the player's selcted inventory spot.
}







