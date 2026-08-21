cmake_minimum_required(VERSION 3.10)
project(EalMan)
set(CMAKE_CXX_STANDARD          17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_library(${PROJECT_NAME} STATIC
"${CMAKE_CURRENT_SOURCE_DIR}/src/EalMan.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/src/Chunk.cpp"
)
target_include_directories(${PROJECT_NAME} PUBLIC 
"${CMAKE_CURRENT_SOURCE_DIR}/src"
"${CMAKE_CURRENT_SOURCE_DIR}"
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
add_subdirectory(tests)
enable_testing()
