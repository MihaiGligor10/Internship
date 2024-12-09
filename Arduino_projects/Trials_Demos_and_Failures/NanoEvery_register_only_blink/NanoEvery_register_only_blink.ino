#include <stdio.h>

void setup() {
  // put your setup code here, to run once:
  /*
  initialization
  */
  Serial.begin(9600);
  TCA0.SINGLE.PER = 0xFFFF; // write TOP value in period register 
  TCA0.SINGLE.CTRLA = 0b00001101;  // bit 0 to 1 to enable the timer,and bits 1,2,3 to 11 to select the clock frequency
  PORTE.DIR = (1<<2);
  Serial.println(PORTE.DIR);
  Serial.println(TCA0.SINGLE.CTRLA);
  //pinMode(13, OUTPUT); 
  //The counter value can be read from the Counter bit field (CNT) in the Counter register (TCAn.CNT).
  

}

void loop() {
  

  //Serial.println(TCA0.SINGLE.CNT);
  PORTE.OUT ^= (1<<2); //blink
  delay(50);
  
  
  

}