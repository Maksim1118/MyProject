enable_testing()

add_executable(
  tests
  testLerp.cpp
 "testRtree.cpp"
  ../Server/BaseOld.cpp
  ../Server/ObjectsServer.cpp
  ../Server/UniqueIdGenerator.cpp
  ../Server/RTree.cpp  
)
target_include_directories(tests PRIVATE "${CMAKE_SOURCE_DIR}/Server")

target_link_libraries(
  tests
  GTest::gtest_main
  BaseLib
)

include(GoogleTest)
#gtest_discover_tests(tests)
