#include <Arduino.h>

class Config
{
public:
  // Пін LED
  static constexpr uint8_t LED_PIN = LED_BUILTIN;

  // Період блимання (мс)
  static constexpr uint32_t BLINK_PERIOD_MS = 500;
};

enum class LedState
{
  Off,
  On
};

class Led
{
public:
  explicit Led(uint8_t pin) : pin_(pin) {}

  void init()
  {
    pinMode(pin_, OUTPUT);
    set(LedState::Off);
  }

  void set(LedState state)
  {
    state_ = state;

    digitalWrite(
        pin_,
        state == LedState::On ? HIGH : LOW);
  }

  LedState state() const
  {
    return state_;
  }

private:
  uint8_t pin_;
  LedState state_{LedState::Off};
};

// Константи часу компіляції
// constexpr uint8_t LED_PIN = LED_BUILTIN;
// constexpr uint32_t BLINK_PERIOD_MS = 500;

// Об'єкт світлодіода
Led led(Config::LED_PIN);

void setup()
{
  led.init();
}

void loop()
{
  static uint32_t lastToggleTime = 0;

  uint32_t currentTime = millis();

  if ((currentTime - lastToggleTime) >= Config::BLINK_PERIOD_MS)
  {
    lastToggleTime = currentTime;

    if (led.state() == LedState::Off)
    {
      led.set(LedState::On);
    }
    else
    {
      led.set(LedState::Off);
    }
  }

  // Тут можуть працювати інші задачі
}