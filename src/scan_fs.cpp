#pragma once

#include "scan_fs.hpp"

#include <stdexcept>
#include <iostream>

template<typename HASH>
lsdpl::scan_fs<HASH>::scan_fs(const boost::filesystem::path &path, bool remove_orphaned_symlinks,
        bool remove_empty_directories, bool verbose, bool suppress_errors)
        :scan_fs{{boost::filesystem::absolute(boost::filesystem::path{path}).normalize()}, remove_orphaned_symlinks,
                 remove_empty_directories, verbose, suppress_errors} {}
template<typename HASH>
lsdpl::scan_fs<HASH>::scan_fs(const std::vector<boost::filesystem::path> &paths, bool remove_orphaned_symlinks,
        bool remove_empty_directories, bool verbose, bool suppress_errors)
        :queued_paths_{}, remove_orphaned_symlinks_{remove_orphaned_symlinks},
        remove_empty_directories_{remove_empty_directories}, verbose_{verbose}, suppress_errors_{suppress_errors},
        symlinks_{}, traversed_directories_{} {
    std::vector<boost::filesystem::path> tmp_paths;
    std::for_each(paths.begin(), paths.end(), [this, &tmp_paths](const auto &path){
        try {
            tmp_paths.push_back(boost::filesystem::canonical(path));
        } catch (const boost::filesystem::filesystem_error &error) {
            if(!is_suppress_errors()) print_error("Could not read", path, error);
        }
    });
    std::sort(tmp_paths.begin(), tmp_paths.end());
    tmp_paths.erase(std::unique(tmp_paths.begin(), tmp_paths.end()), tmp_paths.end());

    std::for_each(tmp_paths.begin(), tmp_paths.end(), [this](const auto &path){
         queued_paths_.push(path_entry{path, is_suppress_errors()});
    });
}

template<typename HASH>
void lsdpl::scan_fs<HASH>::remove_orphaned_symlinks() noexcept {
    for(const auto &symlink : symlinks_) {
        try {
            if (is_symlink_broken(symlink)) {
                try {
                    boost::filesystem::remove(symlink);
                    print_verbose(operation::rm, symlink);
                } catch (const boost::filesystem::filesystem_error &error) {
                    if (!is_suppress_errors()) print_error("Could not remove orphaned symlink", symlink, error);
                }
            }
        } catch (const boost::filesystem::filesystem_error &error) {
            if(!is_suppress_errors()) print_error("Could not remove symlink", symlink, error);
        }
    }
}

template<typename HASH>
void lsdpl::scan_fs<HASH>::remove_empty_directories() noexcept {
    while(!traversed_directories_.empty()) {
        const auto dir{traversed_directories_.top()};
        traversed_directories_.pop();
        try {
            if(boost::filesystem::is_empty(dir)) {
                boost::filesystem::remove(dir);
                print_verbose(operation::rmdir, dir);
            }
        } catch(const boost::filesystem::filesystem_error &error) {
            if(!is_suppress_errors()) print_error("Could not remove directory", dir, error);
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
bool lsdpl::scan_fs<HASH>::is_verbose() const noexcept { return verbose_; }

template<typename HASH>
void lsdpl::scan_fs<HASH>::travers_fs() noexcept {
    HASH file_hash;
    while(!queued_paths_.empty()) {
        auto path{queued_paths_.top()};
        queued_paths_.pop();
        if(boost::filesystem::is_symlink(path.path())) {
            if(is_remove_orphaned_symlinks()) symlinks_.push_front(path.path());
        } else if(boost::filesystem::is_directory(path.path())) {
            // We are not interested in directories but in files only, so we ignore it
            boost::filesystem::directory_iterator dir_iter, dir_end;
            try {
                dir_iter = boost::filesystem::directory_iterator{path.path()};
            } catch(const boost::filesystem::filesystem_error &error) {
                dir_iter = dir_end;
                if(!is_suppress_errors()) print_error("Could not read directory", path.path(), error);
            }
            for(; dir_iter != dir_end; ++dir_iter) {
                queued_paths_.push(path_entry{boost::filesystem::path{dir_iter->path()}.normalize(),
                                   is_suppress_errors()});
            }
            if(is_remove_empty_directories()) traversed_directories_.push(path.path());
            continue;
        } else if(boost::filesystem::is_regular_file(path.path())) {
            auto hash{file_hash(path.path())};
            if(hash.empty()) {
                if(!is_suppress_errors()) print_error("Could not create hash", path.path());
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
