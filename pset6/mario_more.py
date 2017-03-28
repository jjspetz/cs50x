def main ():
    print_pyramid(get_height())

def get_height():
    print("Enter a number between 1 and 22.")
    height = input("Height: ")

    if int(height) > 23 or int(height) < 0:
        print("Enter a number between 1 and 22.")
        height = input("Height: ")

    return int(height);

def print_pyramid(height):
    for i in range(height + 1):
        for j in range(height - i):
            print(" ", end="")
        for k in range(i):
            print("#", end="")
        print("  ", end="")
        for k in range(i):
            print("#", end="")
        print("") # prints the needed page break

if __name__ == "__main__":
    main()
