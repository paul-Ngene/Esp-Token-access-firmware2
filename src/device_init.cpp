#include "device.h"


// struct DeviceConfig {
//   String device_id;
//   String secret_key;
//   uint32_t counter;
//   uint32_t remaining_minutes;
//   uint32_t remaining_hours;
// };
unsigned long end_tim = 0;

//function to retrive device id and secret key done
//function to retrive counter done
//function to save remaining_minutes done
//function to retrive remaining_minutes done
//function to save lastcounter done
//function to save device id and secret key done


// 1. CRITICAL: Define the global variable here so it exists in memory
//DeviceConfig config; 

void device_init(){    
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, LOW);

    // 2. Start LittleFS
    if(!LittleFS.begin()){
        Serial.println("LittleFS Mount Failed!");
        return;
    }

    // 3. Try to load. If it fails (file doesn't exist), set defaults.
    if (!loadConfig(config)) {
        Serial.println("No existing config found. Setting defaults...");
        
        config.device_id = "12345678";
        config.secret_key = "my_super_secret_key_123";
        config.counter = 0;
        config.remaining_minutes = 0;
        config.remaining_hours = 0;

        // Save the default config so the file exists next time
        saveConfig(config);
    } else {
        Serial.println("Config loaded successfully.");
    }
}

// ... your saveConfig and loadConfig functions stay the same ...

bool saveConfig(const DeviceConfig& cfg) {
    JsonDocument doc;

    doc["device_id"] = cfg.device_id;
    doc["secret_key"] = cfg.secret_key;
    doc["counter"] = cfg.counter;
    doc["remaining_minutes"] = cfg.remaining_minutes;
    doc["remaining_hours"] = cfg.remaining_hours;

    File file = LittleFS.open("/config.json", "w");
    if (!file) return false;

    bool ok = serializeJson(doc, file) > 0;
    file.close();

    return ok;
}

bool loadConfig(DeviceConfig& cfg) {
    if (!LittleFS.exists("/config.json"))
        return false;

    File file = LittleFS.open("/config.json", "r");
    if (!file)
        return false;

    JsonDocument doc;
    DeserializationError err = deserializeJson(doc, file);
    file.close();

    if (err)
        return false;

    cfg.device_id = doc["device_id"] | "";
    cfg.secret_key = doc["secret_key"] | "";
    cfg.counter = doc["counter"] | 0;
    cfg.remaining_minutes = doc["remaining_minutes"] | 0;
    cfg.remaining_hours = doc["remaining_hours"] | 0;

    return true;
}

bool updateField(const String& key, const String& value) {
    JsonDocument doc;

    if (LittleFS.exists("/config.json")) {
        File readFile = LittleFS.open("/config.json", "r");
        if (readFile) {
            deserializeJson(doc, readFile);
            readFile.close();
        }
    }

    doc[key] = value;

    File writeFile = LittleFS.open("/config.json", "w");
    if (!writeFile)
        return false;

    bool ok = serializeJson(doc, writeFile) > 0;
    writeFile.close();

    return ok;
}

bool updateConfig() {
    return saveConfig(config);
}