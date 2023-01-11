#include<Wire.h>//Master-Arduino1
void setup()
{
  Wire.begin();
  Serial.begin(9600);
}
char hello[7]="Hello1";

void loop()
{
  OHAYO_GOZAIMASU(2);
  welcomeToPrint(2);
  delay(50);
  OHAYO_GOZAIMASU(3);
  welcomeToPrint(3);
  delay(50);
}


void OHAYO_GOZAIMASU(int ADDR)
{
  Wire.beginTransmission(ADDR);
  Wire.write(hello);
  Wire.endTransmission();
}
void welcomeToPrint(int ADDR)
{
  char received[7];
  Wire.requestFrom(ADDR,6);
  for(int i=0;i<6;i++)
  {
    hello[i]=Wire.read();
  }
  Serial.println(hello);
}
