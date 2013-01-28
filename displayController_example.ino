/*
  Arduino sketch showing how to use the DisplayController.h - Library for Controlling Max Luminator FlipDot display sign.
  Created by Antonio Carioca, July 4, 2012.
*/

#include <FlipDotDisplayController.h>

//DotDisplay::DotDisplay(int dataPin, int clockPin, int latchPin)
DotDisplay myDotDisplay(11,12,8);

void setup() {
 //Serial.println("Setting up...");
 Serial.begin(9600);
}

void loop() {
  
  //regardless of the size of the word, the panel
  //will only display the first 15 characters,
  //no scrolling has been implemented
  char *word = "It was a bright cold day in April, and the clocks were striking thirteen.";

  //sending a dot at a time to the 
  //display using setDot()
  for(int col = 0; col< 75; col++){
    for(int row = 0; row <7; row ++){
      myDotDisplay.setDot(col,row,0);
      delay(0);
    }
  }
  
  myDotDisplay.updateDisplay("");
  myDotDisplay.updateDisplay(word);

  //making sure no pins on the panel are set to high 
  //once we are done (so as not to send unecessary 
  //current through the components?)
  myDotDisplay.disablePins();
  
  delay(300000);
}
