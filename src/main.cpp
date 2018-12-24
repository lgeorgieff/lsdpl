#include "file_hash.hpp"
#include "dpl_finder.hpp"
#include "rm_first.hpp"
#include "rm_last.hpp"

#include <iostream>
#include <algorithm>
#include <vector>
#include "boost/filesystem.hpp"
#include "gflags/gflags.h"

DEFINE_bool(remove_first, false, "Remove (always) the first occurrence of a duplicate.");
DEFINE_bool(remove_last, false, "Remove (always) the last occurrence of a duplicate.");
DEFINE_bool(suppress_errors, false, "Suppress error messages (permission denied, ...)");
DEFINE_bool(remove_orphaned_symlinks, false, "Remove all orphaned symlinks.");
DEFINE_bool(remove_empty_directories, false, "Remove all empty directories.");
DEFINE_bool(verbose, false, "Enable verbose mode.");

int main(int argc, char **argv) {
    gflags::SetUsageMessage("Lists (and removes) all duplicate files based on their content");
    gflags::SetVersionString("0.2.0");
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    if(FLAGS_remove_first && FLAGS_remove_last) {
        std::cerr << "ERROR: remove_first and remove_last must not be set concurrently" << std::endl;
        return 1;
    }

    // Get the target directories or use cwd if no one is specified
    std::vector<boost::filesystem::path> paths;
    paths.reserve(argc - 1);
    for(int i{1}; i < argc; ++i) paths.push_back(boost::filesystem::absolute(std::string{argv[i]}).normalize());
    if(argc == 1) paths.push_back(".");

    if(FLAGS_remove_first) {
        lsdpl::rm_first<lsdpl::file_hash> rm_first{paths, FLAGS_remove_orphaned_symlinks,
                                                   FLAGS_remove_empty_directories, FLAGS_verbose,
                                                   FLAGS_suppress_errors};
        rm_first.start();
    } else if(FLAGS_remove_last) {
        lsdpl::rm_last<lsdpl::file_hash> rm_last{paths, FLAGS_remove_orphaned_symlinks, FLAGS_remove_empty_directories,
                                                 FLAGS_verbose, FLAGS_suppress_errors};
        rm_last.start();
    } else {
        lsdpl::dpl_finder<lsdpl::file_hash> dpl_finder{paths, FLAGS_remove_orphaned_symlinks,
                                                       FLAGS_remove_empty_directories, FLAGS_verbose,
                                                       FLAGS_suppress_errors};
        dpl_finder.start();
        dpl_finder.print(std::cout);
    }

    return 0;
}
