/*
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WebSerial.h>

// Wifi configuration
#define LED 2

AsyncWebServer server(80);

// const char* ssid = "SAISIKA_RT";          // Your WiFi SSID
// const char* password = "1234567890";  // Your WiFi Password

 
// put function declarations here:
#include <Hash.h>  // For SHA256
#include <bearssl/bearssl.h> // For SHA256

// =========================
// CONFIGURATION
// =========================
#define DEVICE_ID 12345678
const char* SECRET_KEY = "my_super_secret_key_123";

#define RELAY_PIN LED_BUILTIN  // GPIO2

unsigned long endTime = 0;

// =========================
// BASE32 DECODER
// =========================
#include <base64.h>

// Simple Base32 decode (custom)
int base32_decode(const char *encoded, uint8_t *result) {
  int buffer = 0, bitsLeft = 0, count = 0;

  for (int i = 0; encoded[i]; i++) {
    char ch = encoded[i];

    int val;
    if (ch >= 'A' && ch <= 'Z') val = ch - 'A';
    else if (ch >= '2' && ch <= '7') val = ch - '2' + 26;
    else continue;

    buffer <<= 5;
    buffer |= val & 0x1F;
    bitsLeft += 5;

    if (bitsLeft >= 8) {
      result[count++] = (buffer >> (bitsLeft - 8)) & 0xFF;
      bitsLeft -= 8;
    }
  }
  return count;
}

// =========================
// HMAC SHA256
// =========================
// void hmac_sha256(const uint8_t *key, int key_len,
//                  const uint8_t *data, int data_len,
//                  uint8_t *output) {

//   SHA256_CTX ctx;
//   uint8_t k_ipad[64];
//   uint8_t k_opad[64];
//   uint8_t tk[32];

//   // If key > 64 bytes, hash it
//   if (key_len > 64) {
//     sha256_init(&ctx);
//     sha256_update(&ctx, key, key_len);
//     sha256_final(&ctx, tk);
//     key = tk;
//     key_len = 32;
//   }

//   memset(k_ipad, 0, 64);
//   memset(k_opad, 0, 64);

//   memcpy(k_ipad, key, key_len);
//   memcpy(k_opad, key, key_len);

//   for (int i = 0; i < 64; i++) {
//     k_ipad[i] ^= 0x36;
//     k_opad[i] ^= 0x5c;
//   }

//   // Inner hash
//   sha256_init(&ctx);
//   sha256_update(&ctx, k_ipad, 64);
//   sha256_update(&ctx, data, data_len);
//   sha256_final(&ctx, output);

//   // Outer hash
//   sha256_init(&ctx);
//   sha256_update(&ctx, k_opad, 64);
//   sha256_update(&ctx, output, 32);
//   sha256_final(&ctx, output);
// }

//The new code but still breaking
//#include <Hash.h>

// void hmac_sha256(const uint8_t *key, int key_len,
//                  const uint8_t *data, int data_len,
//                  uint8_t *output) {

//   uint8_t k_ipad[64];
//   uint8_t k_opad[64];
//   uint8_t temp_hash[32];

//   memset(k_ipad, 0, 64);
//   memset(k_opad, 0, 64);

//   memcpy(k_ipad, key, key_len);
//   memcpy(k_opad, key, key_len);

//   for (int i = 0; i < 64; i++) {
//     k_ipad[i] ^= 0x36;
//     k_opad[i] ^= 0x5c;
//   }

//   // Inner hash: SHA256(k_ipad || data)
//   String inner = "";
//   inner.reserve(64 + data_len);

//   for (int i = 0; i < 64; i++) inner += (char)k_ipad[i];
//   for (int i = 0; i < data_len; i++) inner += (char)data[i];

//   String inner_hash = sha256(inner);

//   // Convert hex string → bytes
//   for (int i = 0; i < 32; i++) {
//     String byteStr = inner_hash.substring(i*2, i*2+2);
//     temp_hash[i] = (uint8_t) strtol(byteStr.c_str(), NULL, 16);
//   }

//   // Outer hash: SHA256(k_opad || inner_hash)
//   String outer = "";
//   outer.reserve(64 + 32);

//   for (int i = 0; i < 64; i++) outer += (char)k_opad[i];
//   for (int i = 0; i < 32; i++) outer += (char)temp_hash[i];

//   String final_hash = sha256(outer);

//   // Convert to bytes
//   for (int i = 0; i < 32; i++) {
//     String byteStr = final_hash.substring(i*2, i*2+2);
//     output[i] = (uint8_t) strtol(byteStr.c_str(), NULL, 16);
//   }
// }

//The new code
void hmac_sha256(const uint8_t *key, int key_len,
                 const uint8_t *data, int data_len,
                 uint8_t *output) {

  br_hmac_key_context kc;
  br_hmac_context ctx;

  // Initialize key
  br_hmac_key_init(&kc, &br_sha256_vtable, key, key_len);

  // Init HMAC
  br_hmac_init(&ctx, &kc, 32); // 32 = SHA256 output size

  // Feed data
  br_hmac_update(&ctx, data, data_len);

  // Finalize
  br_hmac_out(&ctx, output);
}


// =========================
// TOKEN VALIDATION
// =========================
bool validateToken(String token) {
  uint8_t decoded[32];
  int len = base32_decode(token.c_str(), decoded);

  if (len != 24) {
    Serial.println("Invalid length");
    WebSerial.println("Invalid length");
    return false;
  }

  uint8_t *payload = decoded;
  uint8_t *mac_received = decoded + 16;

  uint8_t mac_expected[32];
  hmac_sha256((uint8_t*)SECRET_KEY, strlen(SECRET_KEY),
              payload, 16, mac_expected);

  // Compare first 8 bytes
  if (memcmp(mac_received, mac_expected, 8) != 0) {
    Serial.println("MAC mismatch");
    WebSerial.println("MAC mismatch");
    return false;
  }

  // Extract values
  uint32_t dev_id = (payload[0]<<24)|(payload[1]<<16)|(payload[2]<<8)|payload[3];
  uint32_t duration = (payload[4]<<24)|(payload[5]<<16)|(payload[6]<<8)|payload[7];
  uint32_t expiry = (payload[8]<<24)|(payload[9]<<16)|(payload[10]<<8)|payload[11];

  if (dev_id != DEVICE_ID) {
      Serial.println("Wrong device");
      WebSerial.println("Wrong device");
    return false;
  }

  unsigned long now = millis() / 1000; // MVP (no real time)
  if (now > expiry) {
    Serial.println("Expired");
    WebSerial.println("Expired");
    return false;
  }

  // VALID TOKEN
  endTime = millis() + (duration * 1000UL);

  
  Serial.println("Token accepted!");
  WebSerial.println("Token accepted!");
  return true;
}


void recvMsg(uint8_t *data, size_t len){
  WebSerial.println("Received Data...");
  String token = "";
  for(int i=0; i < len; i++){
    token += char(data[i]);
  }
  WebSerial.println("Processing Token");

  token.trim();
  

  validateToken(token);

  // Timer control
  if (millis() < endTime) {
    digitalWrite(RELAY_PIN, HIGH);
  } else {
    digitalWrite(RELAY_PIN, LOW);
  }



  }

// =========================
// SETUP
// =========================
void setup() {
  Serial.begin(115200);

  //As AP
  const char* ssid     = "ESP8266-Access-Point";
  const char* password = "123456789";

  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);


 //As a station 
  // WiFi.mode(WIFI_STA);
  // WiFi.begin(ssid, password);
  // if (WiFi.waitForConnectResult() != WL_CONNECTED) {
  //   Serial.printf("WiFi Failed!\n");
  //   return;
  // }
  // Serial.println("IP Address: ");
  // Serial.println(WiFi.localIP());

  // WebSerial is accessible at "<IP Address>/webserial" in browser
  WebSerial.begin(&server);
  WebSerial.msgCallback(recvMsg);
  server.begin();
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);

  Serial.println("Enter token:");
  WebSerial.println("Enter token");
}

// =========================
// LOOP
// =========================
void loop() {

  // Read Serial Input
  // if (Serial.available()) {
  //   String token = Serial.readStringUntil('\n');
  //   token.trim();

  //   validateToken(token);
  // }

  // // Timer control
  // if (millis() < endTime) {
  //   digitalWrite(RELAY_PIN, HIGH);
  // } else {
  //   digitalWrite(RELAY_PIN, LOW);
  // }
}*/