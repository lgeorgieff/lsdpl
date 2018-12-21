#pragma once

#include "scan_fs.hpp"

#include <iostream>

namespace {
    bool is_symlink_broken(const boost::filesystem::path &p) {
        auto target{boost::filesystem::read_symlink(p)};
        if(target.is_relative()) {
            boost::filesystem::path parent;
            if(p.has_parent_path()) parent = p.parent_path();
            else if(p.has_root_path()) parent = p.root_path();
            else return false;
            target = boost::filesystem::absolute(target, parent);
        }

        if(boost::filesystem::is_symlink(target)) {
            if(target == p) return true;
            return is_symlink_broken(target);
        } else {
            return !boost::filesystem::exists(target);
        }
    }
} // anonymous namespace

template<typename HASH>
lsdpl::scan_fs<HASH>::scan_fs(const boost::filesystem::path &path, bool remove_orphaned_symlinks,
        bool remove_empty_directories, bool suppress_errors)
        :hashes_{1024}, queued_paths_{}, remove_orphaned_symlinks_{remove_orphaned_symlinks},
        remove_empty_directories_{remove_empty_directories}, suppress_errors_{suppress_errors}, symlinks_{},
        traversed_directories_{} {
    queued_paths_.push(boost::filesystem::absolute(boost::filesystem::path{path}).normalize());
}

template<typename HASH>
lsdpl::scan_fs<HASH>::scan_fs(const std::vector<boost::filesystem::path> &paths, bool remove_orphaned_symlinks,
        bool remove_empty_directories, bool suppress_errors)
        :hashes_{1024}, queued_paths_{}, remove_orphaned_symlinks_{remove_orphaned_symlinks},
        remove_empty_directories_{remove_empty_directories}, suppress_errors_{suppress_errors}, symlinks_{},
        traversed_directories_{} {
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
void lsdpl::scan_fs<HASH>::remove_orphaned_symlinks() noexcept {
    for(const auto &symlink : symlinks_) {
        try {
            if (is_symlink_broken(symlink)) {
                try {
                    boost::filesystem::remove(symlink);
                } catch (const boost::filesystem::filesystem_error &error) {
                    if (!is_suppress_errors()) {
                        std::cerr << "Could not remove orphaned symlink " << symlink.string() << " [" << error.what()
                        << "]" << std::endl;
                    }
                }
            }
        } catch (const boost::filesystem::filesystem_error &error) {
            if(!is_suppress_errors()) {
                std::cerr << "Could not remove symlink " << symlink.string() << " [" << error.what()
                << "]" << std::endl;
            }
        }
    }
}

template<typename HASH>
void lsdpl::scan_fs<HASH>::remove_empty_directories() noexcept {
    while(!traversed_directories_.empty()) {
        const auto dir{traversed_directories_.top()};
        traversed_directories_.pop();
        try {
            if(boost::filesystem::is_empty(dir)) boost::filesystem::remove(dir);
        } catch(const boost::filesystem::filesystem_error &error) {
            if(!is_suppress_errors())
                std::cerr << "Could not remove directory " << dir.string()<< " [" << error.what() << "]"  << std::endl;
        }
    }
}

template<typename HASH>
bool lsdpl::scan_fs<HASH>::is_suppress_errors() const noexcept { return suppress_errors_; }

template<typename HASH>
bool lsdpl::scan_fs<HASH>::is_remove_empty_directories() const noexcept { return remove_empty_directories_; }

template<typename HASH>
bool lsdpl::scan_fs<HASH>::is_remove_orphaned_symlinks() const noexcept { return remove_orphaned_symlinks_; }

template<typename HASH>
void lsdpl::scan_fs<HASH>::travers_fs() noexcept {
    HASH file_hash;
    while(!queued_paths_.empty()) {
        boost::filesystem::path path{queued_paths_.top()};
        queued_paths_.pop();
        if(boost::filesystem::is_symlink(path)) {
            if(is_remove_orphaned_symlinks()) symlinks_.push_front(path);
        } else if(boost::filesystem::is_directory(path)) {
            // We are not interested in directories but in files only, so we ignore it
            boost::filesystem::directory_iterator dir_iter, dir_end;
            try {
                dir_iter = boost::filesystem::directory_iterator{path};
            } catch(const boost::filesystem::filesystem_error &error) {
                dir_iter = dir_end;
                if(!is_suppress_errors()) {
                    std::cerr << "Could not read directory " << path.string() << " ["
                    << error.what() << "]" << std::endl;
                }
            }
            for(; dir_iter != dir_end; ++dir_iter)
                queued_paths_.push(boost::filesystem::path{dir_iter->path()}.normalize());
            if(is_remove_empty_directories()) traversed_directories_.push(path);
            continue;
        } else if(boost::filesystem::is_regular_file(path)) {
            auto hash{file_hash(path)};
            if(hash.empty()) {
                if(!is_suppress_errors()) std::cerr << "Could not create hash for " << path.string() << std::endl;
            } else {
                file_operation(path, hash);
            }
        }
    }
}

template<typename HASH>
void lsdpl::scan_fs<HASH>::start() noexcept {
    travers_fs();
    if(remove_orphaned_symlinks_) remove_orphaned_symlinks();
    if(remove_empty_directories_) remove_empty_directories();
}