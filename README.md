# Getting started

This is simple programm to encrypt file with key
feom password and write the result in encrypted.txt
output file.
The encrypted.txt file can be decrypted with key from
the same password as it was encrypted.

## Installation

The program can be obtained from the source codes which
are located [here](https://github.com/TimofeyGritsun/lab2)

To install and build programm, do the following::

```console
git clone https://github.com/TimofeyGritsun/lab2.git
mkdir lab2/build/
cd lab2/build/
cmake ../CMakeLists.txt
make
```
After that in lab2/build will be executable file lab2.

## Usage

Programm has 2 modes:

- Encryption
- Decryption

In encryption mode program has syntaxis:

```console
./build/lab2 -p arg1 -f arg2
```

- arg1 - password to create secret key *(max length 31)*
- arg2 - filename to encryption *(max length 31)*

In decription mode program has syntaxis:

```console
./build/lab2 -e arg1
```

- arg1 - password to create secret key to decrypt *encrypted.txt* file *(max length 31)*
