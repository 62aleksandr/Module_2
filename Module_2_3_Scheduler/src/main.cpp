#include <Arduino.h>

const uint8_t LED1_PIN = LED_BUILTIN;
const uint8_t LED2_PIN = 16;

struct Task
{
  uint32_t period;
  uint32_t lastRun;
  void (*callback)();
};

//-------------------
// Task LED1
//-------------------

void taskLED1()
{
  static bool ledState = false;

  ledState = !ledState;

  digitalWrite(LED1_PIN, ledState);
}

//-------------------
// Task LED2
//-------------------

void taskLED2()
{
  static bool ledState = false;

  ledState = !ledState;

  digitalWrite(LED2_PIN, ledState);
}

//-------------------
// Scheduler Table
//-------------------

Task tasks[] =
    {
        {200, 0, taskLED1},
        {500, 0, taskLED2}};

constexpr uint8_t TASK_COUNT =
    sizeof(tasks) / sizeof(tasks[0]);

//-------------------
// Setup
//-------------------

void setup()
{
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
}

//-------------------
// Scheduler
//-------------------

void schedulerRun()
{
  uint32_t currentTime = millis();

  for (uint8_t i = 0; i < TASK_COUNT; i++)
  {
    if (currentTime - tasks[i].lastRun >= tasks[i].period)
    {
      tasks[i].lastRun = currentTime;

      tasks[i].callback();
    }
  }
}

//-------------------
// Superloop
//-------------------

void loop()
{
  schedulerRun();
}