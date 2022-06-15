// Kélian SERMET
// 15/06/2022
// http://insa.pages.univ-lyon1.fr/freertos/activites/tp4/

#include <Arduino.h>

#if CONFIG_FREERTOS_UNICORE
#define APP_CORE 0
#else
#define APP_CORE (portNUM_PROCESSORS - 1)
#endif

// Prototypes
void TalkingTask(void *pvParameters);

SemaphoreHandle_t xSemaphore;

void setup() {
  Serial.begin(1200);
  while (!Serial)
    ;

  xSemaphore = xSemaphoreCreateMutex();

  xTaskCreatePinnedToCore(TalkingTask,  // Function
                          "Talking 1",  // Name
                          8192,         // Stack size
                          (void *)1,    // Parameters
                          6,            // Priority
                          NULL,         // Task handle
                          APP_CORE);    // Core

  xTaskCreatePinnedToCore(TalkingTask,  // Function
                          "Talking 2",  // Name
                          8192,         // Stack size
                          (void *)2,    // Parameters
                          5,            // Priority
                          NULL,         // Task handle
                          APP_CORE);    // Core
}

void loop() {
  // Tout est fait dans les tâches
  vTaskDelete(nullptr);
}

void TalkingTask(void *pvParameters) {
  BaseType_t taskNb = (BaseType_t)pvParameters;
  UBaseType_t counter = (taskNb == 1) ? 0 : 10;;

  Serial.printf("Starting task %ld", taskNb);

  for (;;) {
    vTaskDelay(taskNb*50);

    xSemaphoreTake(xSemaphore,portMAX_DELAY);

    Serial.println("**********************************************************");
    Serial.printf("%s - Coeur : %ld - Priority %lu\n", pcTaskGetName(nullptr),
                  xPortGetCoreID(), uxTaskPriorityGet(nullptr));
    Serial.printf("Task %ld\n", taskNb);
    Serial.printf("Valeur %lu\n", counter);

    xSemaphoreGive(xSemaphore);
    
    counter += taskNb; 
  }
}