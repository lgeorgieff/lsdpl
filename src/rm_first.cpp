#pragma once

#include "rm_first.hpp"

template<typename HASH>
lsdpl::rm_first<HASH>::rm_first(const boost::filesystem::path &path, bool remove_orphaned_symlinks,
        bool suppress_errors)
        :scan_fs<HASH>{path, remove_orphaned_symlinks, suppress_errors} {}

template<typename HASH>
lsdpl::rm_first<HASH>::rm_first(const std::vector<boost::filesystem::path> &paths, bool remove_orphaned_symlinks,
        bool suppress_errors)
        :scan_fs<HASH>{paths, remove_orphaned_symlinks, suppress_errors} {}

template<typename HASH>
void lsdpl::rm_first<HASH>::file_operation(const boost::filesystem::path &file_path, const std::string &hash) noexcept {
    auto original{scan_fs<HASH>::hashes_.find(hash)};
    if (original == scan_fs<HASH>::hashes_.end()) {
        scan_fs<HASH>::hashes_[hash] = file_path;
    } else {
        try {
            boost::filesystem::remove(original->second);
            scan_fs<HASH>::hashes_[hash] = file_path;
        } catch(const boost::filesystem::filesystem_error &error) {
            if(!scan_fs<HASH>::suppress_errors()) {
                std::cerr << "Could not remove original file " << original->second.string() << std::endl;
            }
        }
        original->second = file_path;
    }
}