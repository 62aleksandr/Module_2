#include <Arduino.h>

// Піни для сегментів індикатора (A, B, C, D, E, F, G)
const uint8_t segmentPins[7] = {5, 6, 7, 15, 16, 17, 18};

const uint8_t BUTTON_PIN = 0;

volatile bool buttonPressed = false;
unsigned long lastInterruptTime = 0;

// Лічильник натискань кнопки
uint32_t pressCount = 0;

// Час антидребезгу (мс)
const uint8_t debounceDelay = 50;

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

void IRAM_ATTR handleButtonPress()
{
  unsigned long currentTime = millis();

  // Ігноруємо переривання, що надходять занадто часто (брязкіт)
  if (currentTime - lastInterruptTime > debounceDelay)
  {
    lastInterruptTime = currentTime;
    buttonPressed = true;
  }
}

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

  // Кнопка підключена PULLUP
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN),
                  handleButtonPress,
                  FALLING);

  // Виводимо цифру 0
  displayDigit(pressCount);
}

void loop()
{
  if (buttonPressed)
  {
    buttonPressed = false;

    pressCount++;

    if (pressCount > 9)
    {
      pressCount = 0; // Скидаємо після 9
    }

    // Відображення лічильник
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