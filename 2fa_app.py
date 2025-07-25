import pyotp
import qrcode

# Step 1: Generate a secret key (store this for the user)
secret = pyotp.random_base32()
print("Secret key (store this):", secret)

# Step 2: Create TOTP object
totp = pyotp.TOTP(secret)
print("Current OTP:", totp.now())

# Step 3: Create QR Code (scan it with Google Authenticator)
uri = totp.provisioning_uri(name="user@example.com", issuer_name="My2FAApp")
qrcode.make(uri).save("qrcode.png")
print("QR code saved as qrcode.png - scan this with Google Authenticator.")

# Step 4: Verify OTP
user_code = input("Enter the OTP from your app: ")
if totp.verify(user_code):
    print("OTP is valid!")
else:
    print("Invalid OTP.")
