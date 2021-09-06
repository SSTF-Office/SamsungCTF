import os
from zipfile import ZipFile

# Generating digest

os.system("openssl dgst -r -md4 -hex -out digest.der update.py")
print("Digest generated")

# Sign

os.system("openssl dgst -md4 -sign ../vendor_private.pem -out signature.der update.py")
print("Digest signed")

# Verify:
# openssl dgst -md4 -verify vendor_public.pem -signature signature.der update.py

with ZipFile('update.zip', 'w') as myzip:
    myzip.write('config.yaml')
    myzip.write('digest.der')
    myzip.write('signature.der')
    myzip.write('update.py')

print("Update package prepared")
