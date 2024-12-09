#include <avr/interrupt.h>

//******************************************************************************
ISR(TCB2_INT_vect) {
  TCB2.INTFLAGS = TCB_CAPT_bm; 
  PORTB.OUT ^= (1 << 2);        //blink the LED
  Serial.print(".");           
}
//******************************************************************************
ISR(TCB1_INT_vect) {
  TCB1.INTFLAGS = TCB_CAPT_bm; 
  PORTA.OUT ^= (1 << 1);        //blink the LED
  Serial.print(".");           
}
//******************************************************************************
void setup_timer_B2() {
  //0xF424 62500 pentru 1 s//
  //0x7A12 31250 pentru 0,5 s//
  //0x186A 6250 pentru 0,1 s//
  //0x00FA 250 pentru 0,001 s//
  TCB2.CCMP = 0xF424;  
  TCB2.CTRLB = 0x00;  // interrupt mode
  TCB2.CTRLA |= 0b0000101;  //enable timer, assign TCA0 clock
  TCB2.INTCTRL = TCB_CAPT_bm;
}

void setup_timer_B1()
{
  //0xF424 62500 pentru 1 s//
  //0x7A12 31250 pentru 0,5 s//
  //0x186A 6250 pentru 0,1 s//
  //0x00FA 250 pentru 0,001 s//
  TCB1.CCMP = 0xF424;
  TCB1.CTRLB = 0x00;  // interrupt mode
  TCB1.CTRLA |= 0b0000101;  //enable timer, assign TCA0 clock
  TCB1.INTCTRL = TCB_CAPT_bm;
}

//******************************************************************************
//******************************************************************************
void setup() {
  //reset the registers
   TCB2.CCMP = 0x0000;  
   TCB2.CTRLB = 0x00;
   TCB2.CTRLA = 0x00;
   TCA0.SINGLE.CTRLA = 0x00;
   /////////////////////
  Serial.begin(9600);
  TCA0.SINGLE.CTRLA |= 0b00001101;
  PORTB.DIR |= (1 << 2); //set port E pin 2 , or pin 13, to output
  PORTA.DIR |= (1 << 1);
  PORTA.OUT |= (1 << 1);
  setup_timer_B2();
  setup_timer_B1();
  

}

void loop() {
  
}
