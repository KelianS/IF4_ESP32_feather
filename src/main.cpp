//Kélian SERMET
//08/06/2022
//http://insa.pages.univ-lyon1.fr/freertos/activites/tp1/


#include <Arduino.h>
#define BP_S1 digitalPinToInterrupt(GPIO_NUM_14)
#define LED_GREEN 19
#define LED_BLUE 4



#if CONFIG_FREERTOS_UNICORE
#define APP_CORE 0
#else
#define APP_CORE (portNUM_PROCESSORS - 1)
#endif



void LED_builtin_blink(void *pvParameters);
void LED_blue_blink(void *pvParameters);


void setup() {
  //Configure Serial
  Serial.begin(115200);
  while(!Serial);

  //RTOS Priority
  Serial.print("Task : "); Serial.print(pcTaskGetName(NULL)); Serial.print(" - Coeur : ");  Serial.print(xPortGetCoreID()); Serial.print(" - Priority : "); Serial.println(uxTaskPriorityGet(NULL));
  

  xTaskCreatePinnedToCore(LED_builtin_blink,
                          "LED_BUILTIN_TASK",
                          1024,
                          nullptr,
                          1,
                          NULL,
                          1);

  xTaskCreatePinnedToCore(LED_blue_blink,
                          "LED_BLUE_TASK",
                          1024,
                          nullptr,
                          1,
                          NULL,
                          1);

}

void loop() {
  delay(2000);
  Serial.print("Task : "); Serial.print(pcTaskGetName(NULL)); Serial.print(" - Coeur : ");  Serial.print(xPortGetCoreID()); Serial.print(" - Priority : "); Serial.println(uxTaskPriorityGet(NULL));
  
}




void LED_builtin_blink(void *pvParameters){
  Serial.print("Task : "); Serial.print(pcTaskGetName(NULL)); Serial.print(" - Coeur : ");  Serial.print(xPortGetCoreID()); Serial.print(" - Priority : "); Serial.println(uxTaskPriorityGet(NULL));

  // Configure IOs
  pinMode(LED_BUILTIN, OUTPUT);
  int LedState = LOW;

  for(;;){
    LedState = !LedState;
    digitalWrite(LED_BUILTIN, LedState);
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

void LED_blue_blink(void *pvParameters){
  Serial.print("Task : "); Serial.print(pcTaskGetName(NULL)); Serial.print(" - Coeur : ");  Serial.print(xPortGetCoreID()); Serial.print(" - Priority : "); Serial.println(uxTaskPriorityGet(NULL));
  
  // Configure IOs
  pinMode(LED_BLUE, OUTPUT);
  int LedStateBlue = LOW;

  for(;;){
    LedStateBlue = !LedStateBlue;
    digitalWrite(LED_BLUE, LedStateBlue);
    vTaskDelay(pdMS_TO_TICKS(1500));
  }
}