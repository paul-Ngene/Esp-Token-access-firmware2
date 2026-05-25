import hmac
import hashlib

SECRET_KEY = b"my_super_secret_key_123"
DEVICE_ID = 12345678

def generate_token(counter, hr):
    counter_str = f"{counter:04d}"
    time_str = f"{hr:04d}"
    device_str = f"{DEVICE_ID:04d}"

    message = (counter_str + time_str + device_str).encode()

    #mac = hmac.new(SECRET_KEY, message, hashlib.sha256).hexdigest()
    # mac_int = int(mac, 16)
    # signature = str(mac_int % 10000).zfill(4)

    # Updated Python snippet to match C++ logic
    hash_bytes = hmac.new(SECRET_KEY, message, hashlib.sha256).digest()
    # Grab first 4 bytes and convert to int (Big Endian)
    mac_int = int.from_bytes(hash_bytes[:4], byteorder='big')
    signature = str(mac_int % 10000).zfill(4)



    
    token = counter_str + time_str + device_str + signature
    return token


if __name__ == "__main__":
    counter = int(input("Counter: "))
    hr = int(input("Hours: "))

    print("\nTOKEN:")
    print(generate_token(counter, hr))
