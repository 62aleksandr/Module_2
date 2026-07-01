#include <Arduino.h>

// Визначення пінів
const int potPin = 4;  //  Пін ADC для потенціометра
const int ledPin = 16; // Пін для світлодіода (підключений до ШИМ)

// Ініціалізація PWM (LEDC)
const int pwmChannel = 0;    // Віртуальний канал  PWM
const int pwmFreq = 1000;    // Частота   PWM: 5 кГц
const int pwmResolution = 8; // Разрядність: 8 біт (0...255)

// Вказівники на таймери
hw_timer_t *adcTimer = NULL;

// Змінні для зберігання стану та значень
volatile bool readyToRead = false;
int potValue = 0;

// Обробник переривання для таймера(ISR)
void IRAM_ATTR onTimer()
{
  readyToRead = true;
}

void setup()
{
  // Встановлення розширення ADC в 8 бит (0...255)
  analogReadResolution(pwmResolution);

  // Ініціалізуємо канал PWM з частотою та розширенням
  ledcSetup(pwmChannel, pwmFreq, pwmResolution);

  // Прив'язуємо цей канал до конкретного піну світлodіода
  ledcAttachPin(ledPin, pwmChannel);

  // Налаштування таймера: номер таймера (0), дільник (80 -> 1 тик = 1 мкс), рахунок вгору (true)
  adcTimer = timerBegin(0, 80, true);

  // Привязка переривання
  timerAttachInterrupt(adcTimer, &onTimer, true);

  // Налаштування порога спрацьовування: 20000 тиков = 20 мс
  timerAlarmWrite(adcTimer, 20000, true);

  // Вмикаємо таймер
  timerAlarmEnable(adcTimer);
}

void loop()
{
  if (readyToRead)
  {
    readyToRead = false;

    potValue = analogRead(potPin);

    // Запис PWM
    ledcWrite(pwmChannel, potValue);
  }
}
