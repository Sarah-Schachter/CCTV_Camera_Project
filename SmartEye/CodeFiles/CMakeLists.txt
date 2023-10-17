# Add source files
set(SOURCES
    CalcAvarage.cpp
    CameraFunctions.cpp
    Program.cpp
    YOLOdetection.cpp
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
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} "sqlite_files/sqlite3.c")

add_library(CodeFiles ${SOURCES} ${HEADERS})

add_subdirectory(sqlite_files)


##################### opencv #######################

if(WIN32)
    set(OpenCV_DIR "C:\\opencv\\build")
endif()

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
link_directories(${OpenCV_LIBRARY_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC ${OpenCV_LIBS})
