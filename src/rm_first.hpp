#pragma once

#include "utils.hpp"
#include "rm_base.hpp"

#include "boost/filesystem.hpp"

namespace lsdpl {
    template<typename HASH>
    class rm_first : public rm_base<HASH> {
    public:
        rm_first(const boost::filesystem::path &path, bool remove_orphaned_symlinks,
                bool remove_empty_directories, bool verbose, bool suppress_errors);
        rm_first(const std::vector<boost::filesystem::path> &paths, bool remove_orphaned_symlinks,
                bool remove_empty_directories, bool verbose, bool suppress_errors);

        virtual void file_operation(path_entry &file_path, std::string &hash) noexcept override;
    }; // class rm_first
} // namespace lsdpl

#include "rm_first.cpp"