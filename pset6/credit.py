import math

def main():
    # gets user input and saves the credit card number
    cc_num = input("Number: ")

    # gets the numbers to be multiplied by two for checksum
    i = 0 # counting variable
    product_str = "" #this is the part of the check sum that *2
    add_str = "" # just add these to checksum

    if len(cc_num) % 2 == 0:
        for place in cc_num:
            if i % 2 == 0:
                product_str += place
            else:
                add_str += place
            i += 1
    else:
        for place in cc_num:
            if i % 2 == 0:
                add_str += place
            else:
                product_str += place
            i += 1

    # multiply the prouct_str by 2 and adds the digits to checksum
    checksum = 0
    for place in product_str:
        num = int(place) * 2
        checksum += math.floor(num/10) + num%10

    # adds the add_str to make final checksum
    for place in add_str:
        checksum += int(place)

    # checks checksum
    if checksum % 10 == 0:
        if cc_num[0] == '4':
            print("VISA");
        elif cc_num[0] == '3'and cc_num[1] == '4' or '7':
            print("AMEX")
        elif int(cc_num) >= 5100000000000000 and int(cc_num) < 5600000000000000:
            print("MASTERCARD")
    else:
        print("INVALID")


if __name__ == "__main__":
    main()
