#!/usr/bin/python3


from Dictionary import Dictionary
import sys

if __name__ == '__main__':
    if len(sys.argv) != 2:
        print("Usage: dict <word>")
        sys.exit()

    word = argv[1]
    result = Dictionary.search(word)
    print(word + ":")
    print(result)
