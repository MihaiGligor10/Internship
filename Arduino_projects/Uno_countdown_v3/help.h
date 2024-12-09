#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);

void SerialCheckFunc(void);
void DisplayFunc(void);
int  pickTask();
void printStatus(int a, int i);
void ResetFunc(void);
void Action();
int  (*numberSelect(int nr))[4];
void createCustomChar();
void CounterFunc();
void updateQuadrants();

enum states { ACTIVE,
              READY,
              WAITING };

typedef struct task {
  unsigned long activationRate;
  unsigned long lastActivationTime;
  void (*taskFunc)(void);
  int priority;
  states state;
} task;

const int nrOfFunc = 3;
int Button[1][3] = {{1, 600, 850}};
task tasks[nrOfFunc];


int analogpin = A0;

int i = 0;
unsigned long counter=0;

int ButtonValue;
int label;
int auxCounter = 0;
int BPushes = 0;
int debounceTime = 50;
int lastDebounceTime = 0;

void initTasks()
{
  /////////////////////////!!!!!!!!!!!!!!!!!!!!
  //nu executa tasks[0]
  /////////////////////////!!!!!!!!!!!!!!!!!!!!

  i = 0;
  tasks[i].lastActivationTime = 0;
  tasks[i].state = WAITING;
  tasks[i].activationRate = 700;
  tasks[i].taskFunc = &updateQuadrants;
  tasks[i].priority = 1;
  ++i;
  tasks[i].lastActivationTime = 0;
  tasks[i].state = WAITING;
  tasks[i].activationRate = 500;
  tasks[i].taskFunc = &DisplayFunc;
  tasks[i].priority = 2;
  /*++i;
  tasks[i].lastActivationTime = 0;
  tasks[i].state = WAITING;
  tasks[i].activationRate = 1000;
  tasks[i].taskFunc = &CounterFunc;
  tasks[i].priority = 0;*/
  ++i;
  tasks[i].lastActivationTime = 0;
  tasks[i].state = WAITING;
  tasks[i].activationRate = 70;
  tasks[i].taskFunc = &ResetFunc;
  tasks[i].priority = 3;
  
}

void initLCD()
{
  lcd.init(); 
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);    
}

void setupTimer()
{
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  OCR1A = 62;//pt 1ms// set compare match register for 1hz increments
  TCCR1B |= (1 << WGM12); // turn on CTC mode
  TCCR1B |= (1 << CS12) ;  // Set CS12 for 256 prescaler
  TIMSK1 |= (1 << OCIE1A); // enable timer compare interrupt
}


void printStatus(int status, int taskNumber) {
  Serial.println(" ");
  Serial.print("Task number: ");
  Serial.println(taskNumber);
  if (status == 0)
    Serial.print("ACTIVE");
  else if (status == 2)
    Serial.print("WAITING");
  else
    Serial.print("READY");
}

void copyArrayContent(int arrSource[4][4], int arrDestination[4][4])
{
  for(int i=0; i<4 ;i++)
  {
    for(int j=0; j<4; j++)
    {
      arrDestination[i][j] = arrSource[i][j];
    }
  }
}

int nrOfDigits(int nr)
{
  int c=0;
  while(nr)
  {
    c++;
    nr=nr/10;
  }
  return c;
}