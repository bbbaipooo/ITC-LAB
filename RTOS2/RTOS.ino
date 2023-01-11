#include <Arduino_FreeRTOS.h>
#include "semphr.h"
#include "queue.h"
#include "task.h"

#define FAIRED 6
#define FAIYELLOW 7
#define FAIGREEN 8

#define PUMRED 3
#define PUMYELLOW 4
#define PUMGREEN 5

int redstate = 0;
int greenstate = 0;

unsigned long redtime = 0;
unsigned long yellowtime = 0;
unsigned long greentime = 0;

unsigned long redDebounce = 0;
unsigned long yellowDebounce = 0;
unsigned long greenDebounce = 0;
QueueHandle_t ledQueue;
SemaphoreHandle_t xBinarySemaphore[3];

void setup() {
  Serial.begin(9600);
  ledQueue = xQueueCreate(10, sizeof(int32_t));
  xTaskCreate(sentTask, "PUM RED", 100, PUMRED, 1, NULL);
  xTaskCreate(sentTask, "PUM YELLOW", 100, PUMYELLOW , 1, NULL);
  xTaskCreate(sentTask, "PUM GREEN", 100, PUMGREEN, 1, NULL);

  xTaskCreate(faired,    "Red ",    100, NULL, 1, NULL);
  xTaskCreate(faiyellow, "Yellow ", 100, NULL, 1, NULL);
  xTaskCreate(faigreen,  "Green ",  100, NULL, 1, NULL);

  for (int i = 0; i < 3; i++) {
    xBinarySemaphore[i] = xSemaphoreCreateBinary();
    xSemaphoreGive(xBinarySemaphore[i]);
  }
}

void sentTask(void *pvParameters) {
  BaseType_t qStatus;
  int32_t valueToSend = 0;
  int32_t button = (int32_t) pvParameters;
  pinMode(button, INPUT);
  while (1) {
    if (digitalRead(button)) {
      valueToSend = button;
      qStatus = xQueueSend(ledQueue, &valueToSend, 0);
      vTaskDelay(1);
    }
  }
}

void faired(void* pvParameters) {
  int32_t valueReceived;
  pinMode(FAIRED, OUTPUT);
  BaseType_t qStatus;
  const TickType_t xTicksToWait = pdMS_TO_TICKS(100);

  while (1) {
    qStatus = xQueueReceive(ledQueue, &valueReceived, xTicksToWait);
    if (qStatus == pdPASS) {
      if (valueReceived == PUMRED  && millis() - redDebounce >= 500) {
        redDebounce = millis();
        if (xSemaphoreTake(xBinarySemaphore[0], 1) == pdTRUE) {
          redtime = millis();
          redstate = 1;
        } else {
          redstate = 0;
        }
      }
    }
    gotored(redstate);
  }
}

void faiyellow(void* pvParameters) {
  int32_t valueReceived;
  pinMode(FAIYELLOW, OUTPUT);
  BaseType_t qStatus;
  const TickType_t xTicksToWait = pdMS_TO_TICKS(100);

  while (1) {
    qStatus = xQueueReceive(ledQueue, &valueReceived, xTicksToWait);
    if (qStatus == pdPASS) {
      if (valueReceived == PUMYELLOW   && millis() - yellowDebounce >= 500) {
        yellowDebounce = millis();
        if (xSemaphoreTake(xBinarySemaphore[0], 1) == pdTRUE) {
          xSemaphoreGive(xBinarySemaphore[0]);
          if (xSemaphoreTake(xBinarySemaphore[2], 1) == pdTRUE) {
            xSemaphoreGive(xBinarySemaphore[2]);
            if (xSemaphoreTake(xBinarySemaphore[1], 1) == pdTRUE) {
              gotoyellow(1);
            }
          }
        }
      }
    }
  }
}

void faigreen(void* pvParameters) {
  int32_t valueReceived;
  pinMode(FAIGREEN, OUTPUT);
  BaseType_t qStatus;
  const TickType_t xTicksToWait = pdMS_TO_TICKS(100);

  while (1) {
    qStatus = xQueueReceive(ledQueue, &valueReceived, xTicksToWait);
    if (qStatus == pdPASS) {
      if (valueReceived == PUMGREEN  && millis() - greenDebounce >= 500) {
        greenDebounce = millis();
        if (xSemaphoreTake(xBinarySemaphore[0], 1) == pdTRUE) {
          xSemaphoreGive(xBinarySemaphore[0]);
          if (xSemaphoreTake(xBinarySemaphore[2], 1) == pdTRUE) {
            greentime = millis();
            greenstate = 1; 
          } else {
            greenstate = 0; 
          }
        }
      }
    }
    gotogreen(greenstate);
  }
}

void loop() {}

void gotored(int status) {
  if (status) {
    digitalWrite(FAIRED, HIGH);
    if (millis() - redtime >= 3000) {
      digitalWrite(FAIRED, LOW);
      redstate = 0;
      xSemaphoreGive(xBinarySemaphore[0]);
    }
  } else { 
    digitalWrite(FAIRED, LOW);
    xSemaphoreGive(xBinarySemaphore[0]); 
  }
}

void gotoyellow(int status) {
  if (status) {
    int i = 0;
    while (i < 4) {
      if (millis() - yellowtime >= 500) {
        digitalWrite(FAIYELLOW, !digitalRead(FAIYELLOW) ^ 0);
        i++;
        yellowtime = millis();
      }
    }
    xSemaphoreGive(xBinarySemaphore[1]);
  }
}

void gotogreen(int status) {
  if ( status) {
    digitalWrite(FAIGREEN, HIGH);
    if (millis() - greentime >= 3000) {
      digitalWrite(FAIGREEN, LOW);
      greenstate = 0;
      xSemaphoreGive(xBinarySemaphore[2]);
    }
  } else { 
    digitalWrite(FAIGREEN, LOW);
    xSemaphoreGive(xBinarySemaphore[2]);
  }
}
