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

void setup()
{
  // Ініціалізація пінів для світлодіодів як виходи
  pinMode(LED_15, OUTPUT);
  pinMode(LED_16, OUTPUT);
  pinMode(LED_17, OUTPUT);
  pinMode(LED_18, OUTPUT);

  // Ініціалізація таймера: (номер, преддільник, рахувати вгору)
  My_timer_15 = timerBegin(0, 80, true);
  My_timer_16 = timerBegin(1, 80, true);
  My_timer_17 = timerBegin(2, 80, true);
  My_timer_18 = timerBegin(3, 80, true);

  // Прив’язуємо обробник переривання до таймера
  timerAttachInterrupt(My_timer_15, &onTimer_15, true);
  timerAttachInterrupt(My_timer_16, &onTimer_16, true);
  timerAttachInterrupt(My_timer_17, &onTimer_17, true);
  timerAttachInterrupt(My_timer_18, &onTimer_18, true);

  // Налаштовуємо таймер: кожні 1 000 000 мкс = 1 секунда
  timerAlarmWrite(My_timer_15, 500000, true);
  timerAlarmWrite(My_timer_16, 1000000, true);
  timerAlarmWrite(My_timer_17, 2000000, true);
  timerAlarmWrite(My_timer_18, 4000000, true);

  // Вмикаємо таймери
  timerAlarmEnable(My_timer_15); // Just Enable
  timerAlarmEnable(My_timer_16); // Just Enable
  timerAlarmEnable(My_timer_17); // Just Enable
  timerAlarmEnable(My_timer_18); // Just Enable
}
void loop()
{
}