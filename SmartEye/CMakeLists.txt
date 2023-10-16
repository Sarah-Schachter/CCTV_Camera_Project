cmake_minimum_required (VERSION 3.16.0)

project ("SmartEye")

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(CodeFiles)

add_subdirectory(sqlite_files)

add_subdirectory(spdlog)

add_subdirectory(Tests)