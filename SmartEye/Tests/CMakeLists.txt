set(TEST_SOURCES
    Test.cpp
)

add_executable(Tests ${TEST_SOURCES} "../CodeFiles/sqlite_files/sqlite3.c")


# Link the main project library
target_link_libraries(Tests PRIVATE CodeFiles)

##################### opencv #######################
if(WIN32)
    set(OpenCV_DIR "C:\\opencv\\build")
endif()
link_directories(${OpenCV_LIBRARY_DIRS})
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

target_link_libraries(Tests PUBLIC ${OpenCV_LIBS})