/*---------------------------------------------------------------------------------------------

  Gnusbuino MIDI Library 2012 by Michael Egger
 
   Pronatura Yverdon Champs-Pittet
   Exposition 2015: Prairies Fleuries
   
  
  © 2015 Michael Egger me@anyma.ch
  GNU GPL v 2.0
  
--------------------------------------------------------------------------------------------- */


#include "GnusbuinoMIDI.h"  
#include "SPI.h"

MIDIMessage message;        
const int slaveSelectPin = 4;

volatile unsigned char PdmCompare;

unsigned char d[12];

unsigned char sensorValue = 0;         // variable to store the value coming from the sensor
unsigned char sentValue = 255;         // we only want to send data when there's something changing
                             // so we have to keep track of the last value that was sent to the host


//---------------------------------------------------------------------------------------------  //
//																	SETUP
//---------------------------------------------------------------------------------------------  //

void setup() {               
 // set the slaveSelectPin as an output:
  pinMode (slaveSelectPin, OUTPUT);
  // initialize SPI:
   Serial.begin(9600);

    
}


//=============================================================================================  //
//																	Main Loop
//---------------------------------------------------------------------------------------------  //

void loop() {
  unsigned char i;
  
   sensorValue = analogRead(0) / 8;                       // analogRead returns 0-1023, we need 0-127
  if (sensorValue != sentValue) {                         // compare actual readout to last sent value    
       
      //MIDI.write(MIDI_CONTROLCHANGE, controller number , controller value )

        MIDI.write(MIDI_CONTROLCHANGE,1,sensorValue);     // put new control change message into MIDI sending queue
        sentValue = sensorValue;                          // store last sent value
  }

  
  
   if (MIDI.read(&message)) {                            // see if we have received a new MIDI message and store it in our variable
      
                                                         // don't forget the ampersand (&) before the variable name !
        if(message.command == MIDI_CONTROLCHANGE) {
    /*  
          digitalWrite(slaveSelectPin,LOW);
            SPI.transfer(0xFF);
            SPI.transfer(0x0F);
            SPI.transfer(message.key);
            SPI.transfer( 2 * message.value);
            // take the SS pin high to de-select the chip:
            digitalWrite(slaveSelectPin,HIGH); 
                    */
                  //  SPI.transfer(message.key);
              Serial.write(message.key);
              Serial.write(message.value*2);
                  //  SPI.transfer(message.value);
           // d[message.key%12] = message.value;

        }

        
    }    
  
  
 // SPI.transfer(d[i]);
 // i++;
 // i%=12;
    delay(1);              // give some time for sending, otherwhise the MIDI queue could fill up
}


