#include <Arduino.h>
// put function declarations here:
#include "hmac.h"
bool validateToken(String token);
int base32_decode(const char *encoded, uint8_t *result);

