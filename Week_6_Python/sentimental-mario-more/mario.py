def main():
    # keep asking for input until you get an int
    while True:
        try:
            size = int(input("Height: "))
            if size > 0 and size < 9:
                break
            else:
                pass
        except ValueError:
            pass
    # print the pyramid
    mario(size)


def mario(s):
    for i in range(s - 1, -1, -1):
        for j in range(s):
            if j < i:
                print(" ", end="")
            else:
                print("#", end="")
        print("  ", end="")
        for j in range(s - 1, -1, -1):
            if j >= i:
                print("#", end="")
        print()


main()
