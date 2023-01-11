int button[3]{12,11,10};
int LED[3]={4,3,2};
int state[2]={0,0};
long time[3]={0,0,0};
long debounceTime[3]={0,0,0};
int i = 5;

void setup() 
  // put your setup code here, to run once:
{
  pinMode(button[0],INPUT);
  pinMode(button[1],INPUT);
  pinMode(button[2],INPUT_PULLUP);
  pinMode(LED[0],OUTPUT); 
  pinMode(LED[1],OUTPUT);
  pinMode(LED[2],OUTPUT);
}
void loop() {
  // put your main code here, to run repeatedly:
  
  ///////////////////RED/////////////////////
  if(digitalRead(button[0]) == 1 && state[0] == 0)
  {
    if(millis() - debounceTime[0] >= 1000)
     {
       digitalWrite(LED[0],1);
       state[0] = 1;
       time[0] = millis();
       debounceTime[0]=millis();
     } 
  }
  else if(state[0] == 1 && millis() - time[0] >= 3000)
  {
    digitalWrite(LED[0],0);
    state[0] = 0;
    debounceTime[0] = millis();
  }
  else if(digitalRead(button[0]) == 1 && state[0] == 1)
  {
    if(millis() - debounceTime[0] >= 1000)
    {
      digitalWrite(LED[0],0);
      state[0] = 0;
      debounceTime[0] = millis();
    }
  }
  ////////////////GREEN/////////////////
  if(digitalRead(button[2]) == 0 && state[1] == 0 && digitalRead(LED[0]) == 0)
  {
    if(millis() - debounceTime[2] >= 1000){
    digitalWrite(LED[2],1);
    state[1] = 1;
    time[2] = millis();
    debounceTime[2] = millis();
    }
  } 
  else if(state[1] == 1 && millis() - time[2] >= 3000)
  {
    digitalWrite(LED[2],0);
    state[1] = 0;
    debounceTime[2] = millis();
  }
  else if(digitalRead(button[2]) == 0 && state[1] == 1)
  {
    if(millis() - debounceTime[2] >= 1000)
    {
      digitalWrite(LED[2],0);
      state[1] = 0;
      debounceTime[2] = millis();
    }
  }
  ///////////////YELLOW///////////////
  if(digitalRead(button[1])==0 && digitalRead(LED[0])==0 && digitalRead(LED[2])==0)
  {
    if(millis() - debounceTime[1] >= 1000)
    {
      time[1] = millis();
      debounceTime[1] = millis();
      i = 1;
    }
  }
  if(millis() - time[1] >= 500 && i<=4)
  {
    digitalWrite(LED[1],!digitalRead(LED[1]));
    time[1] = millis();
    i++;
  }  
}
