set(CAMERA_SOURCES
    camera.cpp
    cameraClient.cpp
)

set(CAMERA_HEADERS
    cameraClient.h
)

add_executable(Camera ${CAMERA_SOURCES} ${CAMERA_HEADERS})

# Link the main project library
target_link_libraries(Camera PRIVATE smart_eye_lib)
