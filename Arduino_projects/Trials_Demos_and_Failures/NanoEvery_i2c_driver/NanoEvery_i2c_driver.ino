#include <avr/interrupt.h>
#include "help.h"

#define ADDR 0x27

byte Heart[8] = {
  0b00000,
  0b01010,
  0b11111,
  0b11111,
  0b01110,
  0b00100,
  0b00000,
  0b00000
};
char b[8] = {0x08,0x80,0xFF,0xEA};
char initChar[1] = {0x27};
char display[1] = {0x00};

//0100 1110
void setup()
{
  //setupTimerB0();

  Serial.begin(9600);
  PORTC.DIR |= (1 << 6);  //clk pin for i2c (D4)
  PORTE.DIR |= (1 << 3);  //output data pin for i2c (D8)
  PORTE.OUT |= (1 << 3);  //output data pin for i2c
  
  initI2C((ADDR << 1) | 0);
  //writeData(display, 1);
  //writeData(initChar,1);
  writeData(b,4);
}


void loop() 
{

}



void initI2C(char addr)
{
  startI2C();                    
  for ( int i=0; i<8; i++)      //parcurgem bitii
  {
    i2c_writebit(addr & 0x80);  //incepem de la cel mai mare bit
    addr <<= 1;    
    delay(1);             //trecem la urmatorul bit
  }
}

void writeData(char* b, char dlc) 
{
  //startI2C();
  Serial.print(".");
  for (int i = 0; i < dlc; i++)    // parcurgem fiecare byte
  {
    for (int j = 8; j >0; j--)     // la fiecare byte parcurgem fiecare bit
    {
      i2c_writebit(b[i] & (1 << j));
    }
    PORTE.DIR &=~(1 << 3);        // punem data pe input pt citire ack
    Serial.print(" ");
    PORTC.OUT &=~(1 << 6);
    Serial.print("0");        // clk pe low
    delay(1);
    digitalRead(8);               //citim ack
    delay(1);
    PORTC.OUT |= (1 << 6);  
    Serial.print("1");      //clk high
    delay(1);
    PORTE.DIR |= (1 << 3);        // punem data pe output
    delay(1);
    Serial.print(" ");
  }
  stopI2C();                      //conditia de oprire
}



void startI2C() 
{
  PORTC.OUT |= (1 << 6);
  Serial.print( "|1 in start | ");
  
  delay(1);
  PORTE.OUT &= ~(1 << 3);  //A HIGH-to-LOW transition of the data line, while the
  //clock is HIGH is defined as the start condition (S).
  delay(4);
}



void stopI2C() 
{
  //PORTC.OUT |= (1 << 6);
  //Serial.print("1");
  delay(1);
  PORTE.OUT |= (1 << 3);  //A LOW-to-HIGH transition of the data line while the 
  //clock is HIGH is defined as the stop condition (P)
  delay(4);

}

void i2c_writebit(int a)
{
  PORTC.OUT &= ~(1 << 6);
  Serial.print("0");
  delay(1);
  //PORTE.OUT &= ~(1 << 3); 
  delay(1);

  if(a==0)
  {
    PORTE.OUT &= ~(1 << 3);
  }
  else
  {
    PORTE.OUT |= (1 << 3);
  }

  PORTC.OUT |= (1 << 6);
  Serial.print("1");
  delay(1);
}
