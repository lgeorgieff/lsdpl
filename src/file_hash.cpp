#include "file_hash.hpp"

#include "openssl/sha.h"
#include <string>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <iomanip>

std::string lsdpl::file_hash::operator()(const std::string &file_path) const noexcept {
    char file_buffer[BUFFER_SIZE];
    unsigned char digest[SHA_DIGEST_LENGTH];
    std::stringstream ss;
    std::string sha1_string;
    std::ifstream file(file_path, std::ifstream::binary);
    SHA_CTX sha1Context;
    SHA1_Init(&sha1Context);

    // The file does not exist or could not be opened
    if(!file.good()) return "";

    while (file.good()) {
        file.read(file_buffer, BUFFER_SIZE);
        SHA1_Update(&sha1Context, file_buffer, file.gcount());
    }
    file.close();

    int result{SHA1_Final(digest, &sha1Context)};
    if(result == 0) return "";
    ss << std::hex << std::uppercase << std::setfill('0');

    for(unsigned char uc: digest) ss << std::setw(2) << (int)uc;
    sha1_string = ss.str();
    return sha1_string;
}