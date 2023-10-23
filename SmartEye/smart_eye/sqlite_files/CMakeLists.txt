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

add_library(SQLITE ${SQLITE_SOURCES} ${SQLITE_HEADERS})

