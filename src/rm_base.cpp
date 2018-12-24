#pragma once

#include "rm_base.hpp"

template<typename HASH>
lsdpl::rm_base<HASH>::rm_base(const boost::filesystem::path &path, bool remove_orphaned_symlinks,
        bool remove_empty_directories, bool verbose, bool suppress_errors)
        :rm_base{{boost::filesystem::absolute(boost::filesystem::path{path}).normalize()}, remove_orphaned_symlinks,
                 remove_empty_directories, verbose, suppress_errors} {}

template<typename HASH>
lsdpl::rm_base<HASH>::rm_base(const std::vector<boost::filesystem::path> &paths, bool remove_orphaned_symlinks,
        bool remove_empty_directories, bool verbose, bool suppress_errors)
        :scan_fs<HASH>{paths, remove_orphaned_symlinks, remove_empty_directories, verbose, suppress_errors},
        hashes_{1024} {}
