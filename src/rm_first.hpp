#pragma once

#include "scan_fs.hpp"
#include "boost/filesystem.hpp"

namespace lsdpl {
    template<typename HASH>
    class rm_first : public scan_fs<HASH> {
    public:
        explicit rm_first(const boost::filesystem::path &path, bool remove_orphaned_symlinks,
                bool remove_empty_directories, bool suppress_errors);
        explicit rm_first(const std::vector<boost::filesystem::path> &paths, bool remove_orphaned_symlinks,
                bool remove_empty_directories, bool suppress_errors);

        virtual void file_operation(const boost::filesystem::path &file_path, const std::time_t &last_modified,
                const std::string &hash) noexcept
            override;
    }; // class rm_first
} // namespace lsdpl

#include "rm_first.cpp"