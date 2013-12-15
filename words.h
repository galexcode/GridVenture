

/// THIS FUNCTION WILL APPLY A TRANSFORMATION TO TURN A SOLID STRING OF TEXT INTO MULTIPLE SMALLER STRINGS OF TEXT CALLED "LINES" 
// msg is the string of text that will be broken up into lines of text.
// width is the number of characters on each line. NOT INCLUDING THE NULL CHARACTER that exists at the end of each line. so lines MUST BE OF SIZE [height][width+1]
// height is the number of lines tall the paragraph will be limited to.
// lines is a pointer to a string that is height x width.
// resultWidth  is the biggest width that any one of the lines ended up being after the transformation.
// resultHeight is how many lines the paragraph transformed into.
	// both resultWidth and resultHeight are pointers to memory where the function will store the values of the final width and height of the transformed paragraph.
	// when the function is called, you will need to provide a pointer to a memory location where these values can be stored.

/// all of the possible error codes for paragraph are defined here
#define PARAGRAPH_SUCCESS					 0
#define PARAGRAPH_INVALID_MSG				-1
#define PARAGRAPH_INVLIAD_LINES				-2
#define PARAGRAPH_INVLIAD_WIDTH				-3
#define PARAGRAPH_INVLIAD_HEIGHT			-4
#define PARAGRAPH_INVALID_FINALWIDTHPTR		-5
#define PARAGRAPH_INVALID_FINALHEIGHTPTR	-6
int paragraph_to_lines(char *msg, int width, int height, char *lines, int *resultWidthPtr, int *resultHeightPtr){
	
	//----------------------------------------------------------
	// declaring variables
	//----------------------------------------------------------
	
	//this keeps track of the current line being processed
	int curline;
	// keeps track of the characters that have been processed so far.
	int c;
	//used for indexing
	int i,j;
	// used to find a space in a string
	int foundSpace;
	// used to keep track of whether or not we have found the end of the paragraph.
	int foundEnd = false;
	// used to check where spaces are
	int curch;
	// these keep track of the resulting width
	int resultWidth=0;
	
	//----------------------------------------------------------
	// checking for bad input and NULL pointers
	//----------------------------------------------------------
	#if(PARAGRAPH_ERROR_OUTPUT)
	fprintf(errorFile,"\n\n"); // initial spacing
	#endif
	//check to make sure that the paragraph is valid
	if(msg == NULL){
		#if(PARAGRAPH_ERROR_OUTPUT)
		fprintf(errorFile, "paragraph_to_lines(): NULL msg argument.\n");
		#endif
		// this is the message that will be returned
		msg = "paragraph_to_lines(): NULL msg argument.";
	}
	//check to make sure the lines is a valid pointer
	if(lines == NULL){
		#if(PARAGRAPH_ERROR_OUTPUT)
		fprintf(errorFile, "paragraph_to_lines(): NULL lines argument.");
		#endif
		return -1; // return if the pointer to the lines is invalid
	}
	// check to make sure height is valid
	if(height<1){
		#if(PARAGRAPH_ERROR_OUTPUT)
		fprintf(errorFile, "paragraph_to_lines(): Invalid height! height = %d.", height);
		#endif
		return -2;
	}
	// check to make sure width  is valid
	if(width<1){
		#if(PARAGRAPH_ERROR_OUTPUT)
		fprintf(errorFile, "paragraph_to_lines(): Invalid width! height = %d.", width);
		#endif
		return -3;
	}
	
	//----------------------------------------------------------
	// preliminary variable setup and calculation
	//----------------------------------------------------------
	
	// set the entire lines array to spaces
	for(j=0; j<height; j++){
		for(i=0; i<width; i++){
			lines[j*(width+1) + i]= ' ';
		}
	}
	
	// calculate the length of the input msg paragraph
	int msgLen = strlen(msg);
	#if(PARAGRAPH_DEBUG_OUTPUT)
	fprintf(debugFile,"\n\nmsgLen = %d",msgLen);
	#endif
	
	//----------------------------------------------------------
	// transform the paragraph into lines of text
	//----------------------------------------------------------
	
	for(curline=0,c=0; curline<height && !foundEnd; curline++){
		// find the end of this line
		// set the found space variable to false
		foundSpace = false;
		for(i=-1; i<width; i++){
			
			if(c >= msgLen){									// make sure you are not past where you are supposed to be.
				foundEnd = true;
				break;
			}
			
			curch = c + width - i - 1;							// calculate the current character being evaluated
			if(curch > msgLen) continue;						// if you are indexing outside the valid msg data area, then try again
			if(msg[curch] == ' ' && !foundSpace){				// check to see if you have found a space
				foundSpace = curch;								// record where you found the space
				#if(PARAGRAPH_DEBUG_OUTPUT)
				fprintf(debugFile,"foundSpace at %d: %c%c%c%c\n",foundSpace,msg[curch+1],msg[curch+2],msg[curch+3],msg[curch+4]);
				#endif
			}
			if(curch == msgLen){								// found end of the msg string
				memmove(lines+curline*(width+1),msg+c,curch-c);	// copy the last line
				c = msgLen;										// set c equal to the message length. the for loop will detect this and quit processing the string
				if(curch-c>resultWidth) resultWidth = curch-c;	// this updates the resultWidth value if the current width is bigger than the biggest width checked so far
				break;
			}
			else if(foundSpace&&msg[curch]!=' '){				// if you have found a space before, but the current character is not a space OR if you are at the end of the msg
				memmove(lines+curline*(width+1),msg+c,width-i);	// move the string from the input paragraph to one of the output string.
				c=foundSpace+1;									// the current working area is one more step after you found the space. this assumes that there will not be any areas with two spaces in a row.
				if(width-i>resultWidth) resultWidth = width-i;	// this updates the resultWidth value if the current width is bigger than the biggest width checked so far
				break;
			}
			else if(i == width-1){								// if this is the last iteration and you have not found a suitable place to break the sentence, then simply cut off the word mid word.
				memmove(lines+curline*(width+1),msg+c,width);	// copy the line from the paragraph to the one of the output strings
				c += width;										// adjust c to <width> number of characters into this really long word.
				if(width > resultWidth) resultWidth = width;	// this updates the resultWidth value if the current width is bigger than the biggest width checked so far
				break;
			}
		}
		#if(PARAGRAPH_DEBUG_OUTPUT)
		fprintf(debugFile,"c=%d\n",c);
		#endif
	}
	
	//----------------------------------------------------------
	// cleaning up shop
	//----------------------------------------------------------
	
	//ensure that each string is properly terminated on each line
	for(j=0; j<height; j++){
		lines[j*(width+1) + width] = '\0'; // terminate each string.
	}
	
	if(resultHeightPtr != NULL)			// make sure the pointer is valid
		*resultHeightPtr = curline;		// save the number of lines the split up paragraph takes up.
	
	if(resultWidthPtr != NULL)			// make sure the pointer is valid
		*resultWidthPtr = resultWidth;	// save the greatest width out of all the lines
	
	return PARAGRAPH_SUCCESS; // success
}
