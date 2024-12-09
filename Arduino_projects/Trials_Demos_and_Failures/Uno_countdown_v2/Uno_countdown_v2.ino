#include <avr/interrupt.h>
#include "help.h"
#include "NumbersLCD.h"

int auxBtnValue=0;
int volatile secunde = 59;
int volatile minute = 1;

int Q0[4][4], Q1[4][4], Q2[4][4], Q3[4][4], Q4[4][4];   

ISR(TIMER1_COMPA_vect) {
  counter++;
 if(counter % 1000 == 0)
  {
    CounterFunc();
  }
}



void setup() {
  pinMode(analogpin ,INPUT);
  initLCD();
  createCustomChar();
  Serial.begin(9600); 
  setupTimer();
  initTasks();
}


void loop() {

  i = pickTask();
  /*Serial.print("i=");
  Serial.println(i);
  Serial.println(" ");*/
  if (i != -1) 
  {
    tasks[i].state = ACTIVE;
    tasks[i].taskFunc();
    tasks[i].state = WAITING;
    tasks[i].lastActivationTime = counter;
  }
}

//functie care tine evidenta ceasului, 
//task activat odata pe secunda
void CounterFunc() //task cu prioritate mare care sa fie apelat la o secunda
{
  Serial.println("CounterFunc");
  secunde--;
  if(secunde < 0)
  {
    secunde = 59;
    minute--;
    if(minute < 0)
    {
      minute = 59;
    }
  }
}

void updateQuadrants()
{
  Serial.println("updateQuadrants");
  int nrDM = nrOfDigits(minute);
  int nrDS = nrOfDigits(secunde);

  if(nrDM == 1)
  {
    copyArrayContent(bitmap0,Q0);
    copyArrayContent(numberSelect(minute),Q1);
  }
  else
  {
    copyArrayContent(numberSelect((minute / 10)),Q0);
    copyArrayContent(numberSelect((minute % 10)),Q1);
  }

  copyArrayContent(bitmapp,Q2);
  
  if(nrDS == 1)
  {
    copyArrayContent(bitmap0,Q3);
    copyArrayContent(numberSelect(secunde),Q4);
  }
  else
  {
    copyArrayContent(numberSelect((secunde / 10)),Q3);
    copyArrayContent(numberSelect((secunde % 10)),Q4);
  }
}

void DisplayFunc()
{
  Serial.println("DisplayFunc");
  displayOneDigit(Q0,0);
  displayOneDigit(Q1,4);
  displayOneDigit(Q2,8);
  displayOneDigit(Q3,12);
  displayOneDigit(Q4,16);
  Serial.println(" ");
}

void displayOneDigit(int digit[4][4],int offset)
{
  
  for(int i=0; i<4 ;i++)
  {
    for(int j=0; j<4; j++)
    { 
      lcd.home();
      lcd.setCursor(j+offset, i);
      lcd.write(digit[i][j]);
    }
  }

}


bool comp ;
void ResetFunc()
{
  Serial.println("ResetFunc");
  ButtonValue = analogRead(analogpin); 
  comp = (auxBtnValue >= Button[0][1] && auxBtnValue <= Button[0][2]);
  if(((counter - lastDebounceTime)  > debounceTime) && comp == false)
  {
    Serial.println(ButtonValue);
    if(ButtonValue >= Button[0][1] && ButtonValue <= Button[0][2])
    {
      secunde = 59;
      minute = 59;  
    }
  }
  lastDebounceTime = counter ; 
  auxBtnValue = ButtonValue;
}

int pickTask() {  

  int returnTask;
  returnTask = -1;
  int check=0;
  // selectam care dintre taskuri ar trebui activate
  for ( i=0; i<nrOfFunc; i++) 
  {
    if ((counter - tasks[i].lastActivationTime) >= tasks[i].activationRate) 
    { 
      tasks[i].state = READY;
    }
    printStatus(tasks[i].state,i);
  }

  //din taskurile care au status ready, selectam pe cel mai prioritar
  for (i = 0; i < nrOfFunc; i++) 
  {  
    if (tasks[i].state != READY)
    {
      continue;
    }
      
    for (int j = 1; j < nrOfFunc ; j++) 
    {
      if (tasks[j].state != READY)
      {
        continue;
      }
      check++;
      if (tasks[i].priority < tasks[j].priority) 
      {
        returnTask = i;
        tasks[j].state = WAITING;
      } 
      else 
      {
        returnTask = j;
        tasks[i].state = WAITING;
      }
    }
    
    if(check==0)
    {
      returnTask = i;
      break;
    }
  }
  
  
  Serial.println(" ");
  Serial.print("taskReturned:------------> ");
  Serial.println(returnTask);
  return returnTask;
}



int (*numberSelect(int nr))[4]
{
  switch (nr) 
  {
  case 0:
  return bitmap0;
  case 1:
  return bitmap1;
  case 2:
  return bitmap2;
  case 3:
  return bitmap3;
  case 4:
  return bitmap4;
  case 5:
  return bitmap5;
  case 6:
  return bitmap6;
  case 7:
  return bitmap7;
  case 8:
  return bitmap8;
  case 9:
  return bitmap9;

  default:
  return bitmapp;
  }
}

void createCustomChar()
{
  lcd.createChar(0, emptyBlock);
  lcd.createChar(1, fullBlock);
  lcd.createChar(2, upperHalfBlock);
  lcd.createChar(3, lowerHalfBlock);
}