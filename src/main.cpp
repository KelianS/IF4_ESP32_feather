#include <Arduino.h>

void setup() {
  // Configure IOs
  pinMode(LED_BUILTIN, OUTPUT);

  //Configure Serial
  Serial.begin(115200);
  while(!Serial);
}

void loop() {

  //Serial Hello World
  Serial.println("Hello");
  delay(3000);

  // Blink Led
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);
}