#find_library(SQLITE3_LIBRARY sqlite3)

#if(NOT SQLITE3_LIBRARY)
#   message(FATAL_ERROR "SQLite3 library not found")
#endif()

# Define your target (library or executable) and link the sqlite3 library
#add_library(SqliteLibrary STATIC
#    shell.c
#    sqlite3.c
    # Add any other source files here
#)

#target_link_libraries(SqliteLibrary PRIVATE
#    ${SQLITE3_LIBRARY}
#)
