set(BACKEND_SOURCES
    backend.cpp
)

add_executable(Backend ${BACKEND_SOURCES})

# Link the main project library
target_link_libraries(Backend PRIVATE smart_eye_lib)
