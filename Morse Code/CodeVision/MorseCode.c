/*
 * Morse Code.c
 *
 * Created: 10/14/2019 12:06:55 AM
 * Author: Alireza
 */

#include <io.h>
#include <stdbool.h>
#include <delay.h>

static unsigned int time_count;
static unsigned int blink;
bool hold = false;

bool isOpening = false;

bool isResetting = false;
//bool inputForReset = false;

char code = 0b00001010;
//char resetInput = 0b00000000;
char input = 0b00000000;



//External Interrupt 0, rising edge
//In charge of accepting input
interrupt [EXT_INT0] void ext_int0_isr (void)
{
     if (isOpening == false)
    {               
        //If Authorized
        if (PORTC.1 == 1)
        {   
            //Change the code
            if(hold == true)
            {  
                code = code<<1;
                code |= 1UL << 0; //Enqueue 1
            }
            else
            {               
                code = code<<1;
                code &= ~(1UL << 0);  //Enqueue 0
            }
        }                       
        else  //if Input isn't for reset
        {   
            //process the input
            if(hold == true)
            {  
                input = input<<1;
                input |= 1UL << 0; //Enqueue 1
            }
            else
            {               
                input = input<<1;
                input &= ~(1UL << 0);  //Enqueue 0
            }
                
            //compare the input with the code
            if(code == input)
            {
                //give outgoting current to the door
                PORTC.0 = 1;
                    
                //mark the door as being opened                                
                isOpening = true;
                    
                //Enable timer/counter0 overflow interrupt
                TIMSK=0x01;
                hold = false;
                time_count = 0;
            }
            else
            {    
                //If code doesn't match, set output to 0
                PORTC.0 = 0;
            }
            //for testing purposes    
            PORTB = input;
        }
        //for testing purposes
        PORTA = code; 
    }
}


//External Interrupt 1 , falling edge
//In charge of checking if code is being reset
interrupt [EXT_INT1] void ext_int1_isr (void) 
{
    //if the door isn't opening
    if (isOpening == false)
    {
        //Enable timer/counter0 overflow interrupt
        TIMSK=0x01;
        hold = false;
        time_count = 0;
        
        if (PORTC.1 == 1)
        {  
            if(isResetting == false)
            {
                //Enable input mode : reset
                isResetting = true;
                input = 0b00000000;
                code = 0b00000000;
            }            
        }     
        else if (isResetting == true)
        {         
            /*if PIND.0 isn't 1, disable reset state*/   
            isResetting = false;
        }
    }
}

interrupt [TIM0_OVF] void timer0_ovf_isr(void)
{         
    // Count overflow times
    ++time_count;
    if ((time_count == 200)&(PORTC.0 == 0))
    {     
        //Reset timer/counter interrupt mask register
        TIMSK=0x00;  
        hold = true;
    }
    
    //after 1200 ticks of being open, close the door
    if ((time_count == 500)&(PORTC.0 == 1))
    {             
        //Reset timer/counter interrupt mask register    
        TIMSK=0x00;        
        isOpening = false;
        //Close the outgoing current
        PORTC.0 = 0;     
        
        //For testing purposes
        input = 0b00000000;
        PORTB = 0b00000000;
    }
}


void main(void)
{   
    blink = 0;
    
    //Set timer/counter Control Register to use clk/8 prescaler
    TCCR0=0x02;
                 
    //Enable Watchdog Timer and set time-out time at ~2.2s
    WDTCR = 0x0f;
    
    //Enable Interrupts 0 & 1     
    //11000000
    GICR=0xc0;
               
    //Set Interrupt 1 to trigger on falling edge
    //Set Interrupt 0 to trigger on rising edge 
    //00001011
    MCUCR=0x0b;
    
    //Enable global interrupt mask
    #asm("sei");      
    
    //Set DDRC.0 as output
    DDRC.0 = 1;
    //Set DDRC.1 as output
    DDRC.1 = 1;
    
    DDRC.7 = 1;   
    
    //Set DDRA as all output
    DDRA = 0xff;
    //Set DDRB as all output    
    DDRB = 0xff;
    
    //Set DDRD.0 as input                            
    DDRD.0 = 0;
    PORTD.0 = 1;     
    
    //Set DDRD.2 as input
    DDRD.2 = 0;
    PORTD.2 = 1;
    //Set DDRD.3 as input
    DDRD.3 = 0;
    PORTD.3 = 1; 
    
    //Set PORTA as to predefined Code
    PORTA = code;   
    
    //Set PORTC.1 to 0
    PORTC.1 = 0;
    
    PORTC.7 = 0;          
    
    
    // External Interrupt(s) initialization
    // INT0: On
    // INT0 Mode: Rising Edge
    // INT1: On
    // INT1 Mode: Falling Edge
    // INT2: Off
    GICR|=(1<<INT1) | (1<<INT0) | (0<<INT2);
    MCUCR=(1<<ISC11) | (0<<ISC10) | (1<<ISC01) | (1<<ISC00);
    MCUCSR=(0<<ISC2);
    GIFR=(1<<INTF1) | (1<<INTF0) | (0<<INTF2);
            
    while (1)
    {
        //Reset Watchdog timer  
        #asm("wdr");
        
        
        if(++blink >= 10000){
            PORTC.7 = ~PORTC.7; 
            blink = 0;
        }
        
        //
        if (PIND.0 == 0){
            PORTC.1 = 1;            
        } else {
            PORTC.1 = 0;
        }
    }
}