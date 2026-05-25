// #include "validate.h"
// #include "print.h"
// #include  "device.h"


// bool validateToken(String token) {
   

//   if (token.length() != 20) { 
//     print_ln("Invalid length");
//     return false;
//   }

//   uint32_t counter = token.substring(0, 4).toInt();
//   uint32_t minutes = token.substring(4, 8).toInt();
//   uint32_t device  = token.substring(8, 16).toInt(); // Device is 8 chars
//   uint32_t sig     = token.substring(16, 20).toInt(); // Sig is last 4 chars
  
//   if (device != DEVICE_ID) {
//     print_ln("Wrong device");
//     return false;
//   }

//   /* //If EEPROM is used
//   if (counter <= lastCounter) {
//     Serial.println("Replay detected");
//     return false;
//   }
// */
//   String message = token.substring(0,16);

//   uint8_t mac[32];
//   hmac_sha256((uint8_t*)SECRET_KEY, strlen(SECRET_KEY),
//               (uint8_t*)message.c_str(), message.length(),
//               mac);

//   uint32_t mac_int =
//     (mac[0]<<24)|(mac[1]<<16)|(mac[2]<<8)|mac[3];

//   uint32_t expected = mac_int % 10000;

//   if (expected != sig) {
//     print_ln("Invalid signature");
//     return false;
//   }

//   // ACCEPT TOKEN
//   //lastCounter = counter; // If EEPROM is used
//   //saveCounter(lastCounter);

//   //endTime = millis() + (minutes * 60000UL);

//   print_ln("Token accepted!");
//   return true;
// }