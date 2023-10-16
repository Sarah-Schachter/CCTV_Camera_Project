# # Add source files
# set(SQLITE_SOURCES
#     shell.c
#     sqlite3.c
# )
# 
# # Add header files
# set(SQLITE_HEADERS
#     sqlite3.h
#     sqlite3ext.h
# )
# 
# add_library(sqlite ${SQLITE_SOURCES} ${SQLITE_HEADERS})
# target_include_directories(sqlite PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
#  add_executable(${PROJECT_NAME} sqlite3.c)