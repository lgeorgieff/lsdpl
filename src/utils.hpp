#pragma once

#include "boost/filesystem.hpp"
#include <string>

namespace lsdpl {
    bool is_symlink_broken(const boost::filesystem::path &p);

    void print_error(const std::string &message, const boost::filesystem::path &path,
            const std::exception &error) noexcept;

    void print_error(const std::string &message, const boost::filesystem::path &path) noexcept;

    enum class operation {
        rm,
        rmdir
    };

    void print_verbose(operation op, const boost::filesystem::path &path) noexcept;
    void print_verbose(operation op, const boost::filesystem::path &path,
        const boost::filesystem::path &original) noexcept;
} // namespace lsdpl