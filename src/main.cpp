//Kélian SERMET
//10/06/2022
//http://insa.pages.univ-lyon1.fr/freertos/activites/tp2/

#include <Arduino.h>

#if CONFIG_FREERTOS_UNICORE
#define APP_CORE 0
#else
#define APP_CORE (portNUM_PROCESSORS - 1)
#endif

#ifndef LED_BUILTIN
#define LED_BUILTIN 13
#endif

// the number of the LED pins
const int LED_RED   = 16;  // RED LED corresponds to GPIO16
const int LED_GREEN = 19;  // GREEN LED corresponds to GPIO19
const int LED_BLUE  = 4;   // BLUE LED corresponds to GPIO4

// Prototypes
void TaskBlink( void *pvParameters );
void TaskBlinkPeriodic( void *pvParameters );


void setup() {

  Serial.begin(115200);
  while(!Serial);
  
  xTaskCreatePinnedToCore(
    TaskBlink,  //Function
    "Blink red",       //Name
    2048,               //Stack size
    (void*)&LED_RED,    //Parameters
    5,                  //Priority
    nullptr,            //Task handle
    APP_CORE);          //Core




  xTaskCreatePinnedToCore(
    TaskBlink,  //Function
    "Blink green",       //Name
    2048,               //Stack size
    (void*)&LED_GREEN,    //Parameters
    4,                  //Priority
    nullptr,            //Task handle
    APP_CORE);          //Core

    vTaskDelay(pdMS_TO_TICKS(1));

  xTaskCreatePinnedToCore(
    TaskBlink,  //Function
    "Blink blue",       //Name
    2048,               //Stack size
    (void*)&LED_BLUE,    //Parameters
    5,                  //Priority
    nullptr,            //Task handle
    APP_CORE);          //Core

}

void loop()
{
  // Tout est fait dans les tâches
  vTaskDelete(nullptr);
}


/*********** Analyse de l'ordonnancement Round Robin avec 3 taches ******************/
void TaskBlink(void *pvParameters) 
{
  bool shouldWait = true;
  //Retrieve Pin number
  uint8_t pin = *((uint8_t*)pvParameters);

  Serial.printf("Starting on : %hhd \n", pin);
  vTaskDelay(pdMS_TO_TICKS(2));

  pinMode(pin, OUTPUT);

  for (;;) {
    for (uint32_t i = 0; i < 40000; i++) {
      digitalWrite(pin, HIGH);
      digitalWrite(pin, LOW);
    }
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}






/*********** vTaskDelay Vs vTaskDelayUntil ******************/
void TaskBlinkPeriodic(void *pvParameters) 
{
  bool shouldWait = true;
  //Retrieve Pin number
  uint8_t pin = *((uint8_t*)pvParameters);
  pinMode(pin, OUTPUT);
  //vTaskDelayUntil()
  
  for (;;)
  {
    digitalWrite(pin, HIGH);
    vTaskDelay(pdMS_TO_TICKS(200));
    
    //Add jitter
    if(shouldWait) {
      vTaskDelay(pdMS_TO_TICKS(50));
      shouldWait = false;  
    } else {
      shouldWait = true;
    }
    digitalWrite(pin, LOW);
    vTaskDelay(pdMS_TO_TICKS(400));
  }
}