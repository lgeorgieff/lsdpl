#include "file_hash.hpp"
#include "scan_fs.hpp"

#include <iostream>


int main() {
    lsdpl::file_hash hash;
    std::cout << hash("/tmp/test.txt") << std::endl;

    std::vector<std::string> paths{"/tmp", ".."};
    lsdpl::scan_fs<lsdpl::file_hash> scan_fs{paths};

    return 0;
}


/*
    class file_hash {
        size_t operator()(const string &file_path) const noexcept
    }

    template<typename file_hash>
    class scan_fs(path1, path2, ...) {
        - hash
        - unordered_map<hash, (regular) file path>
        + destructor
        + constructor(const string &paths...)
        + star
        // no other constructors
        // no assignment operators
    }

    template<typename file_hash>
    class rm_fs : public scan_fs {
        # rm_duplicate(const std::string &original_file_path, const std::string &duplicate_file_path)
    }

    template<typename file_hash>
    class rm_original : public rm_fs {
        - rm_duplicate(const std::string &original_file_path, const std::string &duplicate_file_path)
    }

    template<typename file_hash>
    class rm_duplicate : public rm_fs {
        - rm_duplicate(const std::string &original_file_path, const std::string &duplicate_file_path)
    }

 */