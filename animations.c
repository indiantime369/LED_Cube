#include "cubo_msp430.h"
#include "draw.h"
#include "graphics.h"
#include "animations.h"

/* Display characterset on first plane */
void display_setText(int iterations, int tiempo){
	int character;
	int i, plane;
	
	for (i = 0; i < 5; i++){   
             //cube_clear();  	
             for (plane = PLANE_1; plane < 8; plane++){
                  cube_clear();  	
 		  print_char(plane, X_AXIS, i, testText , cube_display_buffer); 
		  delay_ms(tiempo/4);
             }   
			
	}
	
}	



