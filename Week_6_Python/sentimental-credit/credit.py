def main():
    number = input("Number: ")

    # if it passes checksum
    if checksum(number):
        # run funtions checking for correct length and digit patterns
        if visa(number):
            print("VISA")
            return
        elif amex(number):
            print("AMEX")
            return
        elif mastercard(number):
            print("MASTERCARD")
            return
        # if it passes checksum but doesn't match any digit patterns
        else:
            print("INVALID")
            return
    # if it doesn't pass checksum
    else:
        print("INVALID")


def checksum(num):
    total = 0
    digits = []

    # gather every other digit starting from second to last
    for i in range(len(num) - 2, -1, -2):
        x = int(num[i]) * 2
        digits.append(x)

    for j in range(len(digits)):
        if len(str(digits[j])) > 1:
            string = str(digits[j])
            total += int(string[0])
            total += int(string[1])
        else:
            total += int(digits[j])
    
    if len(num) % 2 == 1:
        start = 0
    else:
        start = 1

    # Gather the rest
    for i in range(start, len(num), 2):
        total += int(num[i])
    if total % 10 == 0:
        return True
    else:
        return False


# check to see if valid visa number
def visa(n):
    if len(n) == 16 or len(n) == 13:
        if n[0] == "4":
            return True
    return False


# check to see if valid amex number
def amex(n):
    if len(n) == 15:
        if n[:2] == "34" or n[:2] == "37":
            return True
    return False


# check to see if valid mastercard number
def mastercard(n):
    if len(n) == 16:
        if int(n[:2]) > 50 and int(n[:2]) < 56:
            return True
    return False


main()
