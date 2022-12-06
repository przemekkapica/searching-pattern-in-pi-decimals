# Searching pattern in pi decimals

Project aims to tell whether given pattern occurs in pi decimal digits.

It uses Chudnovsky algorithm with binary splitting for generating PI expansion and Knuth–Morris–Pratt as for the pattern searching.

## How to run


### Generate PI
Compile the file

```g++ -o gen_pi gen_pi.cpp```

Generates pi with 1000000 digits of decimal expansion

```./gen_pi -d 1000000```


### Search a pattern
Compile the file

```g++ -o kmp kmp.cpp```

Tells at what index the given pattern occurs, or -1 if it doesn't

```./kmp -i "path_to_pi" -p "523"```
