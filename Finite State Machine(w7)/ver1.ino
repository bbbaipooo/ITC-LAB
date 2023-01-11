//West
#define LED_RED_W 7
#define LED_YELLOW_W 8
#define LED_GREEN_W 9
#define W_BUTTON 3

//South
#define LED_RED_S 4
#define LED_YELLOW_S 5
#define LED_GREEN_S 6
#define S_BUTTON 2

//Walk
#define LED_GREEN_WALK 12
#define LED_RED_WALK 11
#define WALK_BUTTON 10

struct State 
{
  unsigned long ST_OUT;
  unsigned long Time;
  unsigned long Next[8];
};
typedef const struct State Stype;

Stype FSM[17] = 
{               // 000 , 001 , 010 , 011 , 100 , 101 , 110 , 111
  {B00110001,1000,{ 0  ,  2  ,  1  ,  1  ,  0  ,  2  ,  1  ,  1  }},          //0 S
  {B01010001,500,{  3  ,  3  ,  3  ,  3  ,  3  ,  3  ,  3  ,  3  }},          //1
  {B01010001,500,{  6  ,  6  ,  6  ,  6  ,  6  ,  6  ,  6  ,  6  }},          //2
  {B10000101,1000,{ 3  ,  5  ,  3  ,  5  ,  4  ,  5  ,  4  ,  5  }},          //3 W
  {B10001001,500,{  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  ,  0  }},          //4
  {B10001001,500,{  6  ,  6  ,  6  ,  6  ,  6  ,  6  ,  6  ,  6  }},          //5
  {B10010010,1000,{goH,goH,H_S1waitoff,H_S1waitoff,S_H1waitoff,S_H1waitoff,S_H1waitoff,S_H1waitoff}},//goH
  {B10010000,200,{H_S2waiton,H_S2waiton,H_S2waiton,H_S2waiton,H_S2waiton,H_S2waiton,H_S2waiton,H_S2waiton}},//H_S1 wait off
  {B10010010,200,{H_S3waitoff,H_S3waitoff,H_S3waitoff,H_S3waitoff,H_S3waitoff,H_S3waitoff,H_S3waitoff,H_S3waitoff}},//H_S2 wait off
  {B10010000,200,{H_S4waiton,H_S4waiton,H_S4waiton,H_S4waiton,H_S4waiton,H_S4waiton,H_S4waiton,H_S4waiton}},//H_S3 wait off
  {B10010010,200,{H_S5waitoff,H_S5waitoff,H_S5waitoff,H_S5waitoff,H_S5waitoff,H_S5waitoff,H_S5waitoff,H_S5waitoff}},//H_S4 wait off
  {B10010000,200,{H_S6waiton,H_S6waiton,H_S6waiton,H_S6waiton,H_S6waiton,H_S6waiton,H_S6waiton,H_S6waiton}},//H_S6 wait on 
  {B10010010,200,{goW,goW,goW,goW,goW,goW,goW,goW}},//H_S6 wait on
  {B10010000,200,{S_H2waiton,S_H2waiton,S_H2waiton,S_H2waiton,S_H2waiton,S_H2waiton,S_H2waiton,S_H2waiton}},//S_H1 wait off
  {B10010010,200,{S_H3waitoff,S_H3waitoff,S_H3waitoff,S_H3waitoff,S_H3waitoff,S_H3waitoff,S_H3waitoff,S_H3waitoff}},//S_H2 wait off 13
  {B10010000,200,{S_H4waiton,S_H4waiton,S_H4waiton,S_H4waiton,S_H4waiton,S_H4waiton,S_H4waiton,S_H4waiton}},//S_H3 wait off 14
  {B10010010,200,{S_H5waitoff,S_H5waitoff,S_H5waitoff,S_H5waitoff,S_H5waitoff,S_H5waitoff,S_H5waitoff,S_H5waitoff}},//S_H4 wait off 15
  {B10010000,200,{S_H6waiton,S_H6waiton,S_H6waiton,S_H6waiton,S_H6waiton,S_H6waiton,S_H6waiton,S_H6waiton}},//S_H5 wait off 16
  {B10010010,200,{goS,goS,goS,goS,goS,goS,goS,goS}}//S_H6 wait on 16
  

};

unsigned long S = 0 ;

void setup() 
{

  pinMode(LED_GREEN_WALK, OUTPUT);
  pinMode(LED_RED_WALK, OUTPUT);
  pinMode(WALK_BUTTON, INPUT);

  pinMode(LED_RED_S, OUTPUT);
  pinMode(LED_YELLOW_S, OUTPUT);
  pinMode(LED_GREEN_S, OUTPUT);  
  pinMode(S_BUTTON, INPUT);

  pinMode(LED_RED_W, OUTPUT);
  pinMode(LED_YELLOW_W, OUTPUT);
  pinMode(LED_GREEN_W, OUTPUT);  
  pinMode(W_BUTTON, INPUT);

}

int in,in1,in2,in3;

void loop() 
{

  digitalWrite(LED_RED_W,      FSM[S].ST_OUT & B10000000);
  digitalWrite(LED_YELLOW_W,   FSM[S].ST_OUT & B01000000);
  digitalWrite(LED_GREEN_W,    FSM[S].ST_OUT & B00100000);
  digitalWrite(LED_RED_S,      FSM[S].ST_OUT & B00010000);
  digitalWrite(LED_YELLOW_S,   FSM[S].ST_OUT & B00001000);
  digitalWrite(LED_GREEN_S,    FSM[S].ST_OUT & B00000100);
  digitalWrite(LED_GREEN_WALK, FSM[S].ST_OUT & B00000010);
  digitalWrite(LED_RED_WALK,   FSM[S].ST_OUT & B00000001);
  
  delay(FSM[S].Time);


  in1 = digitalRead(W_BUTTON);
  in2 = digitalRead(S_BUTTON);
  in3 = digitalRead(WALK_BUTTON);
  
  in = (((in1 * 2) + in2) * 2) + in3;
  S = FSM[S].Next[in];

}
