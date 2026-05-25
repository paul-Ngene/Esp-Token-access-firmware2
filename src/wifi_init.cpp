#include <Arduino.h>
#include "wifi.h"
void wifi_init(const char* password,const char* ssid, bool station){
    if(!station){
      // AP code
      //As AP
      WiFi.softAP(ssid, password);

      IPAddress IP = WiFi.softAPIP();
      Serial.print("AP IP address: ");
      Serial.println(IP);
  
    }
    else{
        // station
      //As a station 
      WiFi.mode(WIFI_STA);
      WiFi.begin(ssid, password);
      if (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.printf("WiFi Failed!\n");
        return;
      }
      Serial.println("IP Address: ");
      Serial.println(WiFi.localIP());
    }
}




 
