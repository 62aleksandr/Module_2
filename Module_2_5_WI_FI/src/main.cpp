#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

// Настройки Wi-Fi и пина  192.168.4.1
const char *ssid = "ESP32_Relay_Control";
const char *password = "password123";
const int relayPin = 16;

WebServer server(80);

void handleRoot()
{
  String html = "<html><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<style>body{text-align:center; font-family:sans-serif; margin-top:50px;} ";
  html += ".btn{display:inline-block; padding:15px 30px; font-size:24px; color:white; text-decoration:none; margin:10px; border-radius:5px;} ";
  html += ".on{background-color:green;} .off{background-color:red;}</style></head><body>";
  html += "<h2>Керування реле ESP32-S3</h2>";
  html += "<a href='/on' class='btn on'>ВКЛЮЧИТИ</a>";
  html += "<a href='/off' class='btn off'>ВИКЛЮЧИТИ</a>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void handleRelayOn()
{
  digitalWrite(relayPin, HIGH);
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleRelayOff()
{
  digitalWrite(relayPin, LOW);
  server.sendHeader("Location", "/");
  server.send(303);
}

// Этот метод выполнится один раз при старте платы вместо app_main
void setup()
{
  Serial.begin(115200);

  // Настройка периферии
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);

  // Настройка Wi-Fi точки
  WiFi.softAP(ssid, password);
  Serial.println("Точка доступа запущена!");
  Serial.print("IP: ");
  Serial.println(WiFi.softAPIP());

  // Настройка путей сервера
  server.on("/", handleRoot);
  server.on("/on", handleRelayOn);
  server.on("/off", handleRelayOff);
  server.begin();
}

// Этот метод будет крутиться в бесконечном цикле автоматически
void loop()
{
  server.handleClient(); // Обработка запросов веб-сервера
  delay(10);             // Небольшая задержка для стабильности FreeRTOS
}