# **Assignment 5**
The goal of this assignment is to look at Schmidt-Samoa Algorithm.

### Build
---
```
$ make
```

### Running Keygen
---
```
 $ ./keygen
```
+ `-b`:specifies the minimum bits needed for the public modulusn.
+ `-i`:specifies the number of Miller-Rabin iterations for testing primes(default:50).
+ `-n pbfile`:specifies the public key file (default: ss.pub).
+ `-d pvfile`:specifies the private key file (default: ss.priv).
+ `-s`: specifies the random seed for the random state initialization (default: the seconds since the UNIX epoch, given by time(NULL)).
+ `-v`:enables verbose output.
+ `-h`:displays program synopsis and usage

### Running Encrypt
---
```
 $ ./encrypt
```
+ `-i`: specifies the input file to encrypt (default: stdin).
+ `-o`: specifies the output file to encrypt (default: stdout).
+ `-n`: specifies the file containing the public key (default: ss.pub).
+ `-v`: enables verbose output.
+ `-h`: displays program synopsis and usage.

### Running Decrypt
---
```
 $ ./decrypt
```
+ `-i`: specifies the input file to decrypt (default:stdin).
+ `-o`: specifies the output file to decrypt (default:stdout).
+ `-n`: specifies the file containing the private key (default:ss.priv).
+ `-v`: enables verbose output.
+ `-h`: displays program synopsis and usage

### Cleaning
---
```
$ make clean
```

### Include Files:
---
+ `decrypt.c`:This contains the implementation and main() function for the decrypt program.
+ `encrypt.c`:This contains the implementation and main() function for the encrypt program.
+ `keygen.c`:This contains the implementation and main() function for the keygen program.
+ `numtheory.c`:This contains the implementations of the number theory functions.
+ `numtheory.h`: This specifies the interface for the number theory functions.
+ `randstate.c`: This contains the implementation of the random state interface for the SS library and number theory functions.
+ `randstate.h`: This specifies the interface for initializing and clearing the random state.
+ `ss.c`: This contains the implementation of the SS library.
+ `ss.h`: This specifies the interface for the SS library.
+ `Makefile` - has all the command to compile and clean the files
+ `README.md` - Describes how to use the script
+ `DESIGN.pdf` - Describes the design process 
+ `WRITEUP.pdf` - Includes the output of the program 




