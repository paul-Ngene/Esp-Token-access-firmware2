#include "validate.h"
#include "print.h"
#include "device.h"

uint32_t r_sec;
//r_min = 0;
//extern unsigned long endTime;

 

bool validateToken(String token)
{
    // -----------------------------
    // Format:
    // CCCC TTTT DDDDDDDD SSSS
    // 4    4    8        4 = 20 chars
    // -----------------------------
    if (token.length() != 20) {
        print_ln("Invalid length");
        return false;
    }

    uint32_t counter = token.substring(0, 4).toInt();
    uint32_t hours = token.substring(4, 8).toInt();
    String device = token.substring(8, 16);
    uint32_t sig = token.substring(16, 20).toInt();

    // Ensure config loaded
    if (!loadConfig(config)) {
        print_ln("Config load failed");
        return false;
    }

    // -----------------------------
    // Device check
    // -----------------------------
    if (device != config.device_id) {
        print_ln("Wrong device");
        //could set an error variable so i know what causes the false return for better error logging
        return false;
    }

    // -----------------------------
    // Anti replay check
    // -----------------------------
    if (counter <= config.counter) {
        print_ln("Replay detected");
        return false;
    }

    // -----------------------------
    // Recompute HMAC
    // -----------------------------
    String message = token.substring(0, 16);

    uint8_t mac[32];

    hmac_sha256(
        (uint8_t*)config.secret_key.c_str(),
        config.secret_key.length(),
        (uint8_t*)message.c_str(),
        message.length(),
        mac
    );

    uint32_t mac_int =
        ((uint32_t)mac[0] << 24) |
        ((uint32_t)mac[1] << 16) |
        ((uint32_t)mac[2] << 8) |
        ((uint32_t)mac[3]);

    uint32_t expected = mac_int % 10000;

    if (expected != sig) {
        print_ln("Invalid signature");
        return false;
    }

    // -----------------------------
    // ACCEPT TOKEN
    // -----------------------------
    config.counter = counter;

    // Persist absolute expiry
    // For MVP:
    // config.end_time = millis() + (minutes * 60000UL); // gpt initially used endtime
    // Add new minutes to the existing balance
    config.remaining_hours += hours;
    r_sec = config.remaining_hours * 3600;
    config.remaining_minutes = (r_sec / 60);
    
Serial.println(config.remaining_minutes); // for debugging
  delay(1000);
    config.remaining_hours = 0;
    if (!updateConfig()) {
        print_ln("Failed saving config");
        return false;
    }

    //end_time = config.end_time;

    print_ln("Token accepted! Time Added");
    return true;
}/*  */
