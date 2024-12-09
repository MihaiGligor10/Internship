#include <avr/interrupt.h>


typedef struct task {
   unsigned long period;      // Rate at which the task should tick
   unsigned long lastExecutionTime; 
   void (*TickFct)(void);     // Function to call for task's tick
} task;

unsigned char i = 0;

task tasks[2];
const unsigned char tasksNum = 2;
const unsigned long periodBlue   = 1200;
const unsigned long periodRed = 750;

void Blue(void);
void Red(void);

unsigned long count =0;
//******************************************************************************
ISR(TCB2_INT_vect) {
  TCB2.INTFLAGS = TCB_CAPT_bm; 
  count++;
}
//******************************************************************************

//******************************************************************************
void setup_timer_B() {
  //0xF424 62500 pentru 1 s//
  //0x7A12 31250 pentru 0,5 s//
  //0x186A 6250 pentru 0,1 s//
  //0x00FA 250 pentru 0,001 s//
  TCB2.CCMP = 0x003E;  
  TCB2.CTRLB = 0x00;  // interrupt mode
  TCB2.CTRLA |= 0b0000101;  //enable timer, assign TCA0 clock
  TCB2.INTCTRL = TCB_CAPT_bm;
  TCA0.SINGLE.CTRLA |= 0b00001101; //enable peripheral, set clock div
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
  PORTA.DIR = (1 << 1); //set port E pin 2 , or pin 13, to output
  PORTB.DIR = (1 << 2);
  setup_timer_B();


   tasks[i].period      = periodBlue;
   tasks[i].lastExecutionTime = 0;
   tasks[i].TickFct     = &Blue;
   ++i;
   tasks[i].period      = periodRed;
   tasks[i].lastExecutionTime = 0;
   tasks[i].TickFct     = &Red;

}



void loop() {
  
   
      // Heart of the scheduler code
      for (i=0; i < tasksNum; ++i) {
         if ((count-tasks[i].lastExecutionTime) >= tasks[i].period) { // Ready
            tasks[i].TickFct(); //execute task tick
            tasks[i].lastExecutionTime = count;
         }
      }
     
}

void Blue(void)   {
    PORTB.OUT ^= (1 << 2); 
}

 // Task: Sequence a 1 across 3 outputs
void Red(void) {

    PORTA.OUT ^= (1 << 1); 
}