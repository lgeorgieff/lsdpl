#pragma once

#include "boost/filesystem.hpp"
#include <string>

namespace lsdpl {
    bool is_symlink_broken(const boost::filesystem::path &p);

    void print_error(const std::string &message, const boost::filesystem::path &path, const std::exception &error);

    void print_error(const std::string &message, const boost::filesystem::path &path);
} // namespace lsdpl