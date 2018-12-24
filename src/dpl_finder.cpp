#pragma once

#include "dpl_finder.hpp"

#include <algorithm>

template<typename HASH>
lsdpl::dpl_finder<HASH>::dpl_finder(const boost::filesystem::path &path, bool remove_orphaned_symlinks,
        bool remove_empty_directories, bool verbose, bool suppress_errors)
        :scan_fs<HASH>{path, remove_orphaned_symlinks, remove_empty_directories, verbose, suppress_errors} {}

template<typename HASH>
lsdpl::dpl_finder<HASH>::dpl_finder(const std::vector<boost::filesystem::path> &paths, bool remove_orphaned_symlinks,
        bool remove_empty_directories, bool verbose, bool suppress_errors)
        :scan_fs<HASH>{paths, remove_orphaned_symlinks, remove_empty_directories, verbose, suppress_errors} {}

template<typename HASH>
void lsdpl::dpl_finder<HASH>::file_operation(path_entry &file_path, std::string &hash) noexcept {
    auto original{hashes_.find(hash)};
    if (original == hashes_.end()) hashes_.insert(std::pair{hash, std::set<path_entry>{file_path}});
    else original->second.insert(file_path);
}

template<typename HASH>
void lsdpl::dpl_finder<HASH>::print(std::ostream &destination) const noexcept {
    std::for_each(hashes_.begin(), hashes_.end(), [&destination](const auto &files){
        if(files.second.size() > 1) {
            auto files_iter{files.second.begin()};
            destination << files_iter->path().string() << std::endl;
            std::for_each(++files_iter, files.second.end(), [&destination](const auto &file){
                destination << " - " << file.path().string() << std::endl;
            });
        }
    });
}