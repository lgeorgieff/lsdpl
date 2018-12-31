#include "path_entry.hpp"
#include "utils.hpp"

lsdpl::path_entry::path_entry(const boost::filesystem::path &path, bool suppress_errors)
:path_{path}, last_modified_{} {
    try {
        last_modified_ = boost::filesystem::last_write_time(path_);
    } catch (const boost::filesystem::filesystem_error &error) {
        if(!suppress_errors) print_error("Could not read last time modified", path, error);
    }
}

const boost::filesystem::path& lsdpl::path_entry::path() const noexcept { return path_; }

const std::time_t& lsdpl::path_entry::last_modified() const noexcept { return last_modified_; }

bool lsdpl::path_entry::operator<(const path_entry &other) const noexcept {
    return (last_modified_ == other.last_modified_ && path_ < other.path_) || last_modified_ < other.last_modified_;
}
