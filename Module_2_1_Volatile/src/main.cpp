#include <Arduino.h>

#define BUTTON_PIN 0 // GPIO0 (button, use INPUT_PULLUP)
#define LED_PIN 16   // GPIO16 (LED)

#define BLINK_DELAY_MS 500

// volatile bool button_pressed = false;
bool button_pressed = false;

void IRAM_ATTR button_isr()
{
  button_pressed = true;
}

void setup()
{
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN),
                  button_isr,
                  FALLING);
}

void loop()
{
  while (!button_pressed)
  {
  }

  digitalWrite(LED_PIN, HIGH);
  delay(BLINK_DELAY_MS);

  digitalWrite(LED_PIN, LOW);
  delay(BLINK_DELAY_MS);
}