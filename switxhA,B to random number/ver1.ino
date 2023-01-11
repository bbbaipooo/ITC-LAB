#define button1 8
#define button2 9
int num[] = { 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F , 0x38, 0x3D};

int buttonstate1 = 0;
int buttonstate2 = 0;

long lastDebounceTime1 = 0;
long lastDebounceTime2 = 0;

int lastButtonState1 = LOW;
int lastButtonState2 = LOW;

int i = 1,randomNo;
void setup() {
  DDRD = 0xFF; 

  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  
  randomSeed(1);
  randomNo = random(1, 10);
  PORTD = ~num[1];
}

void loop() {
  int reading1 = digitalRead(button1);
  int reading2 = digitalRead(button2);
  
  //button1-------------plus1 1-9-------------------------------------------------------

  if (reading1 != lastButtonState1) {
    lastDebounceTime1 = millis();
  }
  if ((millis() - lastDebounceTime1) > 50) {
    if (reading1 != buttonstate1) {
      if (buttonstate1 == HIGH) {
        if (i == 9)
        {
          i = 0;
        }
        i++;
        PORTD = ~num[i];
      }
      buttonstate1 = reading1;
    }
  }

  //---------------------------------------------------------------------------------

  if (reading2 != lastButtonState2) {
    lastDebounceTime2 = millis();
  }
  if ((millis() - lastDebounceTime2) > 50) {
    
    if (reading2 != buttonstate2) {
      if (buttonstate2 == HIGH) {
        if (i == randomNo) {
          PORTD = ~num[0];
          delay (1000);
          i = 1;
          randomNo = random(1, 10);
        }
        else if (i > randomNo) {
          PORTD = ~num[11];
          delay (1000);

        }
        else
        {
          PORTD = ~num[10];
          delay (1000);
        }
      }
      buttonstate2 = reading2;
    }
    PORTD = ~num[i];
  }
  lastButtonState1 = reading1;
  lastButtonState2 = reading2;
}
