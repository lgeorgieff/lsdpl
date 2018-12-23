#pragma once

#include "path_entry.hpp"
#include "utils.hpp"

#include <unordered_map>
#include <vector>
#include <stack>
#include <forward_list>
#include <stack>
#include <cstddef>
#include <ctime>
#include "boost/filesystem.hpp"

namespace lsdpl {
    template<typename HASH>
    class scan_fs {
    public:
        explicit scan_fs(const boost::filesystem::path &path, bool remove_orphaned_symlinks,
                bool remove_empty_directories, bool verbose, bool suppress_errors);
        explicit scan_fs(const std::vector<boost::filesystem::path> &paths, bool remove_orphaned_symlinks,
                bool remove_empty_directories, bool verbose, bool suppress_errors);

        virtual void file_operation(path_entry &file_path, std::string &hash) noexcept;
        void start() noexcept;
        bool is_suppress_errors() const noexcept;
        bool is_remove_orphaned_symlinks() const noexcept;
        bool is_remove_empty_directories() const noexcept;
        bool is_verbose() const noexcept;
    protected:
        std::unordered_map<std::string, path_entry> hashes_;
        std::stack<path_entry> queued_paths_;

    private:
        bool remove_orphaned_symlinks_;
        bool remove_empty_directories_;
        bool verbose_;
        bool suppress_errors_;
        std::forward_list<boost::filesystem::path> symlinks_;
        std::stack<boost::filesystem::path> traversed_directories_;

        void travers_fs() noexcept;
        void remove_orphaned_symlinks() noexcept;
        void remove_empty_directories() noexcept;
    }; // class scan_fs
} // namespace lsdpl

#include "scan_fs.cpp"
