#include <avr/interrupt.h>

enum states { ACTIVE,
              WAITING };

typedef struct task {
  unsigned long activationRate;
  unsigned long lastActivationTime;
  void (*taskFunc)(void);
  //int exTime;
  states state;

} task;

task tasks[3];
int i = 0;
unsigned long counter;
const int nrOfFunc = 3;
ISR(TCB0_INT_vect) {
  TCB0.INTFLAGS |= TCB_CAPT_bm;
  counter++;
}

void Blue(void);
void Red(void);
void Orange(void);


void setup() {
  TCB2.CCMP = 0x0000;
  TCB2.CTRLB = 0x00;
  TCB2.CTRLA = 0x00;
  TCA0.SINGLE.CTRLA = 0x00;

  TCB0.CCMP = 0x003E;
  TCB0.CTRLB = 0x00;        // interrupt mode
  TCB0.CTRLA |= 0b0000101;  //enable timer, assign TCA0 clock
  TCB0.INTCTRL = TCB_CAPT_bm;
  TCA0.SINGLE.CTRLA |= 0b00001101;

  for (i = 0; i < nrOfFunc; i++) {
    tasks[i].lastActivationTime = 0;
    tasks[i].state = WAITING;
  }
  i = 0;
  tasks[i].activationRate = 500;
  tasks[i].taskFunc = &Blue;
  ++i;
  tasks[i].activationRate = 280;
  tasks[i].taskFunc = &Red;
  ++i;
  tasks[i].activationRate = 665;
  tasks[i].taskFunc = &Orange;

  Serial.begin(9600);
  PORTB.DIR |= (1 << 2);
  PORTA.DIR |= (1 << 1);
  PORTE.DIR |= (1 << 2);
}

void loop() {
  for (i = 0; i < nrOfFunc; i++) {
    
    printStatus(tasks[i].state,i);
    if ((counter - tasks[i].lastActivationTime) >= tasks[i].activationRate) {
      tasks[i].state = ACTIVE;
      tasks[i].taskFunc();
      printStatus(tasks[i].state,i);
      tasks[i].state = WAITING;
      printStatus(tasks[i].state,i);
      tasks[i].lastActivationTime = counter;
    }
    else
    {
       tasks[i].state = WAITING;
    }
      Serial.println(" ");
  }
}

void printStatus(int a,int i)
{
  Serial.println(" ");
  Serial.print("Task number: ");
  Serial.println(i);
  if(a==0)
   Serial.print("ACTIVE");
   else
   Serial.print("WAITING");
}

void Blue(void) {
  PORTB.OUT ^= (1 << 2);
}

void Red(void) {
  PORTA.OUT ^= (1 << 1);
}

void Orange(void) {
  PORTE.OUT ^= (1 << 2);
}
