#pragma once

#include "scan_fs.hpp"
#include "boost/filesystem.hpp"

namespace lsdpl {
    template<typename HASH>
    class rm_last : public scan_fs<HASH> {
    public:
        explicit rm_last(const std::string &path, bool suppress_errors = false);
        explicit rm_last(const std::vector<std::string> &paths, bool suppress_errors = false);

        virtual void file_operation(const boost::filesystem::path &file_path, const std::string &hash) noexcept
            override;
    }; // class rm_last
} // namespace lsdpl

#include "rm_last.cpp"