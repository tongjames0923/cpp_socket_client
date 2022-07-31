cmake_minimum_required(VERSION 3.0.0)
project(socket_client VERSION 0.1.0)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 11)

if(WIN32)
set(boost "C:\\Users\\Abstergo\\Downloads\\boost_1_79_0")
add_definitions(-D _WIN32_WINNT=0x0501)
else()
set(boost "/usr/local/include")
endif()



set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/runtime)
include_directories(BEFORE ${boost} ${PROJECT_SOURCE_DIR}/include)
link_directories( ${PROJECT_SOURCE_DIR}/lib)
file(GLOB_RECURSE SOURCE "source/*.cpp" "source/*.c")
#add_subdirectory(${PROJECT_SOURCE_DIR}/clientLib)
add_executable(socket_client main.cpp ${SOURCE})
target_link_libraries(socket_client PUBLIC progress)