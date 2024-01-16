from cs50 import get_string
import re


def main():
    input_text = get_string("Text: ")

    # get letters
    letter_count = 0
    for character in input_text:
        if character.isalpha():
            letter_count += 1
    print(f"Letters: {letter_count}")

    # get words
    words = input_text.split()
    word_count = len(words)
    print(f"Words: {word_count}")

    # get sentances
    sentances = re.findall("[\w\s',\":;-]+[.\?!]", input_text)
    sentance_count = len(sentances)
    print(f"Sentances: {sentance_count}")

    # Compute Coleman-Liau
    l = (letter_count / word_count) * 100.0
    s = (sentance_count / word_count) * 100.0

    index = 0.0588 * l - 0.296 * s - 15.8
    index = round(index)

    # print results
    print(index)
    if index < 1:
        print("Before Grade 1")
    elif index >= 16:
        print("Grade 16+")
    else:
        print(f"Grade {index}")


main()
