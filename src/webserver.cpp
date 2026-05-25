#include "webserver.h"
#include "validate.h"
#include "device.h"
AsyncWebServer server(80);

void webserver_init(){
    // ... existing setup code (WiFi, etc.)

// Route for the main page
server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
});

// Route to receive token from the web UI
server.on("/msg", HTTP_GET, [](AsyncWebServerRequest *request){
    if (request->hasParam("token")) {
        String token = request->getParam("token")->value();
        if (validateToken(token)) {
            request->send(200, "text/plain", "Token Accepted!");
        } else {
            request->send(200, "text/plain", "Invalid Token.");
        }
    }
});

// Route for the UI to poll status (Time and Relay)
server.on("/status", HTTP_GET, [](AsyncWebServerRequest *request){
    int h = config.remaining_minutes / 60;
    int m = (int)config.remaining_minutes % 60;
    int s = 0; // Simplified 
    
    char json[100];
    sprintf(json, "{\"balance\":%d, \"formatted\":\"%02d:%02d:%02d\"}", 
            (int)config.remaining_minutes, h, m, s);
    request->send(200, "application/json", json);
});

server.begin();
}