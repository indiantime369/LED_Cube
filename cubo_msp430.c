/* Rinku Malhotra 2015 rinku.malhotra@stthomas on 8x8x8 LED Cube Project         */
/*                                                                               */
/* Target = MSP430 Launchpad                                                     */
//*********************************************************************************
//  MSP430F2553 8x8x8 led cube 
//
//  Description: 
//                MSP430G2553
//             -----------------------------
//         /|\|                         P2.2|--> LAYER_1 to Driver  
//          | |                         P2.1|--> LAYER_2 to Driver    
//          --|RST                      P2.0|--> LAYER_3 to Driver
//            |                         P1.5|--> LAYER_4 to Driver
//            |                         P1.4|--> LAYER_5 to Driver  
//            |                         P1.6|--> LAYER_6 to Driver
//            |                         P2.4|--> LAYER_7 to Driver
//            |                         P2.5|--> LAYER_8 to Driver
//           -|XIN                      P1.#|--> Not used
//           -|XOUT                     P1.0|--> LED Drivers ALE    
//            |                         P1.6|--> LED Drivers CLK  
//            |                         P1.7|--> LED Drivers Data
//
//  Built with CCS Version: 4.0
//******************************************************************************
#include  "msp430g2553.h"
#include <math.h>              /* C math library */
#include "draw.h"              /* Lowlevel draw API */
#include "graphics.h"
#include "animations.h"        /* Effects that can be called */
#include "cubo_msp430.h"

#define LAYER1_PIN_HI    P2OUT |=  BIT2;
#define LAYER1_PIN_LO    P2OUT &= ~BIT2;
#define LAYER2_PIN_HI    P2OUT |=  BIT1;
#define LAYER2_PIN_LO    P2OUT &= ~BIT1;
#define LAYER3_PIN_HI    P2OUT |=  BIT0;
#define LAYER3_PIN_LO    P2OUT &= ~BIT0;
#define LAYER4_PIN_HI    P1OUT |=  BIT5;
#define LAYER4_PIN_LO    P1OUT &= ~BIT5;
#define LAYER5_PIN_HI    P1OUT |=  BIT4;
#define LAYER5_PIN_LO    P1OUT &= ~BIT4;
#define LAYER6_PIN_HI    P1OUT |=  BIT3;
#define LAYER6_PIN_LO    P1OUT &= ~BIT3;
#define LAYER7_PIN_HI    P2OUT |=  BIT4;
#define LAYER7_PIN_LO    P2OUT &= ~BIT4;
#define LAYER8_PIN_HI    P2OUT |=  BIT5;
#define LAYER8_PIN_LO    P2OUT &= ~BIT5;

#define CUBE_ALE_LO      P1OUT &= ~BIT0;
#define CUBE_ALE_HI      P1OUT |=  BIT0;
#define CUBE_CLK_LO      P1OUT &= ~BIT6;
#define CUBE_CLK_HI      P1OUT |=  BIT6;
#define CUBE_DATO_LO     P1OUT &= ~BIT7;
#define CUBE_DATO_HI     P1OUT |=  BIT7;

/* -------------------------- Function Prototypes -------------------------- */
/* ------------------------------------------------------------------------- */
void hw_init (void);  /* This initalizes the I/O, interrupt, precaler */

/* -------------------external Function Prototypes ------------------- */

/* ------------------------------- Variables ------------------------------- */
/* ------------------------------------------------------------------------- */

/**** Cube buffer ************************************************************/
/* The 3D image displayed on the cube is buffered in a 2d array 'cube_buffer'.
 * The 1st dimension in this array is the Z axis of the cube.
 * The 2nd dimension of the array is the Y axis.
 * Each byte is a stripe of leds running along the X axis at the given
 * Z and Y coordinates.
  * The "volatile" makes the variables reachable from within the interrupt 
 * functions.
 */
volatile char cube_display_buffer[ENDOF_LAYERS][ENDOF_ROWS];
volatile char cube_buffer[ENDOF_LAYERS][ENDOF_ROWS];
volatile unsigned int current_layer;
volatile unsigned int CCR0_reload;


int main(void){
	int i;
	WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer
	CCR0_reload = 70;
	hw_init();                                // Init timers, I/Os, interrupts
     
	while(1){	
		WDTCTL = WDTPW + WDTHOLD;             // Stop watchdog timer
		launch_effect(i);			
	}
}

void launch_effect (int efect){
   switch (0){		
 
    case 0:	
      display_setText(1,850);
      break;
    
   }   
}							

void hw_init (void){
	//----------------- Set clock source --------------------------------
	BCSCTL1  = CALBC1_16MHZ;                       // Set range
	DCOCTL   = CALDCO_16MHZ;
	BCSCTL2 &= ~(DIVS_0)+ ~(DIVS_1)+ ~(DIVS_2)+ ~(DIVS_3); 

    //-------------------- Set timer -------------------------------------
	TA0CCTL0 = CCIE;                              // CCR0 interrupt enabled
	TA0CCR0  = CCR0_reload;
    TA0CCR1  = 0;
	TA0CTL   = TASSEL_1 + MC_1;                   // SMCLK, contmode
    //-------------------- Set timer B -------------------------------------
	//TA1CCTL = CCIE;                             // CCR0 interrupt enabled
	//TA1CCR  = 1;
	//TA1CTL = TASSEL_2 + MC_2;                   // SMCLK, contmode

	//-------------------- Set pins --------------------------------------    
    P1DIR |=   0xFF;                              // All P1 as output          
    P1OUT &= ~(0xFF);                             // drive 0's 
    P2DIR |=   0xFF;                              // All P1 as output          
    P2OUT &= ~(0xFF);                             // drive 0's 

	/* Start at first layer and enable interrupts */
	current_layer = LAYER_1;

	/* Enable interrupts */
	_EINT();
}


// Timer B0 interrupt service routine Refreshes the Video Zone
#pragma vector=TIMER0_A0_VECTOR 
__interrupt void Timer_A (void){
	_DINT();
	LAYER1_PIN_LO;
	LAYER2_PIN_LO;
	LAYER3_PIN_LO;
	LAYER4_PIN_LO;
	LAYER5_PIN_LO;
	LAYER6_PIN_LO;
	LAYER7_PIN_LO;
	LAYER8_PIN_LO;
	
	int iRow, iBit; 	
    for (iRow = ROW_1; iRow < ENDOF_ROWS; iRow++){     // row under test
		iBit = 0x01;                                   // bit under test
		do{ 
			if (cube_display_buffer[current_layer][iRow] & iBit){CUBE_DATO_HI;
			}else {CUBE_DATO_LO;}
			
			CUBE_CLK_HI;
			CUBE_CLK_LO;		  
			iBit <<= 1;
		}while(iBit < 0x100);         
	}	
    CUBE_ALE_HI;
    CUBE_ALE_LO;
	
	/* Enable the layer where we draw on */
          if (current_layer == LAYER_1){LAYER1_PIN_HI;
	}else if (current_layer == LAYER_2){LAYER2_PIN_HI;
    }else if (current_layer == LAYER_3){LAYER3_PIN_HI;
	}else if (current_layer == LAYER_4){LAYER4_PIN_HI;
    }else if (current_layer == LAYER_5){LAYER5_PIN_HI;
	}else if (current_layer == LAYER_6){LAYER6_PIN_HI;
    }else if (current_layer == LAYER_7){LAYER7_PIN_HI;
	}else if (current_layer == LAYER_8){LAYER8_PIN_HI;}
					
	/* The cube has 8 layers                               */
	/* If we are at LAYER_8, we want to go back to LAYER_1 */
	if (current_layer++ == ENDOF_LAYERS) current_layer = LAYER_1;
 
    CCR0  = CCR0_reload; 
    _EINT();
}

void delay_ms(unsigned int ms ){    
	unsigned int i;    
	for (i = 0; i<= ms; i++)       
	__delay_cycles(500*16); //Built-in function that suspends the execution for 500 cycles
} 
	

