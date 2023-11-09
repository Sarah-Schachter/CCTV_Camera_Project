# Add source files
set(SOURCES
    calcAvarage.cpp
    cameraFunctions.cpp
    YOLOdetection.cpp
    threadSafeQueue.cpp
    logger.cpp
)

# Add header files
set(HEADERS
    calcAvarage.h
    cameraFunctions.h
    detection.h
    YOLOdetection.h
    threadSafeQueue.h
    logger.h
)
find_package(Threads)

# Create the shared library
add_library(smart_eye_lib SHARED STATIC ${SOURCES} ${HEADERS} )

add_subdirectory(sqlite_files)

target_link_libraries(smart_eye_lib PRIVATE sqlite_lib)

target_link_libraries(smart_eye_lib PUBLIC proto_lib)


##################### opencv #######################
if(WIN32)
    set(OpenCV_DIR "C:\\opencv\\build")  
endif()
link_directories(${OpenCV_LIBRARY_DIRS})
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(smart_eye_lib PUBLIC ${OpenCV_LIBS})


#################### spdlog #######################
if(WIN32)
    set(SPDLOG_DIR "C:\\spdlog\\include")
    target_include_directories(smart_eye_lib PUBLIC ${SPDLOG_DIR})
    #include_directories([AFTER|BEFORE] [SYSTEM] ${SPDLOG_DIR})
else()
    set(SPDLOG_DIR "/mnt/c/spdlog/include")
    target_include_directories(smart_eye_lib PUBLIC ${SPDLOG_DIR})
    #find_package(spdlog REQUIRED)
    #target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog)
endif()


###################add proto#################
# find_package(protobuf CONFIG REQUIRED)
# find_package(gRPC CONFIG REQUIRED)
# 
# set(PROTO_DIR ${CMAKE_CURRENT_SOURCE_DIR} "../proto_lib")
# 
# set(PROTOS ${PROTO_DIR} camera_proto.proto)




# Generate protobuf files
# protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTOS})
# Generate gRPC files
# protobuf_generate_grpc_cpp(GRPC_SRCS GRPC_HDRS ${PROTOS})

# target_link_libraries(smart_eye_lib ${PROTOBUF_LIBRARY} gRPC::grpc++)
# target_link_libraries(smart_eye_lib
#     PUBLIC
#         proto_lib
# )

# target_link_libraries(smart_eye_lib PRIVATE 
#     protobuf::libprotobuf
#     gRPC::grpc
#     gRPC::grpc++
# )
