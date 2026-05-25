import hmac
import hashlib
import struct
import base64
import time
import os

# =========================
# CONFIGURATION
# =========================
SECRET_KEY = b"my_super_secret_key_123"   # MUST MATCH ESP8266
DEVICE_ID = 12345678                      # Must match device

# =========================
# GENERATE TOKEN
# =========================
def generate_token(duration_seconds):
    expiry = int(time.time()) + 86400  # valid for 1 day
    nonce = int.from_bytes(os.urandom(4), 'big')

    # Pack payload (4 integers, 4 bytes each)
    payload = struct.pack(">IIII",
        DEVICE_ID,
        duration_seconds,
        expiry,
        nonce
    )

    # Generate MAC
    mac = hmac.new(SECRET_KEY, payload, hashlib.sha256).digest()
    mac_truncated = mac[:8]

    token_bytes = payload + mac_truncated

    # Encode (Base32, no padding)
    token = base64.b32encode(token_bytes).decode().replace("=", "")

    return token


# =========================
# RUN
# =========================
if __name__ == "__main__":
    duration = int(input("Enter duration (seconds): "))
    token = generate_token(duration)

    print("\nTOKEN:")
    print(token)