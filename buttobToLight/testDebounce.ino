int button[3]={12,11,10};
int LED[3]={4,3,2};
int buttonState[3]={LOW,HIGH,HIGH};
int lastButtonState[3]={LOW,HIGH,HIGH};
long lastDebounceTime[3]={0,0,0};
long debounceDelay=50;
void setup() {
  // put your setup code here, to run once:
  pinMode(button[0],INPUT);
  pinMode(LED[0],OUTPUT);

}
void checkDebounce(int a)
{
  int reading=digitalRead(button[a]);

  if(reading!=lastButtonState[a])
  {
    lastDebounceTime[a]=millis();
  }
  if((millis()-lastDebounceTime[a])>debounceDelay)
  {
    if(reading!=buttonState[a])
    {
      buttonState[a]=reading;
    }
  }
  lastButtonState[a]=reading;
}
void loop() {
  // put your main code here, to run repeatedly:
  checkDebounce(0);

  
  if(buttonState[0]==HIGH)
  {
    if(digitalRead(LED[0])==LOW)
      digitalWrite(LED[0],HIGH);
    else
      digitalWrite(LED[0],LOW);
  }

}

 
