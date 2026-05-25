
// #include <Arduino.h>

// // =========================
// // BASE32 DECODER
// // =========================
// #include <base64.h>

// // Simple Base32 decode (custom)
// int base32_decode(const char *encoded, uint8_t *result) {
//   int buffer = 0, bitsLeft = 0, count = 0;

//   for (int i = 0; encoded[i]; i++) {
//     char ch = encoded[i];

//     int val;
//     if (ch >= 'A' && ch <= 'Z') val = ch - 'A';
//     else if (ch >= '2' && ch <= '7') val = ch - '2' + 26;
//     else continue;

//     buffer <<= 5;
//     buffer |= val & 0x1F;
//     bitsLeft += 5;

//     if (bitsLeft >= 8) {
//       result[count++] = (buffer >> (bitsLeft - 8)) & 0xFF;
//       bitsLeft -= 8;
//     }
//   }
//   return count;
// }

// //