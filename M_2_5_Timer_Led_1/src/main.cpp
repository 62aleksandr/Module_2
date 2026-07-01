#include <Arduino.h>

// Визначення пінів для світлодіодів
const uint8_t LED_15 = 15;
const uint8_t LED_16 = 16;
const uint8_t LED_17 = 17;
const uint8_t LED_18 = 18;

// Вказівники на таймери
hw_timer_t *My_timer_15 = NULL;
hw_timer_t *My_timer_16 = NULL;
hw_timer_t *My_timer_17 = NULL;
hw_timer_t *My_timer_18 = NULL;

// Обробник переривання для таймера 15
void IRAM_ATTR onTimer_15()
{
  digitalWrite(LED_15, !digitalRead(LED_15));
}
// Обробник переривання для таймера 16
void IRAM_ATTR onTimer_16()
{
  digitalWrite(LED_16, !digitalRead(LED_16));
}
// Обробник переривання для таймера 17
void IRAM_ATTR onTimer_17()
{
  digitalWrite(LED_17, !digitalRead(LED_17));
}
// Обробник переривання для таймера 18
void IRAM_ATTR onTimer_18()
{
  digitalWrite(LED_18, !digitalRead(LED_18));
}

// Creates and configures one hardware timer for periodic callbacks.
hw_timer_t *setupTimer(uint8_t num, uint16_t time, void (*callback)())
{
  hw_timer_t *timer = timerBegin(num, 80, true); // 80 prescaler for 1 MHz timer frequency (1 tick = 1 microsecond).
  // timerStop(timer);
  timerAlarmWrite(timer, time * 1000000, true); // Convert seconds to microseconds for the alarm value.
  timerAttachInterrupt(timer, callback, true);
  timerAlarmEnable(timer);
  return timer;
}

void setup()
{
  // Ініціалізація пінів для світлодіодів як виходи
  pinMode(LED_15, OUTPUT);
  pinMode(LED_16, OUTPUT);
  pinMode(LED_17, OUTPUT);
  pinMode(LED_18, OUTPUT);

  // Налаштовуємо таймерів
  My_timer_15 = setupTimer(0, 1, onTimer_15); // 1 сек
  My_timer_16 = setupTimer(1, 2, onTimer_16); // 2 сек
  My_timer_17 = setupTimer(2, 3, onTimer_17); // 3 сек
  My_timer_18 = setupTimer(3, 4, onTimer_18); // 4 сек
}
void loop()
{
}