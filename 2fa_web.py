import os
import sys
import pyotp
import qrcode

SECRET_FILE = "2fa_secret.txt"

def get_or_create_secret():
    """Load the saved secret or create a new one if it doesn't exist."""
    if os.path.exists(SECRET_FILE):
        with open(SECRET_FILE, "r") as f:
            secret = f.read().strip()
        print("[INFO] Loaded existing secret from file.")
    else:
        secret = pyotp.random_base32()
        with open(SECRET_FILE, "w") as f:
            f.write(secret)
        print("[INFO] Generated and saved new secret.")
        generate_qr_code(secret)
    return secret

def generate_qr_code(secret, filename="qrcode.png"):
    """Generate a QR code from a secret."""
    uri = pyotp.TOTP(secret).provisioning_uri(name="user@example.com", issuer_name="MyApp")
    qrcode.make(uri).save(filename)
    print(f"[INFO] QR code saved as {filename} (scan with Google Authenticator).")

def verify_otp(secret):
    """Prompt the user for OTP and verify it."""
    totp = pyotp.TOTP(secret)
    print("[DEBUG] Current OTP (for testing):", totp.now())
    user_code = input("Enter the OTP from your authenticator app: ")
    if totp.verify(user_code):
        print("[SUCCESS] OTP is valid!")
    else:
        print("[ERROR] Invalid OTP. Check the time sync and the code.")

def add_new_device(secret):
    """Generate a QR code for scanning on a new device."""
    generate_qr_code(secret, filename="qrcode_new_device.png")
    print("[INFO] Use qrcode_new_device.png to add another device.")

if __name__ == "__main__":
    secret = get_or_create_secret()

    # If user wants to add a new device
    if "--new-device" in sys.argv:
        add_new_device(secret)
    else:
        verify_otp(secret)
