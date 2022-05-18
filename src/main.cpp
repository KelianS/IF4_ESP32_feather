#include <Arduino.h>
#define BP_S1 digitalPinToInterrupt(GPIO_NUM_14)

//Global variables
int LedState = LOW;
TickType_t LastInterruptTick = 0;

void IRAM_ATTR my_isr();

void setup() {
  // Configure IOs
  pinMode(LED_BUILTIN, OUTPUT);

  // Configure Interrupt
  attachInterrupt(BP_S1, my_isr, FALLING);

  //Configure Serial
  Serial.begin(115200);
  while(!Serial);
}

void loop() {

  //Serial Hello World
  //Serial.println("Hello");
  delay(3000);

}


void IRAM_ATTR my_isr() {
  TickType_t tickInISR = xTaskGetTickCountFromISR();
  
  if (pdTICKS_TO_MS(tickInISR) > pdTICKS_TO_MS(LastInterruptTick)+200) {
    LastInterruptTick = tickInISR;
    LedState = !LedState;
    digitalWrite(LED_BUILTIN, LedState);
    //Serial.println(LedState); //Degub
  }
}