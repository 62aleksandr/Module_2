#include <Arduino.h>

const uint8_t LED1_PIN = LED_BUILTIN;
const uint8_t LED2_PIN = 16;

const uint32_t LED1_PERIOD = 200;
const uint32_t LED2_PERIOD = 500;

void pollLED1();
void pollLED2();

void setup()
{
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
}

void loop()
{
  pollLED1();
  pollLED2();
}

// Polling LED1
void pollLED1()
{
  static uint32_t lastTime = 0;
  static bool ledState = false;

  uint32_t currentTime = millis();

  if (currentTime - lastTime >= LED1_PERIOD)
  {
    lastTime = currentTime;

    ledState = !ledState;
    digitalWrite(LED1_PIN, ledState);
  }
}

// Polling LED2
void pollLED2()
{
  static uint32_t lastTime = 0;
  static bool ledState = false;

  uint32_t currentTime = millis();

  if (currentTime - lastTime >= LED2_PERIOD)
  {
    lastTime = currentTime;

    ledState = !ledState;
    digitalWrite(LED2_PIN, ledState);
  }
}