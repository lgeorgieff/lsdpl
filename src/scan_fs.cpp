#pragma once

#include "scan_fs.hpp"

#include <iostream>

template<typename HASH>
lsdpl::scan_fs<HASH>::scan_fs(const std::string &path, bool suppress_errors)
    :hashes_{1024}, queued_paths_{}, suppress_errors_{suppress_errors} {
    queued_paths_.push(boost::filesystem::absolute(path).normalize());
}

template<typename HASH>
lsdpl::scan_fs<HASH>::scan_fs(const std::vector<std::string> &paths, bool suppress_errors)
        :hashes_{1024}, queued_paths_{}, suppress_errors_{suppress_errors} {
    std::for_each(paths.begin(), paths.end(), [this](const auto &path){
         queued_paths_.push(boost::filesystem::absolute(path).normalize());
    });
}

template<typename HASH>
void lsdpl::scan_fs<HASH>::file_operation(const boost::filesystem::path &file_path, const std::string &hash) noexcept {
    auto original{hashes_.find(hash)};
    if (original == hashes_.end()) hashes_[hash] = file_path;
    else std::cout << file_path.string() << " -> " << original->second.string() << std::endl;
}

template<typename HASH>
bool lsdpl::scan_fs<HASH>::suppress_errors() const noexcept { return suppress_errors_; }

template<typename HASH>
void lsdpl::scan_fs<HASH>::start() {
    HASH file_hash;
    while(!queued_paths_.empty()) {
        boost::filesystem::path path{queued_paths_.top()};
        queued_paths_.pop();
        if(boost::filesystem::is_symlink(path)) {
            // If the file is a symlink we don't compare it to other files but ignore it
            continue;
        } else if(boost::filesystem::is_directory(path)) {
            // We are not interested in directories but in files only, so we ignore it
            boost::filesystem::directory_iterator dir_iter, dir_end;
            try {
                dir_iter = boost::filesystem::directory_iterator{path};
            } catch(const boost::filesystem::filesystem_error &err) {
                dir_iter = dir_end;
                if(!suppress_errors_) std::cerr << "Could not read directory " << path.string() << std::endl;
            }
            for(; dir_iter != dir_end; ++dir_iter) queued_paths_.push(dir_iter->path());
            continue;
        } else if(boost::filesystem::is_regular_file(path)) {
            auto hash{file_hash(path)};
            if(hash.empty()) {
                if(!suppress_errors_) std::cerr << "Could not create hash for " << path.string() << std::endl;
            } else {
                file_operation(path, hash);
            }
        }
    }
}