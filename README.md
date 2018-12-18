# lsdpl

Searches for duplicate files based on their actual content. It can:
- list the duplicates
- remove the duplicates
  - either the first occurrence is deleted
  - or all first + 1 occurrences are deleted

# Usage
```bash
# List all duplicates inside the folder ~/Music and do not print errors (e.g. permission denied).
lsdpl --suppress-errors ~/Music
# Remove all duplicates from the folders ~/Music and ~/MyMusic and do not print errors (e.g. permission denied).
# The first encounter of a file for which a duplicate exists is considered as the original.
lsdpl --suppress-errors --remove-last ~/Music ~/MyMusic
# Remove all duplicates from the folder ~/Music.
# The last encounter of a file for which a duplicate exists is considered as the original.
lsdpl --remove-first ~/Music
```
  
# Dependencies
- [boost filesystem](https://www.boost.org/doc/libs/1_68_0/libs/filesystem/doc/index.htm)
- [OpenSSL (crypto)](https://github.com/openssl/openssl)
- [gflags](https://github.com/gflags/gflags)

# Installation
1. Install all depdencies
2. git clone https://github.com/lgeorgieff/lsdpl.git
3. cd lsdpl
4. mkdir build && cd build
5. cmake ..
6. make

# License
This project is licensed under [BSD 3-Clause license](./LICENSE.txt).
