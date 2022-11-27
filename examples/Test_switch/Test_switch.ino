#include <Arduino.h>
#include <Wire.h>

#include "mchweb_Switch.h"
//#include "PCF8574.h"



const byte addres = 0x20;
PCF857 pcf20(addres);
PCF857 pcf201(addres);


const byte buttonGNDpin = 4; 
const byte ButtonESPpin = 15; 

int i;
 
Switch buttonGND = Switch(addres, buttonGNDpin); // button to GND, use internal 20K pullup resistor
Switch ButtonESP = Switch(0, ButtonESPpin); // button to GND, use internal 20K pullup resistor

 
void setup() 
{
  Serial.begin(9600);
  pcf20.begin();
  pcf201.begin();  
}
 
void loop() 
{  

/*
if(pcf20.read(4) == 0){
  buttonGND.process();
  Serial.print("process ");
}
*/
buttonGND.process(pcf20.read(4));

//buttonGND.poll();
  if(buttonGND.switched()) Serial.print("buttonGND switched ");
  if(buttonGND.pushed()) {Serial.print("buttonGND pushed "); Serial.print(++i); Serial.println(" times");}
  if(buttonGND.released()) Serial.println("buttonGND released");
    if(buttonGND.longPress()) Serial.println("buttonGND longPress");
  if(buttonGND.doubleClick()) Serial.println("buttonGND doubleClick");
  if(buttonGND.singleClick()) Serial.println("buttonGND singleClick");

  ButtonESP.poll();
  if(ButtonESP.switched()) Serial.print("ButtonESP switched ");
  if(ButtonESP.pushed()) {Serial.print("ButtonESP pushed "); Serial.print(++i); Serial.println(" times");}
  if(ButtonESP.released()) Serial.println("ButtonESP released");
    if(ButtonESP.longPress()) Serial.println("ButtonESP longPress");
  if(ButtonESP.doubleClick()) Serial.println("ButtonESP doubleClick");
  if(ButtonESP.singleClick()) Serial.println("ButtonESP singleClick");
  

}
