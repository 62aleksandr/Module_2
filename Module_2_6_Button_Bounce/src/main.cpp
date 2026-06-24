#include <Arduino.h>
#include <Bounce2.h>

// Піни для сегментів індикатора (A, B, C, D, E, F, G)
const uint8_t segmentPins[7] = {5, 6, 7, 15, 16, 17, 18};

const uint8_t BUTTON_PIN = 0;

// Час антидребезгу (мс)
const uint8_t debounceDelay = 30;

// Лічильник натискань кнопки
uint32_t pressCount = 0;

// Масив станів для цифр від 0 до 9
bool digits[10][7] = {
    {1, 1, 1, 1, 1, 1, 0}, // 0
    {0, 1, 1, 0, 0, 0, 0}, // 1
    {1, 1, 0, 1, 1, 0, 1}, // 2
    {1, 1, 1, 1, 0, 0, 1}, // 3
    {0, 1, 1, 0, 0, 1, 1}, // 4
    {1, 0, 1, 1, 0, 1, 1}, // 5
    {1, 0, 1, 1, 1, 1, 1}, // 6
    {1, 1, 1, 0, 0, 0, 0}, // 7
    {1, 1, 1, 1, 1, 1, 1}, // 8
    {1, 1, 1, 1, 0, 1, 1}  // 9
};

// Об'єкт антидребезгу Bounce2
Bounce button = Bounce();

void displayDigit(int digit);

void setup()
{
  Serial.begin(115200);
  delay(1000);

  // Налаштування пінів індикатора на вихід
  for (int i = 0; i < 7; i++)
  {
    pinMode(segmentPins[i], OUTPUT);
  }

  // Налаштування кнопки з підтягуванням
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Прив'язка Bounce2 до піна
  button.attach(BUTTON_PIN);

  // Час антидребезгу (30 мс як у тебе)
  button.interval(debounceDelay);

  // Виводимо цифру 0
  displayDigit(pressCount);
}

void loop()
{
  // Оновлення стану кнопки
  button.update();

  // Подія: натискання (перехід HIGH -> LOW)
  if (button.fell())
  {
    pressCount++;

    if (pressCount > 9)
    {
      pressCount = 0; // Скидаємо після 9
    }

    // Відображення лічильника
    displayDigit(pressCount);

    Serial.print("Count = ");
    Serial.println(pressCount);
  }
}

// функція виведення цифри
void displayDigit(int digit)
{
  for (int i = 0; i < 7; i++)
  {

    digitalWrite(segmentPins[i], !digits[digit][i]);
  }
}