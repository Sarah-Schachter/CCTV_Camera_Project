set(CAMERA_SOURCES
    camera.cpp
)

add_executable(Camera ${CAMERA_SOURCES})

# Link the main project library
target_link_libraries(Camera PRIVATE smart_eye_lib)
