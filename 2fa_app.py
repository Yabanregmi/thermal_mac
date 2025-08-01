import os
import pyotp
import qrcode

SECRET_FILE = "2fa_secret.txt"

# Step 1: Load existing secret or create a new one
if os.path.exists(SECRET_FILE):
    with open(SECRET_FILE, "r") as f:
        secret = f.read().strip()
        print("Loaded existing secret.")
else:
    secret = pyotp.random_base32()
    with open(SECRET_FILE, "w") as f:
        f.write(secret)
        print("Generated and saved new secret.")

print("Secret key (store this):", secret)

# Step 2: Create TOTP object
totp = pyotp.TOTP(secret)
print("Current OTP:", totp.now())

# Step 3: Generate QR code only if new secret is generated
if not os.path.exists("qrcode.png"):
    uri = totp.provisioning_uri(name="user@example.com", issuer_name="MyApp")
    qrcode.make(uri).save("qrcode.png")
    print("QR code saved as qrcode.png - scan this with Google Authenticator.")

# Step 4: Verify OTP
user_code = input("Enter the OTP from your app: ")
if totp.verify(user_code):
    print("OTP is valid!")
else:
    print("Invalid OTP.")
