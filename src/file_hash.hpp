#pragma once

#include "boost/filesystem.hpp"
#include <string>
#include <cstddef>

namespace lsdpl {
    class file_hash {
    public:
        static constexpr size_t BUFFER_SIZE{1024 * 8};
        std::string operator()(const boost::filesystem::path &file_path) const noexcept;
    }; // class file_hash
} // namespace lsdpl