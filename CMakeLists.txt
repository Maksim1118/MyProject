cmake_minimum_required(VERSION 3.16)

project(sfml_net VERSION 0.1.1.0)

add_subdirectory(D:/BaseFunctionLib ${CMAKE_BINARY_DIR}/BaseFunctionLib)
#set(LIB_DIR "D:/BaseFunctionLib")
#add_subdirectory(${LIB_DIR} BaseFunctionLib)
add_subdirectory(Desktop)
add_subdirectory(Server)
