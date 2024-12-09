#include <avr/interrupt.h>
#include "help.h"

ISR(TCB0_INT_vect) {
  TCB0.INTFLAGS |= TCB_CAPT_bm;
  counter++;
}

void setup() {
  setupTimerB0();
  initTasks();

  Serial.begin(9600);
  PORTB.DIR |= (1 << 2); //led pin
  PORTA.DIR |= (1 << 1); //led pin
  PORTE.DIR |= (1 << 2); //led pin
  PORTC.DIR |= (1 << 6); //clk pin for i2c
  PORTE.DIR |= (1 << 2); //output data pin for i2c
}


void loop() {
  i = pickTask();
  if (i == -1) {
    Serial.println(" ");
    Serial.println(" ");
    Serial.println("IDLE");
    Serial.println(" ");
    Serial.println(" ");
  } else {
    tasks[i].state = ACTIVE;
    tasks[i].taskFunc(tasks[i].executionTime);
    printStatus(tasks[i].state, i);
    tasks[i].state = WAITING;
    printStatus(tasks[i].state, i);
    tasks[i].lastActivationTime = counter;
    Serial.println(" ");
  }
}

int pickTask() {
  for (i = 0; i < nrOfFunc; i++) {// selectam care dintre taskuri ar trebui activate
    if ((counter - tasks[i].lastActivationTime) >= tasks[i].activationRate) { 
      tasks[i].state = READY;
    }
  }
  int returnTask;
  returnTask = -1;


  for (i = 0; i < nrOfFunc-1; i++) {  //din taskurile care au status ready, selectam pe cel mai prioritar
    if (tasks[i].state != READY)
      continue;
    for (int j = 1; j < nrOfFunc ; j++) {
      if (tasks[j].state != READY)
        continue;
      if (tasks[i].priority > tasks[j].priority) {
        returnTask = i;
        tasks[j].state = WAITING;
      } else {
        returnTask = j;
        tasks[i].state = WAITING;
      }
    }
  }
  return returnTask;
}

void printStatus(int a, int i) {
  Serial.println(" ");
  Serial.print("Task number: ");
  Serial.println(i);
  if (a == 0)
    Serial.print("ACTIVE");
  else if (a == 2)
    Serial.print("WAITING");
  else
    Serial.print("READY");
}

void Blue(int time) {
  PORTB.OUT ^= (1 << 2);
  //delay(time);
}

void Red(int time) {
  PORTA.OUT ^= (1 << 1); 
  //delay(time);
}

void Orange(int time) {
  PORTE.OUT ^= (1 << 2);
  //delay(time);
}

