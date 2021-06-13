cmake_minimum_required(VERSION 3.16)
project(effect
        LANGUAGES CXX
        VERSION 1.0
)

set(CMAKE_CXX_FLAGS_DEBUG "-std=c++17 -Wall -Werror -g")
set(CMAKE_CXX_FLAGS_SANITIZED "-std=c++17 -Wall -Werror -g -fsanitize=address,undefined,leak")
set(CMAKE_CXX_FLAGS_RELEASE "-std=c++17 -Wall -Werror -g -O3")
mark_as_advanced(
        CMAKE_CXX_FLAGS_DEBUG
        CMAKE_CXX_FLAGS_SANITIZED
        CMAKE_CXX_FLAGS_RELEASE
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_VERBOSE_MAKEFILE on)

find_package(Boost 1.40 COMPONENTS program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

# Add cashing object files
find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
endif(CCACHE_FOUND)


add_executable(effect)
add_subdirectory(src)

target_link_libraries(effect LINK_PUBLIC ${Boost_LIBRARIES})