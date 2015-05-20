#include "cubo_msp430.h"      /* Cube main globals */
#include "draw.h"          /* Lowlevel draw routines */

/**************/
/**** Cube ****/
/**************/
/* Fill the cube with a given pattern */
void cube_fill (char pattern, 
                volatile char (*buffer)[CUBE_ROWS]){
	int row;
	int layer;
	/* Fills all the layers and rows in the given buffer */
	for (layer = LAYER_1; layer < ENDOF_LAYERS; layer++)
		for (row = ROW_1; row < ENDOF_ROWS; row++)
			buffer[layer][row] = pattern;
}

/* Copy complete cube from source to target buffer */
void cube_copy (volatile char (*source_buffer)[CUBE_ROWS],
                volatile char (*target_buffer)[CUBE_ROWS]){
	int row;
	int layer;

	/* Copy all layers and rows from source to target buffer */
	for (layer = LAYER_1; layer < ENDOF_LAYERS; layer++)
		for (row = ROW_1; row < ENDOF_ROWS; row++)
			target_buffer[layer][row] = source_buffer[layer][row];
}



/**************************************************************/
/**** Plane                                                ****/
/**************************************************************/
/* Set a plane state in a specified buffer                    */
void plane_set (int plane,
                int axis,
                int state,
                volatile char (*buffer)[CUBE_ROWS]){
	int layer;
	int row;
	char temp_char;

	/* Invoke the X_AXIS */
	if (axis == X_AXIS){
		/* Since coordinates along the X axis are
		 * the actual bits in each row of each layer
		 * We loop them all, and set the apropriate bit.
		 */
		for (layer = LAYER_1; layer < ENDOF_LAYERS; layer++){
			/*
			 * Set the bit, and shift it inwards to the correct plane
			 * According to the given state
			 */
			if (state == ON)
				for (row = ROW_1; row < ENDOF_ROWS; row++)
					buffer[layer][row] |= (0x01 << plane);

			if (state == OFF)
				for (row = ROW_1; row < ENDOF_ROWS; row++)
					buffer[layer][row] &= ~(0x01 << plane);

			if (state == TOGGLE)
				for (row = ROW_1; row < ENDOF_ROWS; row++)
					buffer[layer][row] ^= (0x01 << plane);
		}
	}

	/* Invoke the Y_AXIS */
	if (axis == Y_AXIS){
		if (state == ON)
			for (layer = LAYER_1; layer < ENDOF_LAYERS; layer++)
				buffer[layer][plane] |= ROW_MASK;

		if (state == OFF)
			for (layer = LAYER_1; layer < ENDOF_LAYERS; layer++)
				buffer[layer][plane] &= ~(ROW_MASK);

		if (state == TOGGLE)
			for (layer = LAYER_1; layer < ENDOF_LAYERS; layer++)
				buffer[layer][plane] ^= ROW_MASK;
	}

	/* Invoke the Z_AXIS */
	if (axis == Z_AXIS){
		if (state == ON)
			for (row = ROW_1; row < ENDOF_ROWS; row++)
				buffer[plane][row] |= ROW_MASK;

		if (state == OFF)
			for (row = ROW_1; row < ENDOF_ROWS; row++)
				buffer[plane][row] &= ~(ROW_MASK);

		if (state == TOGGLE)
			for (row = ROW_1; row < ENDOF_ROWS; row++){
				temp_char = buffer[plane][row];
				buffer[plane][row] = ~(temp_char);
			}	
	}
}

/*************/
/**** Row ****/
/*************/


void print_char (int plane, 
                 char axis, 
                 int character,
                 const unsigned char (*source_buffer)[CUBE_ROWS],
                 volatile char (*target_buffer)[CUBE_ROWS]){
   int layer;              	
   switch (axis){
      case X_AXIS: 
         for (layer = LAYER_1; layer < ENDOF_LAYERS; layer++){
	        target_buffer[layer][plane] = source_buffer[character][layer];
	     }
	     break;
      
      case Y_AXIS:
         break;
            
      case Z_AXIS:
         for (layer = LAYER_1; layer < ENDOF_LAYERS; layer++){
	        target_buffer[plane][layer] = source_buffer[character][layer];
	     }
	     break;
   }
}



