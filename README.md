# lsdpl

Searches recursively for duplicate files based on their actual content. It allows to:
- list duplicates
- remove duplicates
  - either the first occurrence is deleted (first in terms of last modified time)
  - or all first + 1 occurrences are deleted (first + 1 in terms of last modified time)

# Usage
```bash
# List all duplicates inside the folder ~/Music and do not print errors (e.g. permission denied).
lsdpl --suppress-errors ~/Music
# Remove all duplicates from the folders ~/Music and ~/MyMusic and do not print errors (e.g. permission denied).
# The first encounter (last modified time) of a file for which a duplicate exists is considered as the original.
lsdpl --suppress-errors --remove-last ~/Music ~/MyMusic
# Remove all duplicates from the folder ~/Music.
# The last encounter (last modified time) of a file for which a duplicate exists is considered as the original.
lsdpl --remove-first ~/Music# Remove all duplicates from the folder ~/Music.
# The last encounter of a file for which a duplicate exists is considered as the original. Eventually all symlinks
# that are orphaned are removed.
lsdpl --remove-first --remove-orphaned-symlinks ~/Music
# The last encounter of a file for which a duplicate exists is considered as the original. Eventually all directories
# (including ~/Music) that are empty are removed.
lsdpl --remove-first --remove-empty-directories ~/Music
# Remove all duplicates from the folder ~/Music. And print every rm operation on stdout.
lsdpl --remove-first --verbose ~/Music# Remove all duplicates from the folder ~/Music.

```
  
# Dependencies
- [cmake 3.13](https://cmake.org/)
- [boost filesystem](https://www.boost.org/doc/libs/1_68_0/libs/filesystem/doc/index.htm)
- [OpenSSL (crypto)](https://github.com/openssl/openssl)
- [gflags](https://github.com/gflags/gflags)

# Installation
1. Install a C++ compiler (developer with [clang++-6.0](http://clang.llvm.org/))
2. Install all dependencies
3. git clone https://github.com/lgeorgieff/lsdpl.git
4. cd lsdpl
5. mkdir build && cd build
6. cmake ..
7. make

# License
This project is licensed under [BSD 3-Clause license](./LICENSE.txt).
