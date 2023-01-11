#include <TimerOne.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <EEPROM.h>
#define Button1 5 //save to EEPROM
#define Button2 6 //increase hour
#define Button3 7 //increase minute
#define Button4 8 //change mode
#define Clock 0
#define Alarm 1
#define Set 2
#define LED 10
#define OLED_RESET -1
Adafruit_SSD1306 OLED(OLED_RESET);
int8_t second;
int8_t minute;
int8_t hour;
int mode=0;
int8_t alarmMin;
int8_t alarmHour;

void setup()
{
    OLED.begin(SSD1306_SWITCHCAPVCC, 0x3c);
    Serial.begin(9600);
    Timer1.initialize();//()=(1000000)=1seconds
    Timer1.attachInterrupt(sumUpdate);
    EEPROM.get(0, second);
    EEPROM.get(1, minute);
    EEPROM.get(2, hour);
    EEPROM.get(3, alarmMin);
    EEPROM.get(4, alarmHour);
    pinMode(Button1, INPUT_PULLUP);
    pinMode(Button2, INPUT_PULLUP);
    pinMode(Button3, INPUT_PULLUP);
    pinMode(Button4, INPUT_PULLUP);
    pinMode(LED,OUTPUT);
}

int32_t bounce;
bool buttonPress(uint8_t pin)
{
    if (!digitalRead(pin))
    {
        if (millis() - bounce > 300)
        {
            bounce = millis();
            return true;
        }
        bounce = millis();
    }
    return false;
}

bool isUp;
void direction()
{
    int dirX = analogRead(A0);

    if (dirX > 400 && !isUp)
    {
        isUp = true;
        OLED.setRotation(0); //normal
    }
    if (dirX < 300 && isUp)
    {
        isUp = false;
        OLED.setRotation(2); //inverse
    }
}

void loop()
{
    direction();
    if (buttonPress(Button1))
    {
        EEPROM.update(0, second);
        EEPROM.update(1, minute);
        EEPROM.update(2, hour);
        EEPROM.update(3, alarmMin);
        EEPROM.update(4, alarmHour);
    }
    if (buttonPress(Button2))
    {
        if (mode == Set)
        {
            hour++;
            hour %= 24;
        }
        else if (mode == Alarm)
        {
            alarmHour++;
            alarmHour %= 24;
        } 
    }
    if (buttonPress(Button3))
    {
        if (mode == Set)
        {
            minute++;
            minute %= 60;
        }
        else if (mode == Alarm)
        {
            alarmMin++;
            alarmMin %= 60;
        }
    }
    if (buttonPress(Button4))
    {
        mode++;
        mode %= 3;
    }
    if (hour == alarmHour && minute == alarmMin)
    {
        digitalWrite(LED,1);
    }
    else
    {
        digitalWrite(LED,0);
    }

    OLED.clearDisplay();
    OLED.setCursor(10, 0);
    OLED.setTextColor(WHITE);
    OLED.setTextSize(2);
    if (mode == Clock || mode == Set)
    {
        OLED.print(hour);
        OLED.print(":");
        OLED.print(minute);
        OLED.print(":");
        OLED.print(second);
    }
    else if (mode == Alarm)
    {
        OLED.print(alarmHour);
        OLED.print(":");
        OLED.print(alarmMin);
    }
    OLED.display();
}

void sumUpdate()
{
    second++;
    if (second == 60)
    {
        minute++;
        second = 0;
    }
    if (minute == 60)
    {
        hour++;
        minute = 0;
    }
    if (hour == 24)
    {
        hour = 0;
    }
}
