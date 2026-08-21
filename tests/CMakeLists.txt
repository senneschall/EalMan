add_executable(tests test.cpp)
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        main
)
FetchContent_MakeAvailable(googletest)

target_include_directories(tests PUBLIC ${CMAKE_SOURCE_DIR})
target_link_libraries(tests PRIVATE GTest::gtest_main ${PROJECT_NAME})
target_sources(tests PRIVATE
test.cpp
)
include(GoogleTest)
gtest_discover_tests(tests)
