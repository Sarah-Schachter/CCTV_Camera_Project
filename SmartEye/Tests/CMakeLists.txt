set(TEST_SOURCES
    Test.cpp
)

add_executable(Tests ${TEST_SOURCES})


# Link the main project library
target_link_libraries(Tests PRIVATE CodeFiles)
