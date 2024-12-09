#include <avr/interrupt.h>
#define ADDR 0x27

void startI2C();
void stopI2C();
void initI2C(char addr);
void writeData(char*  b,char dlc);
void i2c_writebit(int a);

char b[8] = {0x08,0x80,0xFF,0xEA};
char initChar[1] = {0x27};
char display[1] = {0x00};

//0100 1110
void setup()
{
  //setupTimerB0();

  Serial.begin(9600);
  DDRD |= (1 << 6);  //clk pin for i2c ()
  DDRD |= (1 << 3);  //output data pin for i2c ()
  PORTD |= (1 << 3);  //output data pin for i2c
  
 //initI2C((ADDR << 1) | 0);
  initI2C(ADDR);
  writeData(display, 1);
  //writeData(initChar,1);
  writeData(b,4);
  stopI2C();  
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
    DDRD &=~(1 << 3);        // punem data pe input pt citire ack
    Serial.print(" ");
    PORTD &=~(1 << 6);
    Serial.print("0");        // clk pe low
    delay(1);
    digitalRead(8);               //citim ack
    delay(1);
    PORTD |= (1 << 6);  
    Serial.print("1");      //clk high
    delay(1);
    DDRD |= (1 << 3);        // punem data pe output
    delay(1);
    Serial.print(" ");
  }
    //conditia de oprire
}



void startI2C() 
{
  PORTD |= (1 << 6);
  Serial.print( "|1 in start | ");
  
  delay(1);
  PORTD &= ~(1 << 3);  //A HIGH-to-LOW transition of the data line, while the
  //clock is HIGH is defined as the start condition (S).
  delay(4);
}



void stopI2C() 
{
  //PORTD |= (1 << 6);
  //Serial.print("1");
  delay(1);
  PORTD |= (1 << 3);  //A LOW-to-HIGH transition of the data line while the 
  //clock is HIGH is defined as the stop condition (P)
  delay(4);
  Serial.print("|stop|");

}

void i2c_writebit(int a)
{
  PORTD &= ~(1 << 6);
  Serial.print("0");
  delay(1);
  //PORTD &= ~(1 << 3); 
  delay(1);

  if(a==0)
  {
    PORTD &= ~(1 << 3);
  }
  else
  {
    PORTD |= (1 << 3);
  }

  PORTD |= (1 << 6);
  Serial.print("1");
  delay(1);
}

