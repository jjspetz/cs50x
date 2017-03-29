import sys

def main():
    # checks for input of a cipher key
    if len(sys.argv) != 2:
        print("Usage: python caesar.py key")
        print("Where key is a letter for the cipher")
        return 1

    # gets value for key
    key = ord(sys.argv[1])

    # gets user input
    text = input("Enter text to encypt: ")

    # prints cipher_text
    print("cipher text: {}".format(cipher(text, key)))

# loops through the plain text and creates cipher text
def cipher(text, key):
    cipher_text = ""

    for char in text:
        if ord(char) > 64 and ord(char) < 91:
            cipher_text += chr((ord(char) + key - 130) % 26 + 65)
        elif ord(char) > 96 and ord(char) < 122:
            cipher_text += chr((ord(char) + key - 194) % 26 + 97)
        else:
            cipher_text += char

    return cipher_text

if __name__ == "__main__":
    main()
