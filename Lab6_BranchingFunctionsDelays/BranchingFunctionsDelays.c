// BranchingFunctionsDelays.c Lab 6
// Runs on LM4F120/TM4C123
// Use simple programming structures in C to 
// toggle an LED while a button is pressed and 
// turn the LED on when the button is released.  
// This lab will use the hardware already built into the LaunchPad.
// Daniel Valvano, Jonathan Valvano
// January 15, 2016

// built-in connection: PF0 connected to negative logic momentary switch, SW2
// built-in connection: PF1 connected to red LED
// built-in connection: PF2 connected to blue LED
// built-in connection: PF3 connected to green LED
// built-in connection: PF4 connected to negative logic momentary switch, SW1

#include "TExaS.h"    // lab grader functions

// Constant declarations to access port registers
// using symbolic names instead of addresses
#define GPIO_PORTF_DATA_R       (*((volatile unsigned long *)0x400253FC))
#define GPIO_PORTF_DIR_R        (*((volatile unsigned long *)0x40025400))
#define GPIO_PORTF_AFSEL_R      (*((volatile unsigned long *)0x40025420))
#define GPIO_PORTF_PUR_R        (*((volatile unsigned long *)0x40025510))
#define GPIO_PORTF_DEN_R        (*((volatile unsigned long *)0x4002551C))
#define GPIO_PORTF_LOCK_R       (*((volatile unsigned long *)0x40025520))
#define GPIO_PORTF_CR_R         (*((volatile unsigned long *)0x40025524))
#define GPIO_PORTF_AMSEL_R      (*((volatile unsigned long *)0x40025528))
#define GPIO_PORTF_PCTL_R       (*((volatile unsigned long *)0x4002552C))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
#define SYSCTL_RCGC2_GPIOF      0x00000020  // port F Clock Gating Control

//**********2. Declarations Section**********
// Global Variables
unsigned long SW1;    // input from PF4
// Function Prototypes
void DisableInterrupts(void);           // disable interrupts
void EnableInterrupts(void);            // enable interrupts
void delay100ms(unsigned long time);    // delay function
void PortF_Init(void);                  // port F initial function

//**********3. Subroutines Section**********
//----------PortF_Init----------
// Initializes port F pins for input and output
// Inputs: None
// Outputs: None
void PortF_Init(void) {
    volatile unsigned long delay;
    SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF;     // 1) F clock
    delay = SYSCTL_RCGC2_R;           // delay
    GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock PortF
    GPIO_PORTF_CR_R |= 0x14;  // allow changes to PF4 (SW1) and PF2 (Blue LED)
    GPIO_PORTF_AMSEL_R = 0x00;        // 3) disable analog function
    GPIO_PORTF_PCTL_R = 0x00;         // 4) GPIO clear bit PCTL
    GPIO_PORTF_DIR_R |= 0x0E;      // 5) PF4 (SW1) is input,
                                      // PF2 (Blue LED) is output
    GPIO_PORTF_AFSEL_R = 0x00;        // 6) no alternate function
    GPIO_PORTF_PUR_R |= 0x10;         // enable pullup resistor on PF4
    GPIO_PORTF_DEN_R |= 0x1E;         // 7) enable digital pins PF4, PF2
}

//----------delay100ms----------
// Delays some hundreds of milliseconds
// Inputs: time    How many hundreds of milliseconds to delay
// Outputs: None
// Assumes: 80-MHz clock
void delay100ms(unsigned long time) {
    unsigned long i;
    while (time) {
        //i = 1333333;    // this number means 100ms
					i = 2333333;
			while (i) {
            i--;
        }
        time--;  // decrements every 100 ms
    }
}

//**********4. Main function**********
int main(void) {
    // setup:
    // activate grader and set system clock to 80 MHz
    TExaS_Init(SW_PIN_PF4, LED_PIN_PF2);
    PortF_Init();         // port F initialization
    EnableInterrupts();   // enable interrupts for the grader

    // loop:
    while (1) {
       /* SW1 = GPIO_PORTF_DATA_R & 0x10;   // read bit PF4 into SW1
        if (!SW1) {                       // SW1 is pressed
            delay100ms(1);                // set delay of 0.1 second
            GPIO_PORTF_DATA_R ^= 0x0E;    // 
																					// 
        }
        else {                            // SW1 is not pressed
            GPIO_PORTF_DATA_R = 0x04;     // turn and keep on the blue LED
        } */
			delay100ms(1);
		if(GPIO_PORTF_DATA_R&0x10) GPIO_PORTF_DATA_R = 0x04;
			// Initially, GPIO_DATA_1st = 0x04
			// Then we hold the switch, GPIO_DATA_2nd = (GPIO_DATA_1st) XOR 0x0E
		if(!(GPIO_PORTF_DATA_R&0x10)) GPIO_PORTF_DATA_R = GPIO_PORTF_DATA_R ^ 0x0E;
    }
}