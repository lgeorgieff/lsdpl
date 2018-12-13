#pragma once

#include "scan_fs.hpp"

#include <iterator>

template<typename HASH>
lsdpl::scan_fs<HASH>::scan_fs(const std::string &path)
    :hashes_{1024}, queued_paths_{} {
    queued_paths_.push(boost::filesystem::absolute(path).normalize());
}

template<typename HASH>
lsdpl::scan_fs<HASH>::scan_fs(const std::vector<std::string> &paths)
        :hashes_{1024}, queued_paths_{} {
    std::for_each(paths.begin(), paths.end(), [this](const auto &path){
         queued_paths_.push(boost::filesystem::absolute(path).normalize());
    });
}

template<typename HASH>
void lsdpl::scan_fs<HASH>::start() {

}