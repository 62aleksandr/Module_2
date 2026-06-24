#include <Arduino.h>
#include <esp_task_wdt.h>

// Налашиваем таймер WDT 2 секунди
const uint8_t WDT_TIMEOUT_S = 2;

void setup()
{
  Serial.begin(115200);
  delay(1000);

  // Ініціалізація WDT з таймаутом 2 секунди
  esp_task_wdt_init(WDT_TIMEOUT_S, true);

  // Додавання поточної задачі під контроль WDT
  esp_task_wdt_add(NULL);

  Serial.println("Watchdog налаштовано на 2 секунди");
}

void loop()
{
  // === ТЕСТ 1: Затримка < WDT_TIMEOUT_S ===
  Serial.println("\n>>> Тест 1: Затримка < WDT_TIMEOUT_S");
  for (int i = 1; i <= 5; i++)
  {
    Serial.print("\nШаг ");
    Serial.print(i);
    Serial.print(" (Затримка 400мс)...");

    delay(400);

    // Скидання лічильника WDT
    Serial.print("  -  Скидання лічильника WDT");
    esp_task_wdt_reset();
  }

  // === ТЕСТ 2: Затримка > WDT_TIMEOUT_S ===
  Serial.println("\n\n>>> Тест 2: Затримка > WDT_TIMEOUT_S");

  for (int i = 1; i <= 5; i++)
  {
    Serial.print("\nШаг ");
    Serial.print(i);
    Serial.print(" (Затримка 1000мс)...");

    delay(1000);

    if (i == 2)
    {
      // Скидання лічильника WDT
      Serial.print("  -  Скидання лічильника WDT");
      esp_task_wdt_reset();
    }
  }
}
