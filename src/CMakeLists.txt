cmake_minimum_required(VERSION 3.13)
project(lsdpl)

set(CMAKE_CXX_STANDARD 17)

add_executable(lsdpl
        main.cpp
        file_hash.cpp file_hash.hpp
        scan_fs.hpp
        rm_first.hpp
        rm_last.hpp)

find_package(Boost COMPONENTS system filesystem REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(gflags REQUIRED)
target_link_libraries(lsdpl
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
        OpenSSL::Crypto
        gflags)

include_directories (${gflags_INCLUDE_DIR})