# Helper function, which swaps two values in the box.
def swapValueByIndex(box, i, j):
    temp = box[i]
    box[i] = box[j]
    box[j] = temp

# Initialize S-box.
def initSBox(box):
    if len(box) == 0:
        for i in range(256):
            box.append(i)
    else:
        for i in range(256):
            box[i] = i

# Key schedule Algorithm (KSA) for key whose value is in unicode.
def ksa(key, box):
    j = 0
    for i in range(256):
        j = (j + box[i] + ord(key[i % len(key)])) % 256
        swapValueByIndex(box, i, j)

# KSA for key whose value is int
def ksaInt(key, box):
    j = 0
    for i in range(256):
        j = (j + box[i] + key[i % len(key)]) % 256
        swapValueByIndex(box, i, j)

# Pseudo-random generation algorithm (PRGA).
def prga(plain, box, keyStream, output):
    i = 0
    j = 0
    # Loop through every byte in plain text.
    for i in range(len(plain)):
        i = (i + 1) % 256
        j = (j + box[i]) % 256
        swapValueByIndex(box, i, j)
        keyStreamByte = box[(box[i] + box[j]) % 256]
        outputByte = chr(ord(plain[i-1]) ^ keyStreamByte)
        keyStream += chr(keyStreamByte)
        output += outputByte
    return (keyStream, output)

def main():
    SYMMETRIC_KEY_FILE_NAME = "sym_keyfile.key"
    # In symmetric key decryption, input file could also be "encryption.enc"
    INPUT_FILE_NAME = "plain.txt"
    KEYSTREAM_FILE_NAME = "keystream.key"
    # In symmetric key decryption, output file could also be "decryption.txt"
    OUTPUT_FILE_NAME = "encryption.enc"
    # Get symmetric key.
    keyFile = open(SYMMETRIC_KEY_FILE_NAME, "rb")
    key = keyFile.read()

    # Get plain text.
    plainFile = open(INPUT_FILE_NAME, "rb")
    plain = plainFile.read()

    box = []

    # intialize keyStream and output
    keyStream = ""
    output = ""

    initSBox(box)
    ksa(key, box)
    (keyStream, output) = prga(plain, box, keyStream, output)
    keyStreamFile = open(KEYSTREAM_FILE_NAME, "w")
    outputFile = open(OUTPUT_FILE_NAME, "w")
    keyStreamFile.write(keyStream)
    outputFile.write(output)

if __name__ == "__main__":
    main()
