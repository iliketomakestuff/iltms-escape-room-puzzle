/*
 * Written by Bob Clagett - I Like To Make Stuff
 * To go with the project at https://www.iliketomakestuff.com/make-escape-room-puzzle/
*/
/*************************************************** 
  This is a library for our I2C LED Backpacks

  Designed specifically to work with the Adafruit LED 7-Segment backpacks 
  ----> http://www.adafruit.com/products/881
  ----> http://www.adafruit.com/products/880
  ----> http://www.adafruit.com/products/879
  ----> http://www.adafruit.com/products/878

  These displays use I2C to communicate, 2 pins are required to 
  interface. There are multiple selectable I2C addresses. For backpacks
  with 2 Address Select pins: 0x70, 0x71, 0x72 or 0x73. For backpacks
  with 3 Address Select pins: 0x70 thru 0x77

  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include <Wire.h> // Enable this line if using Arduino Uno, Mega, etc.
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

Adafruit_7segment matrix = Adafruit_7segment();

bool clockRunning=false;
bool solved=false;
const int pinSwitch1 = 7; //Pin Reed
const int pinSwitch2 = 8; //Pin Reed
const int pinSwitch3 = 9; //Pin Reed
const int pinSwitch4 = 12; //Pin Reed
int switch1 = 0;
int switch2 = 0;
int switch3 = 0;
int switch4 = 0;
const int lockCode = 201;
uint16_t counter = 5000;
int countdownSpeed = 10;
void setup() {
#ifndef __AVR_ATtiny85__
  Serial.begin(9600);
  Serial.println("7 Segment Backpack Test");
#endif

  pinMode(pinSwitch1, INPUT);
  pinMode(pinSwitch2, INPUT);
  pinMode(pinSwitch3, INPUT);
  pinMode(pinSwitch4, INPUT);

  matrix.begin(0x70);
      // print a hex number
  
          matrix.print(0x3333,HEX);
          matrix.writeDisplay();
          delay(100);
          
          matrix.print(0x2222,HEX);
          matrix.writeDisplay();
          delay(100);
    
          matrix.print(0x1111,HEX);
          matrix.writeDisplay();
          delay(100);
 
    
    matrix.clear();
    matrix.writeDisplay();
    delay(500);
    Serial.println("READY!"); 
}

void loop() {
  if(solved == false){
    switch1 = digitalRead(pinSwitch1);
    switch2 = digitalRead(pinSwitch2); 
    switch3 = digitalRead(pinSwitch3);
    switch4 = digitalRead(pinSwitch4); 

    if (switch1 == HIGH && clockRunning==false)
    {
      Serial.println("BEGIN!"); 
      clockRunning = true;
    }
       
     if(clockRunning==true){
        matrix.println(counter);
        matrix.writeDigitRaw(2,0x02);
        matrix.writeDisplay();
        delay(countdownSpeed);
                
        if(counter == 0){
          matrix.print(0xDEAD, HEX);
          matrix.writeDisplay();
          Serial.println("BOOM!"); 
          solved = true; 
        } else {
          counter--;
        }
      }
    
      if(switch1 == HIGH && switch2 == HIGH && switch3 == HIGH && switch4 == HIGH && clockRunning==true){
        //SOLVED!
        solved=1;
        Serial.println("solved");
        clockRunning = false;
        for (uint16_t counter = 4; counter > 0; counter--) {
          matrix.print(0x0000,HEX);
          matrix.writeDisplay();
          delay(750);
          matrix.clear();
          matrix.writeDisplay();
          delay(750);
        }
        

        matrix.println(lockCode);
        matrix.writeDisplay();
        
      }
  }
 
}
