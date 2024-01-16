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
    # decrement i, incremement j
    for i in range(s - 1, -1, -1):
        for j in range(s):
            # compare them to decide whether to print space or hash
            if i > j:
                print(" ", end="")
            else:
                print("#", end="")
        print()


main()
