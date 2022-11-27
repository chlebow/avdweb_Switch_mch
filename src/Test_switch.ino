#include <Arduino.h>
//#include <Wire.h>

#include <mchweb_Switch.h>
#include "PCF8574.h"



const byte addres = 0x20;
PCF8574 pcf20(addres);
PCF8574 pcf201(addres);


const byte buttonPCFpin = 4; 
const byte ButtonESP2866pin = 14; 

int i;
 
Switch buttonPCF = Switch(buttonPCFpin); // button to GND, use internal 20K pullup resistor
Switch ButtonESP2866 = Switch(ButtonESP2866pin,  INPUT_PULLUP, LOW, 1); // button to GND, use internal 20K pullup resistor, debounceTime 1ms

 
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
  //buttonGND.process();
  Serial.print("process ");
}*/

buttonPCF.process(pcf20.read(4));

//buttonGND.poll();
  if(buttonPCF.switched()) Serial.print("buttonPCF switched ");
  if(buttonPCF.pushed()) {Serial.print("buttonPCF pushed "); Serial.print(++i); Serial.println(" times");}
  if(buttonPCF.released()) Serial.println("buttonPCF released");
  if(buttonPCF.longPress()) Serial.println("buttonPCF longPress");
  if(buttonPCF.doubleClick()) Serial.println("buttonPCF doubleClick");
  if(buttonPCF.singleClick()) Serial.println("buttonPCF singleClick");

  ButtonESP2866.poll();
  if(ButtonESP2866.switched()) Serial.print("ButtonESP switched ");
  if(ButtonESP2866.pushed()) {Serial.print("ButtonESP pushed "); Serial.print(++i); Serial.println(" times");}
  if(ButtonESP2866.released()) Serial.println("ButtonESP released");
  if(ButtonESP2866.longPress()) Serial.println("ButtonESP longPress");
  if(ButtonESP2866.doubleClick()) Serial.println("ButtonESP doubleClick");
  if(ButtonESP2866.singleClick()) Serial.println("ButtonESP singleClick");
  

}
