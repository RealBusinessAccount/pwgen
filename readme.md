# pwgen, the simple, handy password generator

The purpose of pwgen is to generate random strings suitable for passwords.

pwgen accepts the following options to allow the user to select which characters will be included in the random string.
    '-a' for letters (both upper and lower case)
    '-n' for numbers
    '-p' for punctuation
    '-e' for extended

pwgen also accepts the option '-l' to set a custom length.
    '-l 32'

By default, pwgen will generate a 16 character string pulling from all the character sets.
