#include <Arduino.h>

const uint8_t BUTTON_PIN = 0;
const uint8_t LED_PIN = LED_BUILTIN;

volatile bool buttonPressed = false;

uint32_t ledStartTime = 0;

bool ledState = false;

const uint32_t LED_TIME = 1000;

// Обробник переривання
void IRAM_ATTR reaction_left()
{
  buttonPressed = true;
}

void setup()
{
  pinMode(LED_PIN, OUTPUT);

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  attachInterrupt(
      digitalPinToInterrupt(BUTTON_PIN),
      reaction_left,
      FALLING);
}

void loop()
{

  // Подія прийшла з Interrupt
  if (buttonPressed)
  {
    buttonPressed = false;

    digitalWrite(LED_PIN, HIGH);

    ledStartTime = millis();

    ledState = true;
  }

  // Non-blocking Timing
  if (ledState &&
      (millis() - ledStartTime >= LED_TIME))
  {
    digitalWrite(LED_PIN, LOW);

    ledState = false;
  }
}