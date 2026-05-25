/*


bool validateToken(String token) {

  if (token.length() != 16) {
    Serial.println("Invalid length");
    return false;
  }

  uint32_t counter = token.substring(0,4).toInt();
  uint32_t minutes = token.substring(4,8).toInt();
  uint32_t device = token.substring(8,12).toInt();
  uint32_t sig = token.substring(12,16).toInt();

  if (device != DEVICE_ID) {
    Serial.println("Wrong device");
    return false;
  }

  if (counter <= lastCounter) {
    Serial.println("Replay detected");
    return false;
  }

  String message = token.substring(0,12);

  uint8_t mac[32];
  hmac_sha256((uint8_t*)SECRET_KEY, strlen(SECRET_KEY),
              (uint8_t*)message.c_str(), message.length(),
              mac);

  uint32_t mac_int =
    (mac[0]<<24)|(mac[1]<<16)|(mac[2]<<8)|mac[3];

  uint32_t expected = mac_int % 10000;

  if (expected != sig) {
    Serial.println("Invalid signature");
    return false;
  }

  // ACCEPT TOKEN
  lastCounter = counter;
  saveCounter(lastCounter);

  endTime = millis() + (minutes * 60000UL);

  Serial.println("Token accepted!");
  return true;
}
*/