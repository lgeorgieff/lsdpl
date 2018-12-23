#pragma once

#include <ctime>
#include "boost/filesystem.hpp"

namespace lsdpl {
    class path_entry {
    public:
        explicit path_entry(const boost::filesystem::path &path, bool suppress_errors);
        path_entry(const path_entry &src) = default;
        path_entry(path_entry &&src) = default;
        path_entry &operator=(const path_entry &src) = default;
        path_entry &operator=(path_entry &&src) = default;
        ~path_entry() = default;

        const boost::filesystem::path &path() const noexcept;
        const std::time_t &last_modified() const noexcept;
    private:
        boost::filesystem::path path_;
        std::time_t last_modified_;
    }; // class path_entry
} // namespace lsdpl