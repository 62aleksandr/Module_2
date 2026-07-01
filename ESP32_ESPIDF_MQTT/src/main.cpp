/*

1. MQTT Bhai — мобільний MQTT-клієнт.

2. MQTT Explorer — клієнт MQTT для ПК.
   https://mqtt-explorer.com/

3. HiveMQ WebSocket Client — вебклієнт MQTT.
   https://www.hivemq.com/demos/websocket-client/

*/

#include <stdio.h>
#include <time.h>
#include "driver/gpio.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "mqtt.h"
#include "wifi_setup.h"

static const char TAG[] = "main";
static bool led_state = false;
constexpr gpio_num_t LED_GPIO = GPIO_NUM_16;

/* The examples use WiFi configuration that you can set via project configuration menu

   If you'd rather not, just change the below entries to strings with
   the config you want - ie #define EXAMPLE_WIFI_SSID "mywifissid"
*/
// #define ESP_WIFI_SSID "MagentaWLAN-LFDT"
// #define ESP_WIFI_PASS "82090897414409177139"
// #define ESP_MAXIMUM_RETRY 5

// #if CONFIG_ESP_WPA3_SAE_PWE_HUNT_AND_PECK
// #define ESP_WIFI_SAE_MODE WPA3_SAE_PWE_HUNT_AND_PECK
// #define EXAMPLE_H2E_IDENTIFIER ""
// #elif CONFIG_ESP_WPA3_SAE_PWE_HASH_TO_ELEMENT
// #define ESP_WIFI_SAE_MODE WPA3_SAE_PWE_HASH_TO_ELEMENT
// #define EXAMPLE_H2E_IDENTIFIER CONFIG_ESP_WIFI_PW_ID
// #elif CONFIG_ESP_WPA3_SAE_PWE_BOTH
// #define ESP_WIFI_SAE_MODE WPA3_SAE_PWE_BOTH
// #define EXAMPLE_H2E_IDENTIFIER CONFIG_ESP_WIFI_PW_ID
// #endif

// #define ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD WIFI_AUTH_WPA2_PSK

/* FreeRTOS event group to signal when we are connected*/

/* The event group allows multiple bits for each event, but we only care about two events:
 * - we are connected to the AP with an IP
 * - we failed to connect after the maximum amount of retries */

// #define WIFI_CONNECTED_BIT BIT0
// #define WIFI_FAIL_BIT BIT1
// #define MQTT_COMMANDS "EMB11/esp32_S3/led"
// #define MQTT_STATUS "EMB11/esp32_S3/status"
// #define LED_GPIO GPIO_NUM_16

// constexpr char MQTT_COMMANDS[] = "EMB11esp32/led";
// constexpr char MQTT_STATUS[] = "EMB11esp32/status";

// esp_mqtt_client_handle_t mqttClient = NULL;

// void mqtt_publish_task(void *pvParameters)
// {
// 	char datatoSend[20];
// 	while (1)
// 	{
// 		int val = esp_random() % 100;
// 		sprintf(datatoSend, "%d", val);
// 		int msg_id = esp_mqtt_client_publish(mqttClient, "controllerstech/test1", datatoSend, 0, 0, 0);
// 		if (msg_id == 0)
// 			ESP_LOGI(TAG, "Sent Data: %d", val);
// 		else
// 			ESP_LOGI(TAG, "Error msg_id:%d while sending data", msg_id);
// 		vTaskDelay(pdMS_TO_TICKS(2000));
// 	}
// }

// підключення до брокера, відключення,
// отримання повідомлення, помилки тощо.
//
// base      — джерело події MQTT
// event_id  — тип події (MQTT_EVENT_CONNECTED,
//             MQTT_EVENT_DATA, MQTT_EVENT_DISCONNECTED)
// event_data — дані події (тема, повідомлення, клієнт MQTT)
//
// У функції визначається тип події та виконується
// відповідна дія: підписка на тему, прийом даних,
// публікація повідомлень.

void handle_mqtt_message(const char *topic, const char *data)
{
	if (topic == NULL || data == NULL)
	{
		return;
	}

	// Проверяем, что команда пришла в нужную тему
	if (strcmp(topic, MQTT_COMMANDS) != 0)
	{
		return;
	}

	// Получаем MQTT клиент
	esp_mqtt_client_handle_t client = get_mqtt_client();

	// Проверяем существует ли MQTT клиент
	if (client == NULL)
	{
		ESP_LOGE(TAG, "MQTT client is NULL");
		return;
	}

	// Команда включения LED
	if (strcmp(data, "ON") == 0)
	{
		ESP_LOGI(TAG, "Command: LED ON");

		if (gpio_set_level(LED_GPIO, 1) == ESP_OK)
		{
			led_state = true;
			esp_mqtt_client_publish(client, MQTT_STATUS, led_state ? "ON" : "OFF", 0, 0, 0);
		}

		else
		{
			ESP_LOGE(TAG, "Failed to set LED ON");
		}
	}

	// Команда выключения LED
	else if (strcmp(data, "OFF") == 0)
	{
		ESP_LOGI(TAG, "Command: LED OFF");

		if (gpio_set_level(LED_GPIO, 0) == ESP_OK)
		{
			led_state = false;
			esp_mqtt_client_publish(client, MQTT_STATUS, led_state ? "ON" : "OFF", 0, 0, 0);
		}
		else
		{
			ESP_LOGE(TAG, "Failed to set LED OFF");
		}
	}

	// Запрос текущего состояния
	else if (strcmp(data, "STATUS") == 0)
	{
		if (esp_mqtt_client_publish(client, MQTT_STATUS, led_state ? "ON" : "OFF", 0, 0, 0) < 0)
		{
			ESP_LOGE(TAG, "Failed to publish status");
		}
		else
		{
			ESP_LOGI(TAG, "Status sent");
		}
	}

	// Неизвестная команда
	else
	{
		ESP_LOGW(TAG, "Unknown command: %s", data);
	}
}

// static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
// {
// 	ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%" PRIi32, base, event_id);
// 	// esp_mqtt_event_handle_t event = event_data;
// 	esp_mqtt_event_handle_t event = (esp_mqtt_event_handle_t)event_data;

// 	esp_mqtt_client_handle_t client = event->client;
// 	// int msg_id;
// 	switch ((esp_mqtt_event_id_t)event_id)
// 	{
// 	case MQTT_EVENT_CONNECTED:
// 		ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
// 		//        msg_id = esp_mqtt_client_subscribe(client, "controllerstech/test1", 0);
// 		//        ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);

// 		// Подписчик: принимает команды ON/OFF
// 		esp_mqtt_client_subscribe(client, MQTT_COMMANDS, 0);

// 		// Издатель: отправляет текущее состояние LED
// 		esp_mqtt_client_publish(client, MQTT_STATUS, led_state ? "ON" : "OFF", 0, 0, 0);
// 		break;

// 	case MQTT_EVENT_DISCONNECTED:
// 		ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
// 		break;

// 	case MQTT_EVENT_SUBSCRIBED:
// 		ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d, return code=0x%02x ", event->msg_id, (uint8_t)*event->data);
// 		break;

// 	case MQTT_EVENT_UNSUBSCRIBED:
// 		ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
// 		break;

// 	case MQTT_EVENT_PUBLISHED:
// 		ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
// 		break;

// 	case MQTT_EVENT_DATA:
// 	{
// 		ESP_LOGI(TAG, "MQTT_EVENT_DATA");
// 		printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
// 		printf("DATA=%.*s\r\n", event->data_len, event->data);

// 		char topic[event->topic_len + 1];
// 		memcpy(topic, event->topic, event->topic_len);
// 		topic[event->topic_len] = '\0';

// 		char data[event->data_len + 1];
// 		memcpy(data, event->data, event->data_len);
// 		data[event->data_len] = '\0';

// 		handle_mqtt_message(topic, data);

// 		// handle_mqtt_message(event->topic, event->data);
// 		break;
// 	}
// 	case MQTT_EVENT_ERROR:
// 		ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
// 		if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT)
// 		{
// 			ESP_LOGI(TAG, "Last error code reported from esp-tls: 0x%x", event->error_handle->esp_tls_last_esp_err);
// 			ESP_LOGI(TAG, "Last tls stack error number: 0x%x", event->error_handle->esp_tls_stack_err);
// 			ESP_LOGI(TAG, "Last captured errno : %d (%s)", event->error_handle->esp_transport_sock_errno,
// 					 strerror(event->error_handle->esp_transport_sock_errno));
// 		}
// 		else if (event->error_handle->error_type == MQTT_ERROR_TYPE_CONNECTION_REFUSED)
// 		{
// 			ESP_LOGI(TAG, "Connection refused error: 0x%x", event->error_handle->connect_return_code);
// 		}
// 		else
// 		{
// 			ESP_LOGW(TAG, "Unknown error type: 0x%x", event->error_handle->error_type);
// 		}
// 		break;

// 	default:
// 		ESP_LOGI(TAG, "Other event id:%d", event->event_id);
// 		break;
// 	}
// }

// static void mqtt_start(void)
// {
// 	// const esp_mqtt_client_config_t mqtt_cfg = {
// 	// 	.broker = {
// 	// 		.address.uri = "mqtt://broker.hivemq.com",
// 	// 		.address.port = 1883},
// 	esp_mqtt_client_config_t mqtt_cfg = {};

// 	mqtt_cfg.broker.address.uri = "mqtt://broker.hivemq.com";
// 	mqtt_cfg.broker.address.port = 1883;

// 	esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
// 	mqttClient = client;
// 	/* The last argument may be used to pass data to the event handler, in this example mqtt_event_handler */
// 	esp_mqtt_client_register_event(client, (esp_mqtt_event_id_t)ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
// 	esp_mqtt_client_start(client);
// }

bool getled_state()
{
	return led_state;
}

extern "C" void app_main(void)
{
	// Initialize NVS
	esp_err_t ret = nvs_flash_init();
	if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
	{
		ESP_ERROR_CHECK(nvs_flash_erase());
		ret = nvs_flash_init();
	}
	ESP_ERROR_CHECK(ret);
	ESP_LOGI(TAG, "ESP_WIFI_MODE_STA");

	// Сконфігурувати LED GPIO
	gpio_config_t io_conf = {};
	io_conf.pin_bit_mask = (1ULL << LED_GPIO);
	io_conf.mode = GPIO_MODE_OUTPUT;
	io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
	io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
	io_conf.intr_type = GPIO_INTR_DISABLE;
	ESP_ERROR_CHECK(gpio_config(&io_conf));
	ESP_ERROR_CHECK(gpio_set_level(LED_GPIO, 0));

	wifi_init_sta();

	if (get_isConnected())
		mqtt_start();
	while (1)
	{
		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}
