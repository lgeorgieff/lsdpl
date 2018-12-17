# lsdpl

Searches for duplicate files based on their actual content. It can:
- list the duplicates
- remove the duplicates
  - either the first occurrence is deleted
  - or all first + 1 occurrences are deleted
  
# Depedencies
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
