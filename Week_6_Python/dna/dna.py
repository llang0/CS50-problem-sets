import csv
import sys


def main():
    # important variables
    db_filename = sys.argv[1]  # database filename
    seq_filename = sys.argv[2]  # sequence filename
    dicts = []  # all the db rows as dicts
    db_strs = []  # all the strs from the database
    results = {}  # the longest match of each str in the sequence

    # Check for command-line usage
    if len(sys.argv) != 3:
        print("Usage: python dna.py DATABASE SEQUENCE")
        sys.exit

    # read from file and save data to dicts
    with open(db_filename) as f:
        reader = csv.DictReader(f)
        for row in reader:
            dicts.append(row)

    # create list of strs
    db_strs = list(dicts[0].keys())[1:]

    # convert all the str counts to ints
    for dict in dicts:
        for str in db_strs:
            dict[str] = int(dict[str])

    # Read DNA sequence file into a variable
    with open(seq_filename) as f:
        seq = f.read()

    # Find longest match of each STR in DNA sequence
    for str in db_strs:
        results[str] = longest_match(seq, str)

    # Check database for matching profiles
    for row in dicts:
        for str in db_strs:
            if row[str] != results[str]:
                break
        else:
            print(row["name"])
            return
    else:
        print("No match")
    return


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):
        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:
            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
