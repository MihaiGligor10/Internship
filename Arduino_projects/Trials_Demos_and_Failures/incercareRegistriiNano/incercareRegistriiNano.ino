#include <avr/interrupt.h>

void setup() {
  // put your setup code here, to run once:
  /*
  initialization
  */
  Serial.begin(9600);
  TCA0.SINGLE.PER = 0xF424; // write TOP value in period register 
  TCA0.SINGLE.CTRLA = 0b00001101;  // bit 0 to 1 to enable the timer,and bits 1,2,3 to 11 to select the clock frequency
  PORTE.DIR = (1<<2);
  Serial.println(PORTE.DIR);
  Serial.println(TCA0.SINGLE.CTRLA);
    
  //The counter value can be read from the Counter bit field (CNT) in the Counter register (TCAn.CNT).
  //interrupt pg 112
  //PITCTRLA Synchronization Busy 273
  //PITINTCTRL PIT Interrupt Control 274
  //PITINTFLAGS Periodic interrupt Flag 275
  //INTCTRL RTC Interrupt Control 263
  //71
  

}
ISR(TCA0_OVF_vect){

  TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;
  PORTE.OUT ^= (1<<2); //blink
}
 

void loop() {

  //Serial.println(TCA0.SINGLE.CNT);
  
  //delay(50);

}