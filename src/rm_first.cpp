#pragma once

#include "rm_first.hpp"

template<typename HASH>
lsdpl::rm_first<HASH>::rm_first(const boost::filesystem::path &path, bool remove_orphaned_symlinks,
        bool remove_empty_directories, bool verbose, bool suppress_errors)
        :rm_base<HASH>{path, remove_orphaned_symlinks, remove_empty_directories, verbose, suppress_errors} {}

template<typename HASH>
lsdpl::rm_first<HASH>::rm_first(const std::vector<boost::filesystem::path> &paths, bool remove_orphaned_symlinks,
        bool remove_empty_directories, bool verbose, bool suppress_errors)
        :rm_base<HASH>{paths, remove_orphaned_symlinks, remove_empty_directories, verbose, suppress_errors} {}

template<typename HASH>
void lsdpl::rm_first<HASH>::file_operation(path_entry &file_path, std::string &hash) noexcept {
    auto original{rm_base<HASH>::hashes_.find(hash)};
    if (original == rm_base<HASH>::hashes_.end()) {
        this->hashes_.insert(std::pair{hash, file_path});
    } else {
        auto to_be_deleted{file_path};
        try {
            if(file_path.last_modified() > original->second.last_modified()) {
                to_be_deleted = original->second;
                original->second = file_path;
            }
            boost::filesystem::remove(to_be_deleted.path());
            print_verbose(operation::rm, to_be_deleted.path(), original->second.path());
        } catch(const boost::filesystem::filesystem_error &error) {
            if(!this->is_suppress_errors()) print_error("Could not remove original file", to_be_deleted.path(), error);
        }
    }
}