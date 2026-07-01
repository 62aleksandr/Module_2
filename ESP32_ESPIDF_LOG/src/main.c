#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

// Створюємо унікальний тег для нашого модуля
static const char *TAG = "MAIN_MODULE";

void app_main(void)
{
	printf("--- Демонстрація ESP_LOG для ESP32-S3 ---\n\n");

	// 1. Демонстрація всіх стандартних рівнів логування
	ESP_LOGE(TAG, "Це повідомлення про ПОМИЛКУ (Error)");
	ESP_LOGW(TAG, "Це ПОПЕРЕДЖЕННЯ (Warning)");
	ESP_LOGI(TAG, "Це ІНФОРМАЦІЙНЕ повідомлення (Info)");
	ESP_LOGD(TAG, "Це повідомлення для ВІДЛАДКИ (Debug)");
	ESP_LOGV(TAG, "Це ДЕТАЛЬНЕ повідомлення (Verbose)");

	vTaskDelay(pdMS_TO_TICKS(2000));

	// 2. Зміна рівня логування під час роботи програми (динамічно)
	printf("\n--- Змінюємо рівень логування лише до INFO ---\n");

	// Тепер повідомлення Debug та Verbose для цього тегу будуть ігноруватися
	esp_log_level_set(TAG, ESP_LOG_INFO);
	// esp_log_level_set(TAG, ESP_LOG_WARN);

	ESP_LOGE(TAG, "Error: все ще видно");
	ESP_LOGW(TAG, "Warning: все ще видно");
	ESP_LOGI(TAG, "Info: все ще видно");
	ESP_LOGD(TAG, "Debug: тепер НЕ ВИДНО");
	ESP_LOGV(TAG, "Verbose: тепер НЕ ВИДНО");

	// Нескінченний цикл, щоб програма не перезавантажувалася
	while (1)
	{
		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}