/*
#include "validate.h"
#include "print.h"
#include  "device.h"


// =========================
// TOKEN VALIDATION
// =========================
bool validateToken(String token) {
  uint8_t decoded[32];
  int len = base32_decode(token.c_str(), decoded);

  if (len != 24) {
    print_ln("Invalid length");
    return false;
  }

  uint8_t *payload = decoded;
  uint8_t *mac_received = decoded + 16;

  uint8_t mac_expected[32];
  hmac_sha256((uint8_t*)SECRET_KEY, strlen(SECRET_KEY),
              payload, 16, mac_expected);

  // Compare first 8 bytes
  if (memcmp(mac_received, mac_expected, 8) != 0) {
    print_ln("MAC mismatch");
    return false;
  }

  // Extract values
  uint32_t dev_id = (payload[0]<<24)|(payload[1]<<16)|(payload[2]<<8)|payload[3];
  uint32_t duration = (payload[4]<<24)|(payload[5]<<16)|(payload[6]<<8)|payload[7];
  uint32_t expiry = (payload[8]<<24)|(payload[9]<<16)|(payload[10]<<8)|payload[11];

  if (dev_id != DEVICE_ID) {
      print_ln("Wrong device");
    return false;
  }

  //Set EEPROM and RTC
  unsigned long now = millis() / 1000; // MVP (no real time)
  if (now > expiry) {
    print_ln("Expired");
    return false;
  }

  // VALID TOKEN
  endTime = millis() + (duration * 1000UL);

  
  print_ln("Token accepted!");
  
  return true;
}

*/