#define LED_W_R 4
#define LED_W_Y 5
#define LED_W_G 6
#define WEST_BUTTON_PIN 2

#define LED_S_R 7
#define LED_S_Y 8
#define LED_S_G 9
#define SOUTH_BUTTON_PIN 3

#define LED_P_R 11
#define LED_P_G 12
#define H_BUTTON_PIN 10

#define goS 0
#define HwaitS 1
#define WwaitS 2
#define goW 3
#define HwaitW 4
#define SwaitW 5
#define goH 6
#define SwaitH_off1 7
#define SwaitH_on1 8
#define SwaitH_off2 9
#define SwaitH_on2 10
#define SwaitH_off3 11
#define SwaitH_on3 12
#define WwaitH_off1 13
#define WwaitH_on1 14
#define WwaitH_off2 15
#define WwaitH_on2 16

struct State
{
  unsigned long ST_Out;
  unsigned long Time;
  unsigned long Next[8];
};
typedef const struct State SType;
// 110
SType FSM[19]={
  {B10100001,3000,{goS,WwaitS,goS,WwaitS,HwaitS,HwaitS,HwaitS,HwaitS}},
  {B10100010,1000,{goH,goH,goH,goH,goH,goH,goH,goH}},
  {B10100010,1000,{goW,goW,goW,goW,goW,goW,goW,goW}},
  {B10001100,3000,{goW,goW,SwaitW,SwaitW,HwaitW,HwaitW,HwaitW,HwaitW}},
  {B10010100,1000,{goH,goH,goH,goH,goH,goH,goH,goH}},
  {B10010100,1000,{goS,goS,goS,goS,goS,goS,goS,goS}},
  {B01100100,500,{goH,WwaitH_off1,SwaitH_off1,SwaitH_off1,goH,goH,goH,goH}},
  {B00100100,500,{SwaitH_on1,SwaitH_on1,SwaitH_on1,SwaitH_on1,SwaitH_on1,SwaitH_on1,SwaitH_on1,SwaitH_on1}},
  {B01100100,500,{SwaitH_off2,SwaitH_off2,SwaitH_off2,SwaitH_off2,SwaitH_off2,SwaitH_off2,SwaitH_off2,SwaitH_off2}},
  {B00100100,500,{SwaitH_on2,SwaitH_on2,SwaitH_on2,SwaitH_on2,SwaitH_on2,SwaitH_on2,SwaitH_on2,SwaitH_on2}},
  {B01100100,500,{SwaitH_off3,SwaitH_off3,SwaitH_off3,SwaitH_off3,SwaitH_off3,SwaitH_off3,SwaitH_off3,SwaitH_off3}},
  {B00100100,500,{SwaitH_on3,SwaitH_on3,SwaitH_on3,SwaitH_on3,SwaitH_on3,SwaitH_on3,SwaitH_on3,SwaitH_on3}},
  {B01100100,500,{goS,goS,goS,goS,goS,goS,goS,goS}},
  {B00100100,500,{WwaitH_on1,WwaitH_on1,WwaitH_on1,WwaitH_on1,WwaitH_on1,WwaitH_on1,WwaitH_on1,WwaitH_on1}},
  {B01100100,500,{WwaitH_off2,WwaitH_off2,WwaitH_off2,WwaitH_off2,WwaitH_off2,WwaitH_off2,WwaitH_off2,WwaitH_off2}},
  {B00100100,500,{WwaitH_on2,WwaitH_on2,WwaitH_on2,WwaitH_on2,WwaitH_on2,WwaitH_on2,WwaitH_on2,WwaitH_on2}},
  {B01100100,500,{goH,goH,goH,goH,goH,goH,goH,goH}}
};

unsigned long S=0;

void setup()
{
  pinMode(LED_W_R, OUTPUT);
  pinMode(LED_W_Y, OUTPUT);
  pinMode(LED_W_G, OUTPUT);
  pinMode(WEST_BUTTON_PIN, INPUT);
  
  pinMode(LED_S_R, OUTPUT);
  pinMode(LED_S_Y, OUTPUT);
  pinMode(LED_S_G, OUTPUT);
  pinMode(SOUTH_BUTTON_PIN, INPUT);
  
  pinMode(LED_P_R, OUTPUT);
  pinMode(LED_P_G, OUTPUT);
  pinMode(H_BUTTON_PIN, INPUT);
}

int input, input1, input2, input3;

void loop()
{
  digitalWrite(LED_W_R, FSM[S].ST_Out & B00100000);
  digitalWrite(LED_W_Y, FSM[S].ST_Out & B00010000);
  digitalWrite(LED_W_G, FSM[S].ST_Out & B00001000);
  
  digitalWrite(LED_S_R, FSM[S].ST_Out & B00000100);
  digitalWrite(LED_S_Y, FSM[S].ST_Out & B00000010);
  digitalWrite(LED_S_G, FSM[S].ST_Out & B00000001);
  
  digitalWrite(LED_P_R, FSM[S].ST_Out & B10000000);
  digitalWrite(LED_P_G, FSM[S].ST_Out & B01000000);
  
  delay(FSM[S].Time);
  
  input1 = digitalRead(WEST_BUTTON_PIN);
  input2 = digitalRead(SOUTH_BUTTON_PIN);
  input3 = digitalRead(H_BUTTON_PIN);
  
  input = (((input3 * 2) + input2) * 2) + input1;
  S = FSM[S].Next[input]; 
}
