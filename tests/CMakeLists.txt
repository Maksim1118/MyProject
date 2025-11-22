enable_testing()

add_executable(
  tests
  testLerp.cpp
)
target_link_libraries(
  tests
  GTest::gtest_main
  BaseLib
)

include(GoogleTest)
#gtest_discover_tests(tests)
