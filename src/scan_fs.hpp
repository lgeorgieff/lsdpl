#pragma once

#include <unordered_map>
#include <vector>
#include <stack>
#include <forward_list>
#include "boost/filesystem.hpp"

namespace lsdpl {
    template<typename HASH>
    class scan_fs {
    public:
        explicit scan_fs(const boost::filesystem::path &path, bool remove_orphaned_symlinks,
                bool suppress_errors = false);
        explicit scan_fs(const std::vector<boost::filesystem::path> &paths, bool remove_orphaned_symlinks,
                bool suppress_errors = false);

        virtual void file_operation(const boost::filesystem::path &file_path, const std::string &hash) noexcept;
        void start() noexcept;
        bool suppress_errors() const noexcept;
    protected:
        std::unordered_map<std::string, boost::filesystem::path> hashes_;
        std::stack<boost::filesystem::path> queued_paths_;

    private:
        bool remove_orphaned_synmlinks_;
        bool suppress_errors_;
        std::forward_list<boost::filesystem::path> symlinks_;

        void travers_fs() noexcept;
        void remove_orphaned_symlinks() noexcept;
    }; // class scan_fs
} // namespace lsdpl

#include "scan_fs.cpp"
