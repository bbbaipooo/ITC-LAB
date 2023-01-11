#include<Wire.h>//SLAVE-ARDUINO2
void setup()
{
  Wire.begin(2);
  Wire.onReceive(GetHello);
  Wire.onRequest(sendHello);
  Serial.begin(9600);
}
char receive[7];

void loop()
{
  ///
}

void GetHello(int howManyByte)
{
  for(int i=0;i<howManyByte;i++)
  {
    receive[i]=Wire.read();
  }
  //Serial.println(receive);
  if(receive[5]<'9')
  {
    receive[5]++;
  }
  else
  {
    receive[5]='1';
  }
}

void sendHello()
{
  Wire.write(receive);
  Serial.println(receive);
}
