#include <Arduino.h>
#include <vector>

std::vector<int> values;

void printMemoryInfo()
{
  Serial.println("\n=== ESP32-S3 memory info ===");

  // SRAM / heap
  Serial.printf("Heap total: %.2f MB\n", ESP.getHeapSize() / 1048576.0f);
  Serial.printf("Heap free:  %.2f MB\n", ESP.getFreeHeap() / 1048576.0f);

  Serial.println("==========");
  // PSRAM
  if (ESP.getPsramSize() > 0)
  {
    Serial.printf("PSRAM total: %.2f MB\n", ESP.getPsramSize() / 1048576.0f);
    Serial.printf("PSRAM free:  %.2f MB\n", ESP.getFreePsram() / 1048576.0f);
  }
  else
  {
    Serial.println("PSRAM not available or not enabled");
  }

  Serial.println("==========");
  // Flash / sketch space
  Serial.printf("Sketch size: %.2f MB\n", ESP.getSketchSize() / 1048576.0f);
  Serial.printf("Free sketch space: %.2f MB\n", ESP.getFreeSketchSpace() / 1048576.0f);
  Serial.printf("Flash size: %.2f MB\n", ESP.getFlashChipSize() / 1048576.0f);

  Serial.println("===========================");
}

void setup()
{
  Serial.begin(115200);
  delay(1000);

  printMemoryInfo();

  // Виділити буфер у PSRAM
  uint8_t *psramBuf = (uint8_t *)heap_caps_malloc(
      1024 * 1024 * 4, MALLOC_CAP_SPIRAM);

  if (psramBuf == nullptr)
  {
    Serial.println("\nПам'ять не виділина!");
  }
  else
  {
    Serial.println("\n4 MB PSRAM виділено");
  }

  printMemoryInfo();

  //   // Виділити буфер у SRAM
  //   uint8_t *sramBuf = (uint8_t *)heap_caps_malloc(
  //       1024 * 100, MALLOC_CAP_INTERNAL);

  //   if (sramBuf == nullptr)
  //   {
  //     Serial.println("\nПам'ять не виділина!");
  //   }
  //   else
  //   {
  //     Serial.println("\n100 KB SRAM виділено");
  //   }

  //   printMemoryInfo();

  //   // Виділити буфер у SRAM для DMA
  //   uint8_t *dmaBuf = (uint8_t *)heap_caps_malloc(
  //       1024 * 100, MALLOC_CAP_DMA);

  //   if (dmaBuf == nullptr)
  //   {
  //     Serial.println("\nПам'ять не виділина!");
  //   }
  //   else
  //   {
  //     Serial.println("\n100 KB DMA виділено");
  //   }

  //   printMemoryInfo();

  //   // Виділити пам'ять у PSRAM для вектора
  //   values.reserve(12800);
  //   values.push_back(42);
  //   Serial.println("\n50 KB vector виділено");

  //   printMemoryInfo();
}

void loop()
{
  delay(5000);
}