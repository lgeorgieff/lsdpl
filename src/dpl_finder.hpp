#pragma once

#include "scan_fs.hpp"
#include "path_entry.hpp"

#include <unordered_map>
#include <string>
#include <set>
#include <iostream>

namespace lsdpl {
    template<typename HASH>
    class dpl_finder : public scan_fs<HASH> {
    public:
        dpl_finder(const boost::filesystem::path &path, bool remove_orphaned_symlinks,
                bool remove_empty_directories, bool verbose, bool suppress_errors);
        dpl_finder(const std::vector<boost::filesystem::path> &paths, bool remove_orphaned_symlinks,
                bool remove_empty_directories, bool verbose, bool suppress_errors);

        virtual void file_operation(path_entry &file_path, std::string &hash) noexcept override;
        void print(std::ostream &destination) const noexcept;
    private:
        std::unordered_map<std::string, std::set<path_entry>> hashes_;
    }; // class dpl_finder
} // namespace lsdpl

#include "dpl_finder.cpp"