//Kélian SERMET
//10/06/2022
//http://insa.pages.univ-lyon1.fr/freertos/activites/tp3/


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


// Buttons
#define  BP_1_PIN    14
#define  BP_2_PIN    15

//Queue
static QueueHandle_t queue;



static void DebounceTask(void *pvParameters) {
  uint32_t level, state = 0, last = 1;
  uint32_t mask = 0x7FFFFFFF;
  bool event;
  Serial.println("Debounce TASK start");

  for (;;) {
    level = digitalRead(BP_1_PIN);
    state = (state << 1) | level;
    if (((state & mask) == mask) || ((state & mask) == 0)) {
      if (level != last) {
        event = !level;  // true when pressed
        if (uxQueueSpacesAvailable(queue)!=0) {       //send event to Queue and only modify last level if the event could be sent
          xQueueSendToBack(queue, (void *)&event, portMAX_DELAY);
          last = level;
        }
      }
    }
    vTaskDelay(pdMS_TO_TICKS(2));
  }
}

static void LedTask(void *pvParameters) {
  bool event, led = false;
  Serial.println("Led TASK start");

  digitalWrite(LED_BUILTIN, led);

  for (;;) {
    xQueueReceive(queue, (void *)&event, portMAX_DELAY);     //Read from queue
    if (event) {
      led ^= true; //led = led 'OU EXCLUSIF' TRUE   (ou led = !led)
      digitalWrite(LED_BUILTIN, led);
    } 
    else {
      for(int i=1;i<=10;i++){ //Flash 10 pulses on Blue LED
        digitalWrite(LED_BLUE,HIGH);
        vTaskDelay(pdMS_TO_TICKS(50));
        digitalWrite(LED_BLUE,LOW);
        vTaskDelay(pdMS_TO_TICKS(200));
      }    
    }
  }
}

void setup() {

  Serial.begin(115200);
  while(!Serial);
  Serial.println("BEGIN");
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(BP_1_PIN, INPUT);

  //...         //Creation de la file
  queue = xQueueCreate(10, sizeof(bool));

  xTaskCreatePinnedToCore(DebounceTask,  // Function
                          "Debounce",    // Name
                          8192,          // Stack size
                          nullptr,       // Parameters
                          2,             // Priority
                          nullptr,       // Task handle
                          APP_CORE);     // Core

  xTaskCreatePinnedToCore(LedTask,       // Function
                          "Toggle Led",  // Name
                          8192,          // Stack size
                          nullptr,       // Parameters
                          1,             // Priority
                          nullptr,       // Task handle
                          APP_CORE);     // Core
}

void loop() {
  // Tout est fait dans les tâches
  vTaskDelete(nullptr);
}