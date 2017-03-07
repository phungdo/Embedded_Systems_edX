// This is your second program to run on the LaunchPad
// You will debug this program as your Lab 4
// If both switches SW1 and SW2 are pressed, the LED should be blue
// If just SW1 switch is pressed,            the LED should be red
// If just SW2 switch is pressed,            the LED should be green
// If neither SW1 or SW2 is pressed,         the LED should be off

// 0.Documentation Section 
// main.c
// Runs on LM4F120 or TM4C123
// Lab4_IO, Inputs from PF4,PF0, output to PF3,PF2,PF1 (LED)
// Authors: Daniel Valvano, Jonathan Valvano and Ramesh Yerraballi
// Date: January 15, 2016

// LaunchPad built-in hardware
// SW1 left switch is negative logic PF4 on the Launchpad
// SW2 right switch is negative logic PF0 on the Launchpad
// red LED connected to PF1 on the Launchpad
// blue LED connected to PF2 on the Launchpad
// green LED connected to PF3 on the Launchpad

// 1. Pre-processor Directives Section
// Constant declarations to access port registers using 
// symbolic names instead of addresses
// TExaS.o is the object code for the real-board grader
#include "TExaS.h"
// General-Purpose Input/Outputs (GPIOs) 26/1396 tm4c123gh6pm.pdf

// This is a mapping between the name of register and its address in the memory map
#define GPIO_PORTF_DATA_R       (*((volatile unsigned long *)0x400253FC)) //Data	
#define GPIO_PORTF_DIR_R        (*((volatile unsigned long *)0x40025400)) //Direction	
#define GPIO_PORTF_AFSEL_R      (*((volatile unsigned long *)0x40025420))	//Alternate Function Set
#define GPIO_PORTF_PUR_R        (*((volatile unsigned long *)0x40025510))	//Pull-Up Select
#define GPIO_PORTF_DEN_R        (*((volatile unsigned long *)0x4002551C))	//Digtal Enable
#define GPIO_PORTF_LOCK_R       (*((volatile unsigned long *)0x40025520))	//Lock
#define GPIO_PORTF_CR_R         (*((volatile unsigned long *)0x40025524))	//Commit
#define GPIO_PORTF_AMSEL_R      (*((volatile unsigned long *)0x40025528))	//Analog Mode Select
#define GPIO_PORTF_PCTL_R       (*((volatile unsigned long *)0x4002552C)) //Port Control

// All I/O device have a clock. Here we are going to turn on the clock.
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108)) //Run Mode Clock Gating Control Register 2

// 2. Declarations Section
//   Global Variables
unsigned long SW1,SW2;  // input from PF4,PF0
unsigned long Out;      // outputs to PF3,PF2,PF1 (multicolor LED)

//   Function Prototypes
void PortF_Init(void);
void Delay(void);
void EnableInterrupts(void);


// 3. Subroutines Section
// MAIN: Mandatory for a C Program to be executable
/*
int main(void)
{    
  TExaS_Init(SW_PIN_PF40,LED_PIN_PF321); 
  // TExaS_Init initializes the real board grader for lab 4
  PortF_Init();        // Call initialization of port PF4, PF3, PF2, PF1, PF0    
  EnableInterrupts();  // The grader uses interrupts
  while(1)
		{
			SW1 = !(GPIO_PORTF_DATA_R&0x10);     // read PF4 into SW1
			SW2 = !(GPIO_PORTF_DATA_R&0x01);     // read PF0 into SW2
			if(SW1&&SW2)
				{                     // both pressed
					GPIO_PORTF_DATA_R = 0x04;       // LED is blue
				} 
			else
				{                           
					if(SW1&&(!SW2))
						{                // just SW1 pressed
							GPIO_PORTF_DATA_R = 0x02;     // LED is red
						} 
					else
						{                        
							if((!SW1)&&SW2)
								{              // just SW2 pressed
									GPIO_PORTF_DATA_R = 0x08;   // LED is green
								}
							else
								{                        // neither switch
									GPIO_PORTF_DATA_R = 0x00;   // LED is off
								}
						}
				}
		}
}
*/

int main(void){    
  TExaS_Init(SW_PIN_PF40,LED_PIN_PF321); 
																		 // TExaS_Init initializes the real board grader for lab 4
  PortF_Init();        								// Call initialization of port PF4, PF3, PF2, PF1, PF0    
  EnableInterrupts();  								// The grader uses interrupts
  while(1){
    SW1 = GPIO_PORTF_DATA_R&0x10;     // read PF4 into SW1
    SW2 = GPIO_PORTF_DATA_R&0x01;     // read PF0 into SW2
																			// SW1 and SW2 are negative logic
    if(SW1&&SW2)						
			{                     					// neither switch
      GPIO_PORTF_DATA_R = 0x00;       // no LED. 0001 0000 & 0000 0001 = 0000 0000
			} 
		else
		{                           
     if(SW1&&(!SW2))
			 {                							// just SW2 pressed
        GPIO_PORTF_DATA_R = 0x08;     // LED is green. 0000 1000. enable PF3. 2^3
       } else{                        
        if((!SW1)&&SW2){              // just SW1 pressed
          GPIO_PORTF_DATA_R = 0x02;   // LED is red. 0000 0010. enable PF3. 2^1
        }else{                        // both switch
          GPIO_PORTF_DATA_R = 0x04;   // LED is blue. 0000 0100. enable  PF2. 2^2
        }
      }
    }
  }
}

// Subroutine to initialize port F pins for input and output
// PF4 and PF0 are input SW1 and SW2 respectively
// PF3,PF2,PF1 are outputs to the LED
// Inputs: None
// Outputs: None
// Notes: These five pins are connected to hardware on the LaunchPad
void PortF_Init(void){ volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x00000020;     // 1) F clock - tm4c123gh6pm page 338 
																		// |0x20 = 0010 0000 (set bit 5th = 1) Enable and provide a clock to GPIO Port F in Run mode. 
  delay = SYSCTL_RCGC2_R;           // delay   
  GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock PortF PF0 - tm4c123gh6pm page 681
																		// unlocks the GPIO Commit (GPIOCR) register for write access
	
  GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0 - tm4c123gh6pm page 682 683  
																		// 0x1F = 0001 1111. bit 4 -> bit 0. 
																		// 1-The corresponding GPIOAFSEL, GPIOPUR, GPIOPDR, or GPIODEN bits can be written.
  
	GPIO_PORTF_AMSEL_R = 0x00;        // 3) disable analog function - tm4c123gh6pm 684
																		// If any pin is to be used as an ADC input, the appropriate bit in GPIOAMSEL 
																		// must be set to disable the analog isolation circuit.
  
	GPIO_PORTF_PCTL_R = 0x00000000;   // 4) GPIO clear bit PCTL  - page 685
																		// The reset value for this register is 0x0000.0000 for GPIO ports that are not PA, PB, PC in tables.
  
	GPIO_PORTF_DIR_R = 0x0E;          // 5) PF4,PF0 input, PF3,PF2,PF1 output - page 660
																		// 0x0E = 0 1110. 0 is output pins, 1 is input pins.
  
	GPIO_PORTF_AFSEL_R = 0x00;        // 6) no alternate function - tm4c123gh6pm page 668
																		// If a bit is clear, the pin is used as a GPIO and is controlled by the GPIO registers.
  
	GPIO_PORTF_PUR_R = 0x11;          // enable pullup resistors on PF4,PF0 - page 675
																		// 0x11 = 0001 0001. bit 4 and bit 0. 1-The corresponding pin's weak pull-up resistor is enabled
  
	GPIO_PORTF_DEN_R = 0x1F;          // 7) enable digital pins PF4-PF0 - page 680
																		// 0x1F = 0001 1111. bit 4 -> bit 0. 1-The digital functions for the corresponding pin are enabled.
}
// Color    LED(s) PortF
// dark     ---    0
// red      R--    0x02
// blue     --B    0x04
// green    -G-    0x08
// yellow   RG-    0x0A
// sky blue -GB    0x0C
// white    RGB    0x0E
// pink     R-B    0x06


