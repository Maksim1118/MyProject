cmake_minimum_required(VERSION 3.16)
project(Server LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

#option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

#include(FetchContent)
#FetchContent_Declare(SFML
#  GIT_REPOSITORY https://github.com/SFML/SFML.git
#   GIT_TAG 2.6.x)
#FetchContent_MakeAvailable(SFML)

add_executable(Server
main.cpp
BaseOld.cpp
BotServer.cpp
FeedServer.cpp
FoodServer.cpp
GameEngine.cpp
HeroServer.cpp
ObjectsServer.cpp
PieceServer.cpp
ThornServer.cpp
MessageServer.cpp
 "MoveObjectServer.cpp")
target_link_libraries(Server PRIVATE BaseFunctionLib)
target_compile_features(Server PRIVATE cxx_std_17)
target_include_directories(Server  PUBLIC ..\\jsonLib\\include)

install(TARGETS Server)

#add_custom_command(
#    TARGET Server
#    COMMENT "Copy SFML DLLs"
#    PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_BIN_DIR}/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/sfml-system-d.dll $<TARGET_FILE_DIR:Server>
#    VERBATIM)
#
#add_custom_command(
#    TARGET Server
#    COMMENT "Copy SFML DLLs"
#    PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_BIN_DIR}/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/sfml-network-d.dll $<TARGET_FILE_DIR:Server>
#    VERBATIM)

if(WIN32)
    add_custom_command(
        TARGET Server
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:Server>
        VERBATIM)
endif()

#if(WIN32)
#   add_custom_command(
#       TARGET Server
#       COMMENT "Copy OpenAL DLL"
#       PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy "C:/smfl/SFML-2.6.1/bin/*.dll" $<TARGET_FILE_DIR:Server>
#       VERBATIM)
#endif()
