#include <Arduino.h>
#include <esp_sleep.h>

/*  +--> setup()
    +--> Робота 10s
    +--> Deep Sleep 10s
    +--> Wake Up
    +--> Repeat setup()
*/

// Визначення пінів
const uint8_t LED_PIN_16 = 16;

const uint8_t WORK_TIME_SEC = 10;
const uint8_t SLEEP_TIME_SEC = 10;

// Вказівники на таймери
hw_timer_t *My_timer_16 = NULL;

// Обробник переривання для таймера 16
void IRAM_ATTR onTimer_16()
{
  digitalWrite(LED_PIN_16, !digitalRead(LED_PIN_16));
}

void setup()
{
  // ініціалізацію серійного порту
  Serial.begin(115200);
  delay(1000);

  Serial.println("ESP32-S3 Timer RTC");

  // Ініціалізація пінів для світлодіодів режим - OUT
  pinMode(LED_PIN_16, OUTPUT);

  // Ініціалізація таймера: (номер, преддільник, рахувати вгору)
  My_timer_16 = timerBegin(0, 80, true);

  // Прив’язуємо обробник переривання до таймера
  timerAttachInterrupt(My_timer_16, &onTimer_16, true);

  // Налаштовуємо таймер: кожні 500 000 мкс = 0,5 секунди
  timerAlarmWrite(My_timer_16, 500000, true);

  // Вмикаємо таймери
  timerAlarmEnable(My_timer_16); // Just Enable

  uint32_t startTime = millis();

  // Працюємо 20 секунд
  while ((millis() - startTime) < WORK_TIME_SEC * 1000)
  {
    // Працюємо датчик пристрою
  }

  Serial.println("Go to Deep Sleep");

  // Прокинутися через 20 секунд
  esp_sleep_enable_timer_wakeup(
      SLEEP_TIME_SEC * 1000000ULL);

  // Перейти в режим Deep Sleep
  esp_deep_sleep_start();
}

void loop()
{
}