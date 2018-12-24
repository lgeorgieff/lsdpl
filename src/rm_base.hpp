#pragma once

#include "scan_fs.hpp"

namespace lsdpl {
    template<typename HASH>
    class rm_base : public scan_fs<HASH> {
    public:
        rm_base(const boost::filesystem::path &path, bool remove_orphaned_symlinks,
                bool remove_empty_directories, bool verbose, bool suppress_errors);
        rm_base(const std::vector<boost::filesystem::path> &paths, bool remove_orphaned_symlinks,
                bool remove_empty_directories, bool verbose, bool suppress_errors);

    protected:
        std::unordered_map<std::string, path_entry> hashes_;
    }; // class rm_base
} // namespace lsdpl

#include "rm_base.cpp"