/*
 * Blinking LED.c
 *
 * Created: 10/7/2019 10:54:05 AM
 * Author: Alireza
 */

//set your clock speed
#define F_CPU 4000000UL
//these are the include files. They are outside the project folder
#include <io.h>
#include <delay.h>
//this include is in your project folder
//#include “myTimer.h”

void main (void)
{
    //Set PORTC to all outputs
    DDRC = 0xFF;
    DDRA = 0x00; 
        
    //create an infinite loop
    while(1) {
        //this turns pin C0 on and off
        //turns C0 HIGH
        PORTC |= (1 << 0);
        //PAUSE 250 miliseconds
        delay_ms(50 + 50 * PINA);
        //turns C0 LOW
        PORTC &= ~(1 << 0);
        //PAUSE 250 miliseconds
        delay_ms(50 + 50*PINA);
    };
}