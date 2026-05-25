#include <Arduino.h>
#include <Hash.h>  // For SHA256 // doesnt work with esp
#include <bearssl/bearssl.h> // For SHA256 //this works on esp


//The new code
void hmac_sha256(const uint8_t *key, int key_len,
                 const uint8_t *data, int data_len,
                 uint8_t *output);
