#include <Arduino_FreeRTOS.h>
#include "queue.h"

#define FAIRED 6
#define FAIYELLOW 7
#define FAIGREEN 8

#define PUM1 3
#define PUM2 4
#define PUM3 5

QueueHandle_t redQ, yellowQ, greenQ;

void setup()
{
    redQ = xQueueCreate(10, sizeof(bool));
    yellowQ = xQueueCreate(1, sizeof(bool));
    greenQ = xQueueCreate(1, sizeof(bool));

    xTaskCreate(pumRed,"RED LED Task",64,NULL,1,NULL);
    xTaskCreate(pumGreen,"YELLOW LED Task",64,NULL,1,NULL);
    xTaskCreate(pumYellow,"GREEN LED Task",64,NULL,1,NULL);
    xTaskCreate(faiRed,"RED PUM Task",64,NULL,1,NULL);
    xTaskCreate(faiYellow,"YELLOW PUM Task",64,NULL,1,NULL);
    xTaskCreate(faiGreen,"GREEN PUM Task",64,NULL,1,NULL);
}
void pumRed(void *val)
{
    pinMode(PUM1, INPUT_PULLUP);
    unsigned long bounce = 0;
    while (1)
    {
        int pum = !digitalRead(PUM1);
        if (pum && millis() - bounce > 500)
        {
            bounce = millis();
            xQueueSend(redQ, NULL, 0);
        }
        vTaskDelay(5);
    }
}
void pumYellow(void *val)
{
    pinMode(PUM2, INPUT_PULLUP);
    bool lastPum = false;
    while (1)
    {
        int pum = !digitalRead(PUM2);
        if (!lastPum && pum)
        {
            xQueueSend(yellowQ, NULL, 0);
        }
        lastPum = pum;
    }
}
void pumGreen(void *val)
{
    pinMode(PUM3, INPUT_PULLUP);
    bool lastPum = false;
    while (1)
    {
        int pum = !digitalRead(PUM3);
        if (!lastPum && pum)
        {
            xQueueSend(greenQ, NULL, 0);
        }
        lastPum = pum;
    }
}
void faiRed(void *val)
{
    pinMode(FAIRED, OUTPUT);
    while (1)
    {
        int qStatus = xQueueReceive(redQ, NULL,pdMS_TO_TICKS(3000));//xTaskใช้เวลาdelayจากCPUซึ่งแต่ละเครื่องไม่เท่ากัน แต่delayคือเวลาของโปรแกรทดังนั้pdMSมันเอาไปบวกลบอะไรกันไม่รู้ทำให้เวลามันเท่ากัน
        if (qStatus == pdPASS)
        {
            digitalWrite(FAIRED, HIGH);
        }
        else
        {
            digitalWrite(FAIRED, LOW);
            vTaskDelay(10);
        }
    }
}
void faiYellow(void *val)
{
    pinMode(FAIYELLOW, OUTPUT);
    bool state = false;
    while (1)
    {
        int qStatus = xQueueReceive(yellowQ, NULL, 0);
        if (qStatus)
        {
            state = !state;
        }
        if (state)
        {
            digitalWrite(FAIYELLOW, HIGH);
            vTaskDelay(pdMS_TO_TICKS(500));
            digitalWrite(FAIYELLOW, LOW);
            vTaskDelay(pdMS_TO_TICKS(500));
        }
        else
        {
            digitalWrite(FAIYELLOW, LOW);
            vTaskDelay(pdMS_TO_TICKS(500));
        }
    }
}
void faiGreen(void *val)
{
    pinMode(FAIGREEN, OUTPUT);
    while (1)
    {
        int qStatus = xQueueReceive(greenQ, NULL, 0);
        if (qStatus)
        {
            xQueueSend(greenQ, NULL, 0);
            for (int i = 0; i < 3; i++)
            {
                digitalWrite(FAIGREEN, HIGH);
                vTaskDelay(pdMS_TO_TICKS(500));
                digitalWrite(FAIGREEN, LOW);
                vTaskDelay(pdMS_TO_TICKS(500));
            }
            xQueueReceive(greenQ,NULL,0);
        }
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}
void loop()
{
}
