set(TEST_SOURCES
    calcAverageTest.cpp
    sameFrameTest.cpp
    threadSafeQueueTest.cpp
)

add_executable(Tests ${TEST_SOURCES} )

# Link the main project library
target_link_libraries(Tests PUBLIC smart_eye_lib)
