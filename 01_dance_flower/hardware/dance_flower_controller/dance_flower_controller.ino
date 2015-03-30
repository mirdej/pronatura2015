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



unsigned int sensorValue[4];         // variable to store the value coming from the sensor
unsigned int sentValue[4];         // we only want to send data when there's something changing
                             // so we have to keep track of the last value that was sent to the host
   unsigned char i;    

 
//---------------------------------------------------------------------------------------------  //
//																	SETUP
//---------------------------------------------------------------------------------------------  //

void setup() {               
  // initialize SPI:
   Serial.begin(9600);
   
}


//=============================================================================================  //
//																	Main Loop
//---------------------------------------------------------------------------------------------  //

void loop() {
  
  i++;
  i %= 4;
  
  
  //sensorValue[i] = ((sensorValue[i] * 3) + (analogRead(i) / 8)) / 4;                       // analogRead returns 0-1023, we need 0-127
  
   sensorValue[i] = analogRead(i) / 8;                       // analogRead returns 0-1023, we need 0-127
  if (sensorValue[i] != sentValue[i]) {                         // compare actual readout to last sent value    
       
        MIDI.write(MIDI_CONTROLCHANGE,i,sensorValue[i]);     // put new control change message into MIDI sending queue
        sentValue[i] = sensorValue[i];                          // store last sent value
  }

  
  
   if (MIDI.read(&message)) {                            // see if we have received a new MIDI message and store it in our variable
      
                                                         // don't forget the ampersand (&) before the variable name !
        if(message.command == MIDI_CONTROLCHANGE) {
              Serial.write(message.key);
              Serial.write(message.value*2);
        }

        
    }    
 }


