set(CMAKE_SYSTEM_NAME Linux)

set(CMAKE_C_COMPILER $CROSS_C_COMPILER)
set(CMAKE_CXX_COMPILER $CROSS_CXX_COMPILER)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
