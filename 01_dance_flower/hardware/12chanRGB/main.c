/*---------------------------------------------------------------------------------------------

  12 channel RGB PWM Module
  Atmega88 @ 8 MHZ
 
   Pronatura Yverdon Champs-Pittet
   Exposition 2015: Prairies Fleuries
   
  
  © 2015 Michael Egger me@anyma.ch
  GNU GPL v 2.0
  
--------------------------------------------------------------------------------------------- */

// AVR Libc (see http://www.nongnu.org/avr-libc/)
#include <avr/io.h>			// include I/O definitions (port names, pin names, etc)
#include <avr/wdt.h>		// include watchdog timer support
#include <avr/interrupt.h>	// include interrupt support


volatile unsigned char dimmer_values[12];
volatile unsigned char PdmCompare;
unsigned char rx_idx;
unsigned char dimmer_addr;
unsigned char last_spdr;


//---------------------------------------------------------------------------------------------  //
//																	Interrupt PDM
//---------------------------------------------------------------------------------------------  //


ISR(TIMER1_COMPA_vect) {
	unsigned char c_state = 0; 
	unsigned char d_state = 0;
	
	// adapt interval to next interrupt according to the target value,
	// to get finer steps for dark shades
	unsigned char tmp = PdmCompare & 0x7F;
	if (PdmCompare < 128) {
		OCR1A = 200 * (tmp / 16 + 1);
	}
	else {
		OCR1A = 160 * (16 - tmp/16);
	}
	
	if (dimmer_values[0] >= PdmCompare) c_state|= (1<<0);        //compare channels 
	if (dimmer_values[1] >= PdmCompare) c_state|= (1<<1); 
	if (dimmer_values[2] >= PdmCompare) c_state|= (1<<2); 
	if (dimmer_values[3] >= PdmCompare) c_state|= (1<<3); 
	if (dimmer_values[4] >= PdmCompare) c_state|= (1<<4); 
	if (dimmer_values[5] >= PdmCompare) c_state|= (1<<5); 
	PORTC= c_state; 

	if (dimmer_values[6] >= PdmCompare) d_state|= (1<<2); 
	if (dimmer_values[7] >= PdmCompare) d_state|= (1<<3); 
	if (dimmer_values[8] >= PdmCompare) d_state|=  (1<<4);       
	if (dimmer_values[9] >= PdmCompare) d_state|=  (1<<5); 
	if (dimmer_values[10] >= PdmCompare) d_state|= (1<<6); 
	if (dimmer_values[11] >= PdmCompare) d_state|= (1<<7); 
	PORTD = (PORTD & 0x03) | d_state; 
	 
	 
	if (PdmCompare &(1<<7)) { 
		 PdmCompare += 7;                                       //increment compare register 
		 PdmCompare &= ~(1<<7);                                                //clear MSB 
		 if (PdmCompare == 0) PdmCompare = 0x80; 
	} 
	else PdmCompare |= (1<<7);                                                 //set MSB
		
}

//---------------------------------------------------------------------------------------------  //
//																	SETUP
//---------------------------------------------------------------------------------------------  //

void setup() {        
       
	dimmer_values[0] = 0;
	dimmer_values[1] = 255;
	dimmer_values[2] = 0;
	dimmer_values[3] = 255;
	dimmer_values[4] = 0;
	dimmer_values[5] = 0;
	dimmer_values[6] = 0;
	dimmer_values[7] = 0;
	dimmer_values[8] = 255;
	dimmer_values[9] = 255;
	dimmer_values[10] = 255;
	dimmer_values[11] = 0;


    DDRC = 0xFF;
	DDRD = 0xFC;

	UBRR0 = 51;  //9600 BAud @ 8Mhz
	UCSR0B = (1<<RXEN0);
    UCSR0C = (1<<USBS0)|(3<<UCSZ00); /* Set frame format: 8data, 2stop bit */

	// set up timer
	TCCR1A =  0;
	TCCR1B = (1 << WGM12) | (1 << CS10); // no prescaler, CTC mode;
	OCR1A = 180;
	TIMSK1 =  0; // disable all timer interrupts
	TIMSK1 |= (1 << OCIE1A); // output compare interrupt
	TCNT1 = 0;

	// globally enable interrupts
	sei();

}


unsigned char USART_Receive( void ) {
/* Wait for data to be received */ while ( !(UCSR0A & (1<<RXC0)) )
;
/* Get and return received data from buffer */ return UDR0;
}

//=============================================================================================  //
//																	Main Loop
//---------------------------------------------------------------------------------------------  //

void loop() {

	unsigned char temp;
	
	if (rx_idx == 1) dimmer_values[dimmer_addr] = USART_Receive();
	else dimmer_addr =  USART_Receive();;
	rx_idx++;
	rx_idx%=2;
	
	/*temp = SPDR;
	switch (rx_idx) {
		case  1:
			dimmer_addr = temp % 12;
			rx_idx++;
			break;
		
		case 2:
			dimmer_values[dimmer_addr] = temp;
			rx_idx++;
			break;
			
			
		default:
			if (last_spdr == 0xff) {
				if (temp == 0x0f) {
					rx_idx = 1;
				}
			}
			break;
	}
	last_spdr = temp;*/
}


int main(void) {
	setup();
	while(1) loop();
	return 0;
}