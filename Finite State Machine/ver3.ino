#define LED_redS 7
#define LED_yellowS 8
#define LED_greenS 9
#define buttonS 3

#define LED_redW 4
#define LED_yellowW 5
#define LED_greenW 6
#define buttonW 2

#define LED_greenH 12
#define LED_redH 11
#define buttonH 10

#define goS 0
#define H_Wwait 1
#define S_Wwait 2
#define goW 3
#define W_Hwait 4
#define W_Swait 5
#define goH 6
#define H_S1waitoff 7
#define H_S2waiton 8
#define H_S3waitoff 9
#define H_S4waiton 10
#define H_S5waitoff 11
#define S_H1waitoff 12
#define S_H2waiton 13
#define S_H3waitoff 14
#define S_H4waiton 15
#define S_H5waitoff 16

struct State
{
  unsigned long ST_Out;
  unsigned long Time;
  unsigned long Next[8];
};
typedef const struct State SType;

SType FSM[17]
{
  {B10000101,1000,{0,1,0,1,2,1,2,1}},//goS 0
  {B10001001,500,{6,6,6,6,6,6,6,6}},//H_Wwait 1
  {B10001001,500,{3,3,3,3,3,3,3,3}},//S_Wwait 2
  {B00110001,1000,{3,4,5,5,3,4,5,5}},//goW 3
  {B01010001,500,{6,6,6,6,6,6,6,6}},//W_Hwait 4
  {B01010001,500,{0,0,0,0,0,0,0,0,}},//W_Swait 5
  {B10010010,1000,{6,6,7,7,12,12,12,12}},//goH 6
  {B10010000,200,{8,8,8,8,8,8,8,8}},//H_S1 wait off 7
  {B10010010,200,{9,9,9,9,9,9,9,9}},//H_S2 wait off 8
  {B10010000,200,{10,10,10,10,10,10,10,10}},//H_S3 wait off 9
  {B10010010,200,{11,11,11,11,11,11,11,11}},//H_S4 wait off 10
  {B10010000,200,{0,0,0,0,0,0,0,0}},//H_S5 wait off 11
  {B10010000,200,{13,13,13,13,13,13,13,13}},//S_H1 wait off 12
  {B10010010,200,{14,14,14,14,14,14,14,14}},//S_H2 wait off 13
  {B10010000,200,{15,15,15,15,15,15,15,15}},//S_H3 wait off 14
  {B10010010,200,{16,16,16,16,16,16,16,16}},//S_H4 wait off 15
  {B10010000,200,{3,3,3,3,3,3,3,3}}//S_H5 wait off 16
};

unsigned long S=0;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_redS,OUTPUT);
  pinMode(LED_yellowS,OUTPUT);
  pinMode(LED_greenS,OUTPUT);
  pinMode(buttonS,INPUT);

  pinMode(LED_redW,OUTPUT);
  pinMode(LED_yellowW,OUTPUT);
  pinMode(LED_greenW,OUTPUT);
  pinMode(buttonW,INPUT);

  pinMode(LED_greenH,OUTPUT);
  pinMode(LED_redH,OUTPUT);
  pinMode(buttonH,INPUT);
}

int input,input1,input2,input3;

void loop() {
  // put your main code here, to run repeatedly:
  pinMode(LED_redS,FSM[S].ST_Out&B00010000);
  pinMode(LED_yellowS,FSM[S].ST_Out&B00001000);
  pinMode(LED_greenS,FSM[S].ST_Out&B00000100);

  pinMode(LED_redW,FSM[S].ST_Out&B1000000);
  pinMode(LED_yellowW,FSM[S].ST_Out&B0100000);
  pinMode(LED_greenW,FSM[S].ST_Out&B0010000);

  pinMode(LED_redH,FSM[S].ST_Out&B00000010);
  pinMode(LED_greenH,FSM[S].ST_Out&B00000001);

  delay(FSM[S].Time);

  //input=digitalRead;
  input1=digitalRead(buttonW);
  input2=digitalRead(buttonS);
  input3=digitalRead(buttonH);

  input=(((input1*2)+input2)*2)+input3;
  S=FSM[S].Next[input];
}
