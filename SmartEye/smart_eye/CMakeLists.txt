# Add source files
set(SOURCES
    calcAvarage.cpp
    cameraFunctions.cpp
    program.cpp
    YOLOdetection.cpp
    logger.cpp
    backendThread.cpp
     cameraThread.cpp
    threadSafeQueue.cpp


)

# Add header files
set(HEADERS
    calcAvarage.h
    cameraFunctions.h
    detection.h
    YOLOdetection.h
    framework.h
    logger.h
    backendThread.h
    cameraThread.h
    threadSafeQueue.h
)

# Create the executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} "sqlite_files/sqlite3.c")

add_library(SmartEye_lib ${SOURCES} ${HEADERS})

add_subdirectory(sqlite_files)


##################### opencv #######################

if(WIN32)
    set(OpenCV_DIR "C:\\opencv\\build")  
endif()
link_directories(${OpenCV_LIBRARY_DIRS})
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC ${OpenCV_LIBS})



##################### spdlog #######################

if(WIN32)
    set(SPDLOG_DIR "C:\\spdlog\\include")
    include_directories([AFTER|BEFORE] [SYSTEM] ${SPDLOG_DIR})
else()
    set(SPDLOG_DIR "/mnt/c/spdlog/include")
    include_directories([AFTER|BEFORE] [SYSTEM] ${SPDLOG_DIR})

    #find_package(spdlog REQUIRED)
    #target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog)
endif()


#set(SPDLOG_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR} "../spdlog/include")
#include_directories(${SPDLOG_INCLUDE_DIR})
