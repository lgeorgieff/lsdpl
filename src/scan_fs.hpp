#pragma once

#include <unordered_map>
#include <vector>
#include <stack>
#include "boost/filesystem.hpp"

namespace lsdpl {
    template<typename HASH>
    class scan_fs {
    public:
        explicit scan_fs(const std::string &path);
        explicit scan_fs(const std::vector<std::string> &paths);

        virtual void start();
    protected:
        std::unordered_map<std::string, boost::filesystem::path> hashes_;
        std::stack<boost::filesystem::path> queued_paths_;
    }; // class scan_fs
} // namespace lsdpl

#include "scan_fs.cpp"
