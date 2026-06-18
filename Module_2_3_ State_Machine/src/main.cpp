#include <Arduino.h>

const uint8_t LED1_PIN = LED_BUILTIN;
const uint8_t LED2_PIN = 16;

const uint32_t LED1_PERIOD = 500;
const uint32_t LED2_PERIOD = 1000;

enum LedState
{
  LED_OFF,
  LED_ON
};

LedState led1State = LED_OFF;
LedState led2State = LED_OFF;

uint32_t lastLed1Timer = 0;
uint32_t lastLed2Timer = 0;

void updateLED1();
void updateLED2();

void setup()
{
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
}

void loop()
{
  updateLED1();
  updateLED2();
}

// Машина станів LED1
void updateLED1()
{
  uint32_t currentTime = millis();

  if (currentTime - lastLed1Timer >= LED1_PERIOD)
  {
    lastLed1Timer = currentTime;

    switch (led1State)
    {

    case LED_OFF:

      led1State = LED_ON;
      digitalWrite(LED1_PIN, HIGH);

      break;

    case LED_ON:

      led1State = LED_OFF;
      digitalWrite(LED1_PIN, LOW);

      break;
    }
  }
}

// Машина станів LED2
void updateLED2()
{
  uint32_t currentTime = millis();

  if (currentTime - lastLed2Timer >= LED2_PERIOD)
  {
    lastLed2Timer = currentTime;

    switch (led2State)
    {

    case LED_OFF:

      led2State = LED_ON;
      digitalWrite(LED2_PIN, HIGH);

      break;

    case LED_ON:

      led2State = LED_OFF;
      digitalWrite(LED2_PIN, LOW);

      break;
    }
  }
}