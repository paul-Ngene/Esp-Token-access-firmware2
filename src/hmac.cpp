
#include "hmac.h"


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

