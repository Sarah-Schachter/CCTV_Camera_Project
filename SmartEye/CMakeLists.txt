cmake_minimum_required (VERSION 3.16.0)

project (SmartEye)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(smart_eye)

add_subdirectory(tests)

add_subdirectory(camera)

add_subdirectory(backend)


