enum states { ACTIVE,
              READY,
              WAITING };

typedef struct task {
  unsigned long activationRate;
  unsigned long lastActivationTime;
  void (*taskFunc)(int e);
  int executionTime;
  int priority;
  states state;
} task;

task tasks[3];
int i = 0;
unsigned long counter;
const int nrOfFunc = 3;

void Blue(int time);
void Red(int time);
void Orange(int time);
int pickTask();
void printStatus(int a, int i);



void initTasks()
{
   for (i = 0; i < nrOfFunc; i++) {
    tasks[i].lastActivationTime = 0;
    tasks[i].state = WAITING;
  }
  i = 0;
  tasks[i].activationRate = 400;
  tasks[i].taskFunc = &Blue;
  tasks[i].executionTime = 200;
  tasks[i].priority = 2;
  ++i;
  tasks[i].activationRate = 100;
  tasks[i].taskFunc = &Red;
  tasks[i].executionTime = 150;
  tasks[i].priority = 0;
  ++i;
  tasks[i].activationRate = 662;
  tasks[i].taskFunc = &Orange;
  tasks[i].executionTime = 100;
  tasks[i].priority = 1;
}

void setupTimerB0()
{
  TCB0.CCMP = 0x0000;
  TCA0.SINGLE.CTRLA = 0x00;

  TCB0.CCMP = 0x003E;
  TCB0.CTRLB = 0x00;        // interrupt mode
  TCB0.CTRLA |= 0b0000101;  //enable timer, assign TCA0 clock
  TCB0.INTCTRL = TCB_CAPT_bm;
  TCA0.SINGLE.CTRLA |= 0b00001101;
}
