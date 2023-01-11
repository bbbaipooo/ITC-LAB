int num[10] = {B0100000, B1111001, B1000100, B1010000, B0011001, B0010010, B0000010, B1111000, B0000000, B0010000};  //0-9
int compare[2] = {B0100010, B0100111}; //G,L
const byte segmentPins = 7; 
const int segment[7] = {4, 5, 6, 7, 8, 10, 9}; //pin
int switch1 = 2;
int switch2 = 3;
int i = 1;
int c1 = 0;
int c2 = 0;
int randomNo;
void setup() {
  Serial.begin(9600);
  for (int i = 0; i < segmentPins; i++) {
    pinMode(segment[i], OUTPUT);
  }
  pinMode(switch1, INPUT_PULLUP);
  pinMode(switch2, INPUT_PULLUP);
  randomSeed(analogRead(A0));
  randomNo = random(1, 10);
}
void loop() {
  Serial.println(randomNo);
  start_switch();
  guess_switch();
}
void start_switch() {
  boolean setBit;
  int check_s1 = digitalRead(switch1);
  if (check_s1 != c1 && c1 == 0) {
    i += 1;
    c1 = 1;
  } else if (check_s1 == 0) c1 = 0;
  if (i >= 10)i = 1;
  for (int x = 0; x < 8; x++) {
    setBit = bitRead(num[i], x);
    digitalWrite(segment[x], setBit);
  }
}
void guess_switch() {
  int check_s2 = digitalRead(switch2);
  boolean a;
  if (check_s2 != c2 && c2 == 0) {
    c2 = 1;
    if (i > randomNo) {
      for (int x = 0; x < 8; x++) {
        a = bitRead(compare[0], x);
        digitalWrite(segment[x], a);
      }
      delay(500);
    }
    else if (i < randomNo) {
      for (int x = 0; x < 8; x++) {
        a = bitRead(compare[1], x);
        digitalWrite(segment[x], a);
      }
      delay(500);
    } else if (i == randomNo) {
      i = 0;
      for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 8; x++) {
          a = bitRead(num[i], x);
          digitalWrite(segment[x], HIGH);
        }
        delay(250);
        for (int x = 0; x < 8; x++) {
          a = bitRead(num[i], x);
          digitalWrite(segment[x], a);
        }
        delay(250);
      }
      randomNo = random(1, 10);
    }
  } else if (check_s2 == 0) c2 = 0;
}
