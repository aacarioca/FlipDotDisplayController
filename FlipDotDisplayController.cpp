/*
  DisplayController.h - Library for Controlling Max Luminator FlipDot display sign.
  Created by Antonio Carioca, July 4, 2012.
*/

#include "Arduino.h"
#include "FlipDotDisplayController.h"

DotDisplay::DotDisplay(int dataPin, int clockPin, int latchPin){

  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  
  _dataPin = dataPin;
  _clockPin = clockPin;
  _latchPin = latchPin;
}

/*
  The 75 columns are addressed by a combination of 3 8-bit multiplexers,
  a selector that choses between the different multiplexers and another
   selector that points to one of the 3, 25-column sub panels.
  Each multiplexer can address 8 columns, which gives a total
  of 24 directly accessible columns. The 25th column in each subpanel 
  is acessed through a special case in the logic.
  
  Parameters:
  
  col: takes a byte from 0 to 74, representing the absolute column to 
  be accessed.
  
  Returns a byte containing the panel colum address as such:
  
  First 3 bits: which column the chosen multiplexer will address (from 0 to 7)
  Next 2 bits: which multiplexer to enable (from 0 to 2 for the multiplexers, 
  3 for the special case)
  Last 2 bits: which subpanel to enable (from 1 to 3)
  
*/
byte DotDisplay::generateColumnCode (byte col){  
  byte subpanel = 0;
  byte relcol = 0;
  byte subgroup = 0;
  
  subpanel = (col/25); //from 0 to 2
  relcol = (col % 25)+1; // 1 to 25
  
	if(relcol == 25){
	 //special case, the extra column in each panel the
	 // 3 8-bit multiplexes can't access directly
	 //can't address directly
		subgroup=3;
		relcol = 7;
	} else { 
	 //dealing with columns from 1 to 24
		 subgroup = ((relcol-1) / 8); //from 0 to 2
		 relcol = (relcol-1) % 8; //from 0 to 7
	}
  
  relcol = relcol << 5;
  subgroup = subgroup << 3;
  subpanel = subpanel << 1;
  byte result = relcol | subgroup | subpanel;
	
  return result;
}

void DotDisplay::setDot(byte col, byte row, bool on){
	  byte dotFlag = on?1:2;
	  byte secondbyte = (dotFlag << 3) | (row << 5);
	  byte firstbyte = generateColumnCode(col);
	  
	  digitalWrite(_latchPin, LOW); 
	  shiftOut(_dataPin, _clockPin, MSBFIRST, firstbyte); 
	  shiftOut(_dataPin, _clockPin, MSBFIRST, secondbyte);  
	  digitalWrite(_latchPin, HIGH);
	  delay(3);
}

//void DotDisplay::updateDisplay(char *textMessage){
void DotDisplay::updateDisplay(char textMessage[]){


	/* CONSTANTS */
	int DISPLAY_SIZE = 15;

	//=== F O N T ===
	// Font code taken from
	// http://www.instructables.com/files/orig/FQC/A1CY/H5EW79JK/FQCA1CYH5EW79JK.txt
	// The @ will display as space character.
	byte alphabet[][5] = {
	  {0,0,0,0,0},
	  {0,56,125,56,0},// !
	  {80,96,0,80,96},//"
	  {20,127,20,127,20},//#
	  {18,42,127,42,36},//$
	  {98,100,8,19,35},//%
	  {54,73,85,34,5},//&
	  {0,80,96,0,0},//'(7)
	  {0,28,34,65,0},//(
	  {0,65,34,28,0},//)
	  {20,8,62,8,20},//*
	  {8,8,62,8,8},//+
	  {0,5,6,0,0},//,(12)
	  {8,8,8,8,8},//-
	  {0,3,3,0,0},//.(14)
	  {2,4,8,16,32},// /
	  {62,69,73,81,62},//0
	  {17,33,127,1,1},//1
	  {33,67,69,73,49},//2
	  {34,65,73,73,54},//3
	  {12,20,36,127,4},//4
	  {114,81,81,81,78},//5
	  {62,73,73,73,38},//6
	  {64,71,72,80,96},//7
	  {54,73,73,73,54},//8
	  {50,73,73,73,62},//9
	  {0,54,54,0,0},//:(26)
	  {0,53,54,0,0},//;(27)
	  {8,20,34,65,0},//<
	  {20,20,20,20,20},//=
	  {0,65,34,20,8},//>
	  {32,64,69,72,48},//?
	  {62,73,87,85,62},//@
	  {31, 36, 68, 36, 31},    //A
	  {127, 73, 73, 73, 54},   //B
	  {62, 65, 65, 65, 34},    //C
	  {127, 65, 65, 34, 28},   //D
	  {127, 73, 73, 65, 65},   //E
	  {127, 72, 72, 72, 64},   //F
	  {62, 65, 65, 69, 38},    //G
	  {127, 8, 8, 8, 127},     //H
	  {0, 65, 127, 65, 0},     //I
	  {2, 1, 1, 1, 126},       //J
	  {127, 8, 20, 34, 65},    //K
	  {127, 1, 1, 1, 1},       //L
	  {127, 32, 16, 32, 127},  //M
	  {127, 32, 16, 8, 127},   //N
	  {62, 65, 65, 65, 62},    //O
	  {127, 72, 72, 72, 48},   //P
	  {62, 65, 69, 66, 61},    //Q
	  {127, 72, 76, 74, 49},	//R
	  {50, 73, 73, 73, 38},		//S
	  {64, 64, 127, 64, 64},	//T
	  {126, 1, 1, 1, 126},		//U
	  {124, 2, 1, 2, 124},		//V
	  {126, 1, 6, 1, 126},		//W
	  {99, 20, 8, 20, 99},		//X
	  {96, 16, 15, 16, 96},		//Y
	  {67, 69, 73, 81, 97},		//Z
	  {0,127,65,65,0},//[
	  {32,16,8,4,2},//
	  {0,65,65,127,0},
	  {16,32,64,32,16},//^
	  {1,1,1,1,1},//_
	  {0,64,32,16,0},//`
	  {2,21,21,14,1},//a
	  {64,126,9,17,14},//b
	  {14,17,17,17,10},//c
	  {14,17,74,127,1},//d
	  {14,21,21,21,9},//e
	  {1,9,63,72,32},//f
	  {9,21,21,21,30},//g
	  {127,8,8,8,7},//h
	  {0,0,46,1,1},//i
	  {2,1,1,1,94},//j
	  {1,127,4,10,17},//k
	  {0,65,127,1,0},//l
	  {31,16,14,16,31},//m
	  {31,8,16,16,15},//n
	  {14,17,17,17,14},//o
	  {127,20,20,20,8},//p
	  {8,20,20,31,1},//q
	  {31,8,16,16,8},//r
	  {9,21,21,21,18},//s
	  {16,16,126,17,18},//t
	  {30,1,1,30,1},//u
	  {28,2,1,2,28},//v
	  {30,1,6,1,30},//w
	  {17,10,4,10,17},//x
	  {16,9,6,8,16},//y
	  {17,19,21,25,17},//z
	  {8,54,65,65,0},//{
	  {0,0,127,0,0},//|(92)
	  {0,65,65,54,8},//}
	  {32,64,32,16,32},//~
	};

	//Serial.print("FROM WITHIN THE FUNCTION(");
	//Serial.print(textMessage);
	//Serial.print(")");
	
    int currentColumn = 0;
	
    //goes through all characters
    for (int ch = 0; ch < (DISPLAY_SIZE);ch++){  
	  //get a character from the message
      int alphabetIndex = textMessage[ch] - ' '; //Subtract '@' so we get a number
	  
	  //Serial.println(alphabetIndex);
	  
      if (alphabetIndex < 0) alphabetIndex=0; 
	  
	  //set all the bits in the next 5 columns on the display
      for(byte col = currentColumn; col<(currentColumn+5); col++){
		  
		byte calculatedColumn = (col)%5;
        for(byte row = 0; row < 7; row++){
		  bool isOn = bitRead(alphabet[alphabetIndex][calculatedColumn],6-row);//this index is needed as we are going from back to front
		  setDot(col, row, isOn);
        }
      }
      currentColumn = currentColumn+5;
      
    }
    
} 



void DotDisplay::disablePins(){
	  byte secondbyte = B00000000;
	  byte firstbyte = B00000000;
	  
	  digitalWrite(_latchPin, LOW); 
	  shiftOut(_dataPin, _clockPin, MSBFIRST, firstbyte); 
	  shiftOut(_dataPin, _clockPin, MSBFIRST, secondbyte);  
	  digitalWrite(_latchPin, HIGH);
}
