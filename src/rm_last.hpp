#pragma once

#include "utils.hpp"
#include "scan_fs.hpp"

#include "boost/filesystem.hpp"

namespace lsdpl {
    template<typename HASH>
    class rm_last : public scan_fs<HASH> {
    public:
        explicit rm_last(const boost::filesystem::path &path, bool remove_orphaned_symlinks,
                bool remove_empty_directories, bool suppress_errors);
        explicit rm_last(const std::vector<boost::filesystem::path> &paths, bool remove_orphaned_symlinks,
                bool remove_empty_directories, bool suppress_errors);

        virtual void file_operation(path_entry &file_path, std::string &hash) noexcept
            override;
    }; // class rm_last
} // namespace lsdpl

#include "rm_last.cpp"