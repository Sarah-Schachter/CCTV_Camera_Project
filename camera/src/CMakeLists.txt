# Add source files
set(SOURCES
    CalcAvarage.cpp
    CameraFunctions.cpp
    Program.cpp
    YOLOdetection.cpp
    Test.cpp
)

# Add header files
set(HEADERS
    CalcAvarage.h
    CameraFunctions.h
    Detection.h
    YOLOdetection.h
    framework.h
)

# Create the executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} "../sqlite_files/sqlite3.c" Test.cpp)
# Find and include OpenCV
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
# Link OpenCV libraries
target_link_libraries(${PROJECT_NAME} PUBLIC ${OpenCV_LIBS})

# Find Catch2 package
find_package(Catch2 REQUIRED)

# Include OpenCv
if(WIN32)
    set(OpenCV_DIR "C:\\opencv\\build")
endif()
link_directories(${OpenCV_LIBRARY_DIRS})


