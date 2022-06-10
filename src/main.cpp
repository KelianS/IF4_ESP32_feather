//Kélian SERMET
//08/06/2022
//http://insa.pages.univ-lyon1.fr/freertos/activites/tp1/


#include <Arduino.h>
#define BP_S1 digitalPinToInterrupt(GPIO_NUM_14)
#define LED_RED 16
#define LED_GREEN 19
#define LED_BLUE 4


#if CONFIG_FREERTOS_UNICORE
#define APP_CORE 0
#else
#define APP_CORE (portNUM_PROCESSORS - 1)
#endif



/***** PROTOTYPE *****/
void LED_blink(void *pvParameters);
void dumb_delay(uint32_t count);


/***** GLOBAL *****/
  //LED list
const uint8_t leds[] = {LED_RED, LED_GREEN, LED_BLUE};
  


void setup() {
  //Configure Serial
  Serial.begin(115200);
  while(!Serial);

  //RTOS Priority
  //Serial.print("Task : "); Serial.print(pcTaskGetName(NULL)); Serial.print(" - Coeur : ");  Serial.print(xPortGetCoreID()); Serial.print(" - Priority : "); Serial.println(uxTaskPriorityGet(NULL));


  xTaskCreatePinnedToCore(LED_blink,
                          "LED_RED",
                          2048,
                          (void*) &leds[0],
                          1,
                          NULL,
                          tskNO_AFFINITY);

  Serial.println(leds[2]);
  vTaskDelay(pdMS_TO_TICKS(1000));
  xTaskCreatePinnedToCore(LED_blink,
                          "LED_BLUE",
                          1024,
                          (void*) &leds[2],
                          1,
                          NULL,
                          tskNO_AFFINITY);
    

}

void loop() {
  //delay(2000);
  //Serial.print("Task : "); Serial.print(pcTaskGetName(NULL)); Serial.print(" - Coeur : ");  Serial.print(xPortGetCoreID()); Serial.print(" - Priority : "); Serial.println(uxTaskPriorityGet(NULL));
  
}



void LED_blink(void *pvParameters){
  //Serial.print("Task : "); Serial.print(pcTaskGetName(NULL)); Serial.print(" - Coeur : ");  Serial.print(xPortGetCoreID()); Serial.print(" - Priority : "); Serial.println(uxTaskPriorityGet(NULL));
  
  // Configure IOs
  uint8_t pin = *(uint8_t*)pvParameters;
  Serial.println(pin);
  pinMode(pin, OUTPUT);
  int LedState = LOW;

  for(;;){
    LedState = !LedState;
    digitalWrite(pin, LedState);
    //dumb_delay(1500);
    vTaskDelay(pdMS_TO_TICKS(1500));
  }
}



void dumb_delay(uint32_t count)
{
  volatile uint32_t cpt, i;
  
  for (i = 0; i < count; i++) {
    for (cpt = 0; cpt < 10000; cpt++) {
      
    }
  }
}