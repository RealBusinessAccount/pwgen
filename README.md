# mkpass

The purpose of mkpass is to generate random strings suitable for passwords.

mkpass accepts the following options to allow the user to select which characters will be included in the random string.
    '-a' for lowercase letters
    '-A' for uppercase letters
    '-n' for numbers
    '-s' for symbols
    '-b' for brackets

mkpass also accepts the options '-l' to set a custom length and '-o' to specify a number of strings to output.
    '-l 32'
    '-o 10'

By default, mkpass will generate a 16 character string pulling from all the character sets.

mkpass pulls all its randomness from /dev/urandom, so it only works on *nix systems.
