#include <Arduino.h>

// Піни для сегментів індикатора (A, B, C, D, E, F, G)
const uint8_t segmentPins[7] = {5, 6, 7, 15, 16, 17, 18};

// Пін для кнопки
const uint8_t BUTTON_PIN = 0;

uint32_t pressCount = 0;
bool lastButtonState = HIGH;

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

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Виводимо початковий 0
  displayDigit(pressCount);
}

void loop()
{
  bool currentButtonState = digitalRead(BUTTON_PIN);

  // Перевірка натискання кнопки (було HIGH, стало LOW)
  if (lastButtonState == HIGH && currentButtonState == LOW)
  {
    pressCount++;
    if (pressCount > 9)
    {
      pressCount = 0; // Скидаємо після 9
    }

    // Відображення лічильник
    displayDigit(pressCount);

    Serial.print("Count = ");
    Serial.println(pressCount);

    // Захисту від брязкоту контактів (дебаунс)
    delay(50);
  }

  lastButtonState = currentButtonState;
}

// функція виведення цифри
void displayDigit(int digit)
{
  for (int i = 0; i < 7; i++)
  {

    digitalWrite(segmentPins[i], !digits[digit][i]);
  }
}