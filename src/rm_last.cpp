#pragma once

#include "rm_last.hpp"

template<typename HASH>
lsdpl::rm_last<HASH>::rm_last(const boost::filesystem::path &path, bool remove_orphaned_symlinks,
        bool remove_empty_directories, bool suppress_errors)
        :scan_fs<HASH>{path, remove_orphaned_symlinks, remove_empty_directories, suppress_errors} {}

template<typename HASH>
lsdpl::rm_last<HASH>::rm_last(const std::vector<boost::filesystem::path> &paths, bool remove_orphaned_symlinks,
        bool remove_empty_directories, bool suppress_errors)
        :scan_fs<HASH>{paths, remove_orphaned_symlinks, remove_empty_directories, suppress_errors} {}

template<typename HASH>
void lsdpl::rm_last<HASH>::file_operation(const boost::filesystem::path &file_path, const std::time_t &last_modified,
        const std::string &hash) noexcept {
    auto original{scan_fs<HASH>::hashes_.find(hash)};
    if (original == scan_fs<HASH>::hashes_.end()) {
        scan_fs<HASH>::hashes_[hash] = get_timestamp(file_path, this->is_suppress_errors());
    } else {
        auto to_be_deleted{file_path};
        try {
            if(last_modified >= original->second.second) {
                boost::filesystem::remove(file_path);
            } else {
                to_be_deleted = original->second.first;
                original->second = std::pair{file_path, last_modified};
                boost::filesystem::remove(to_be_deleted);
            }
        } catch(const boost::filesystem::filesystem_error &error) {
            if(!this->is_suppress_errors()) print_error("Could not remove duplicate file", to_be_deleted, error);
        }
    }
}