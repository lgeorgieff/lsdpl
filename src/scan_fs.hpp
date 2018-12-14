#pragma once

#include <unordered_map>
#include <vector>
#include <stack>
#include "boost/filesystem.hpp"

namespace lsdpl {
    template<typename HASH>
    class scan_fs {
    public:
        explicit scan_fs(const std::string &path, bool suppress_errors = false);
        explicit scan_fs(const std::vector<std::string> &paths, bool suppress_errors = false);

        virtual void start();
        bool suppress_errors() const noexcept;
    protected:
        std::unordered_map<std::string, boost::filesystem::path> hashes_;
        std::stack<boost::filesystem::path> queued_paths_;

    private:
        bool suppress_errors_;
    }; // class scan_fs
} // namespace lsdpl

#include "scan_fs.cpp"
