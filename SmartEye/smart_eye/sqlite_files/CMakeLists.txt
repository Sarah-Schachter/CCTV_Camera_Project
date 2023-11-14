# Add source files
set(SQLITE_SOURCES
    shell.c
    sqlite3.c
)

# Add header files
set(SQLITE_HEADERS
    sqlite3.h
    sqlite3ext.h
)

# Create a static library from the source files
add_library(sqlite_lib STATIC ${SQLITE_SOURCES} ${SQLITE_HEADERS})
