/*
  DisplayController.h - Library for Controlling Max Luminator FlipDot display sign.
  Created by Antonio Carioca, July 4, 2012.
*/

#ifndef FlipDotDisplayController_h
#define FlipDotDisplayController_h

#include "Arduino.h" 

class DotDisplay{
public:
  DotDisplay(int dataPin, int clockPin, int latchPin);
	byte generateColumnCode (byte col);
	void updateDisplay(char textMessage[]);
	void updateDisplayArray(char textMessage[][16], int pause);
	void setDot(byte col, byte row, bool on);
	void disablePins();
private:
	int _dataPin;
	int _clockPin;
	int _latchPin;
	byte alphabet[][5];
	
};

#endif
