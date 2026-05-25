#include <Arduino.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WebSerial.h>

void print_ln(const char *message){
    Serial.println(message);
    //WebSerial.println(message);
}


void print_p(const char *message){
    Serial.print(message);
    //WebSerial.print(message);
}