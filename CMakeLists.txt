cmake_minimum_required(VERSION 3.13)
project(lsdpl)

set(CMAKE_CXX_STANDARD 17)

add_executable(lsdpl
        src/main.cpp
        src/file_hash.cpp src/file_hash.hpp
        src/scan_fs.hpp
        src/rm_first.hpp
        src/rm_last.hpp)

find_package(Boost COMPONENTS system filesystem REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(gflags REQUIRED)
target_link_libraries(lsdpl
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
        OpenSSL::Crypto
        gflags)

include_directories (${gflags_INCLUDE_DIR})