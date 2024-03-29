cmake_minimum_required(VERSION 3.0.0)
project(socket_client)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 11)

if (WIN32)
    set(boost "C:\\msys64\\mingw64\\include")
    add_definitions(-D _WIN32_WINNT=0x0501)
else ()
    set(boost "/opt/homebrew/include")
endif ()

if (${CMAKE_BUILD_TYPE} STREQUAL Debug)
    set(DEBUG_MODE DEBUG_MODE)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/runtime)
    link_directories("/opt/homebrew/lib" ${PROJECT_SOURCE_DIR}/lib)
else ()
    set(DEBUG_MODE RELEASE_MODE)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/relruntime)
    link_directories("/opt/homebrew/lib" ${PROJECT_SOURCE_DIR}/lib)
endif ()

include_directories(${boost} ${PROJECT_SOURCE_DIR}/include)
file(GLOB_RECURSE SOURCE "include/*.hpp" "source/*.cpp" "source/*.c"  )
#add_subdirectory(progress)
#add_subdirectory(${PROJECT_SOURCE_DIR}/clientLib)
add_executable(socket_client  ${SOURCE} main.cpp include/Error.hpp include/SocketClient.h)
target_link_libraries(socket_client PUBLIC cpp_Handler event wsock32 ws2_32)