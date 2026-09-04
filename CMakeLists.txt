cmake_minimum_required(VERSION 3.10)
project(EalManTest)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_library(ealman STATIC 
"src/Chunk.cpp"
"src/EalMan.cpp"
)
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
enable_testing()
add_executable(run_tests
"tests/test.cpp"
)
target_include_directories(run_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(run_tests ealman GTest::GTest GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(run_tests)
set(TEST_DATA_SRC_DIR_GOOD "${CMAKE_CURRENT_SOURCE_DIR}/tests/Eal")
set(TEST_DATA_SRC_DIR_FAIL "${CMAKE_CURRENT_SOURCE_DIR}/tests/EalFail")
set(TEST_DATA_BIN_DIR "$<TARGET_FILE_DIR:run_tests>")
add_custom_command(
    TARGET run_tests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${TEST_DATA_SRC_DIR_GOOD}"
            "${TEST_DATA_BIN_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${TEST_DATA_SRC_DIR_FAIL}"
            "${TEST_DATA_BIN_DIR}"
)
