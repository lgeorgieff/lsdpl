#include "utils.hpp"

#include <iostream>

bool lsdpl::is_symlink_broken(const boost::filesystem::path &p) {
    auto target{boost::filesystem::read_symlink(p)};
    if(target.is_relative()) {
        boost::filesystem::path parent;
        if(p.has_parent_path()) parent = p.parent_path();
        else if(p.has_root_path()) parent = p.root_path();
        else return false;
        target = boost::filesystem::absolute(target, parent);
    }

    if(boost::filesystem::is_symlink(target)) {
        if(target == p) return true;
        return is_symlink_broken(target);
    } else {
        return !boost::filesystem::exists(target);
    }
}

void lsdpl::print_error(const std::string &message, const boost::filesystem::path &path,
        const std::exception &error) noexcept {
    std::cerr << message << " [" << path.string() << "]" << " [" << error.what() << "]" << std::endl;
}

void lsdpl::print_error(const std::string &message, const boost::filesystem::path &path) noexcept {
    std::cerr << message << " [" << path.string() << "]" << std::endl;
}

void lsdpl::print_verbose(operation op, const boost::filesystem::path &path) noexcept {
    switch(op) {
        case operation::rm:
            std::cout << "rm";
            break;
        case operation::rmdir:
            std::cout << "rmdir";
            break;
    }
    std::cout << " " << path.string() << std::endl;
}