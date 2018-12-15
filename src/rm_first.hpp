#pragma once

#include "scan_fs.hpp"
#include "boost/filesystem.hpp"

namespace lsdpl {
    template<typename HASH>
    class rm_first : public scan_fs<HASH> {
    public:
        explicit rm_first(const std::string &path, bool suppress_errors = false);
        explicit rm_first(const std::vector<std::string> &paths, bool suppress_errors = false);

        virtual void file_operation(const boost::filesystem::path &file_path, const std::string &hash) noexcept
            override;
    }; // class rm_first
} // namespace lsdpl

#include "rm_first.cpp"