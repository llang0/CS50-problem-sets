def main():
    change = get_change()
    change *= 100
    num_coins = 0

    # count number or quarters
    while change >= 25:
        num_coins += 1
        change -= 25

    # count number of dimes
    while change >= 10:
        num_coins += 1
        change -= 10

    # count number of nickels
    while change >= 5:
        num_coins += 1
        change -= 5

    # count number of pennies
    while change >= 1:
        num_coins += 1
        change -= 1

    print(num_coins)


def get_change():
    while True:
        try:
            c = float(input("Change: "))
            if c > 0:
                break
        except ValueError:
            pass
    return c


main()
