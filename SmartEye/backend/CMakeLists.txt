set(BACKEND_SOURCES
    backend.cpp
)
set(BACKEND_HEADERS
    backend.h
)

add_executable(Backend ${BACKEND_SOURCES} ${BACKEND_HEADERS})

# Link the main project library
target_link_libraries(Backend PRIVATE smart_eye_lib)
