#include "file_hash.hpp"
#include "scan_fs.hpp"
#include "rm_first.hpp"

#include <iostream>
#include "boost/filesystem.hpp"
#include "gflags/gflags.h"

DEFINE_bool(remove_first, false, "Remove (always) the first occurrence of a duplicate");
DEFINE_bool(remove_last, false, "Remove (always) the last occurrence of a duplicate");
DEFINE_bool(suppress_errors, false, "Suppress error messages (permission denied, ...)");

int main(int argc, char **argv) {
    gflags::SetUsageMessage("Lists (and removes) all duplicate files based on their content");
    gflags::SetVersionString("0.0.1");
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    if(FLAGS_remove_first && FLAGS_remove_last) {
        std::cerr << "ERROR: remove_first and remove_last must not be set concurrently" << std::endl;
        return 1;
    }

    // Get the target directories or use cwd if no once is specified
    std::vector<std::string> paths;
    paths.reserve(argc - 1);
    for(int i{1}; i < argc; ++i) paths.push_back(argv[i]);
    if(argc == 1) paths.push_back(".");

    if(FLAGS_remove_first) {
        lsdpl::rm_first<lsdpl::file_hash> rm_first{paths, FLAGS_suppress_errors};
        rm_first.start();
    } else if(FLAGS_remove_last) {
        // TODO: implement
    } else {
        lsdpl::scan_fs<lsdpl::file_hash> scan_fs{paths, FLAGS_suppress_errors};
        scan_fs.start();
    }

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
        + start
        // no other constructors
        // no assignment operators
    }

    template<typename file_hash>
    class rm_first : public scan_fs {
        - rm_duplicate(const std::string &original_file_path, const std::string &duplicate_file_path)
    }

    template<typename file_hash>
    class rm_last : public scan_fs {
        - rm_duplicate(const std::string &original_file_path, const std::string &duplicate_file_path)
    }

 */