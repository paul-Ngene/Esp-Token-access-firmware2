#include <Arduino.h>
#include "print.h"
#include "device.h"
#include "wifi.h"
#include "webserver.h"
#include "validate.h"


void convertToTime(float min);
DeviceConfig config;

// Variable to track the last time we decremented a minute
unsigned long lastMinuteTick = 0;


// =========================
// SETUP
// =========================
void setup() {
  Serial.begin(115200);
//float totalHours = 0; // Example: 2 hours and 31.8 minutes

//LittleFS.format();

  // Load existing config from Flash (Survives power loss)
    
  // As AP password
  const char* ssid     = "ESP8266-Access-Point"; //  WiFi SSID
  const char* password = "123456789";            //  WiFi Password

  // As Station Password
  //const char* ssid = "SAISIKA_RT";          //  WiFi SSID
  //const char* password = "1234567890";  //  WiFi Password
  bool station = false;  
  wifi_init(password, ssid, station);
  device_init();
  webserver_init();
  // from the web interface / terminal
  print_ln("System Ready. Enter token:");
    
    // Initialize the ticker
    lastMinuteTick = millis();

}

// =========================
// LOOP
// =========================
void loop() {
  
    // 1. Time Accounting Ticker
    // Check if 60 seconds (60,000 ms) have passed
    if (millis() - lastMinuteTick >= 60000UL) {
        lastMinuteTick = millis();

        if (config.remaining_minutes > 0) {
            config.remaining_minutes--;
            
            // Save the new balance immediately so it survives power loss
            updateConfig(); 
            //char buf[256]; // Ensure the buffer is large enough for digits + null terminator

            //print_p(itoa(config.remaining_minutes, buf, 10));
           convertToTime(config.remaining_minutes);
        }
    }

    // 2. Physical Output Control
    // The relay stays HIGH as long as there is a minute balance
    if (config.remaining_minutes > 0) {
        digitalWrite(RELAY_PIN, LOW); // active low is pin 2
    } else {
        digitalWrite(RELAY_PIN, HIGH);
    }

    // Optional: Keep your WebSerial or other background tasks happy
    yield(); 
}
void printTwoDigits(int number) {
  if (number < 10) {
    print_p("0");
  }
  Serial.print(number);
}

void convertToTime(float min) {
  // 1. Convert total hours to total seconds
  // Use long to prevent overflow if hours are very large
  long totalSeconds = (long)(min * 60);

  // 2. Extract hours, minutes, and seconds
  int h = totalSeconds / 3600;
  int m = (totalSeconds % 3600) / 60;
  int s = totalSeconds % 60;

  // 3. Print the result with leading zeros for formatting

  Serial.print("Total Hours: ");
  Serial.println(h, 4);
  
  // Serial.print("Formatted: ");
  // printTwoDigits(h);
  // Serial.print(":");
  // printTwoDigits(m);
  // Serial.print(":");
  // printTwoDigits(s);
  // Serial.println();
     print_p("Formatted: ");
   printTwoDigits(h);
   print_p(":");
   printTwoDigits(m);
   print_p(":");
   printTwoDigits(s);
   print_ln("\n");
}