cmake_minimum_required(VERSION 3.25)
project(Hexis VERSION 1.0.0 LANGUAGES C CXX)

# Require C23 and C++20 standard
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable strict warnings
if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_C_COMPILER_ID MATCHES "Clang")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic -Werror")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Werror")
elseif(MSVC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W4 /WX")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /WX")
endif()

# Enable testing
enable_testing()

# Add subdirectories
add_subdirectory(libhexis)

# GUI and other parts will be added in later phases
add_subdirectory(gui)
add_subdirectory(tests/c)
