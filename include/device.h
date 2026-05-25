#include <Arduino.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
// =========================
// CONFIGURATION
// =========================
#define DEVICE_ID 12345678
//inline constexpr const char* SECRET_KEY = "my_super_secret_key_123";
#define SECRET_KEY "my_super_secret_key_123"
#define RELAY_PIN LED_BUILTIN  // GPIO2
void device_init();

extern unsigned long end_tim;

extern uint32_t r_sec;
//Set EEPROM and RTC or littlefs
#pragma once
struct DeviceConfig {
  String device_id;
  String secret_key;
  uint32_t counter;
  uint32_t remaining_minutes; // gpt initially used endtime
  uint32_t remaining_hours; // gemini recomended this because of lfs
  //uint32_t r_min;
};

extern DeviceConfig config;

bool saveConfig(const DeviceConfig& cfg);
bool loadConfig(DeviceConfig& cfg);
bool updateConfig();