cmake_minimum_required(VERSION 3.16)
project(Desktop LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(RESOURCE_SOURCE_DIR "D:/HungryBall/Resources")
set(RESOURCE_DEST_DIR "${CMAKE_BINARY_DIR}/bin/Resources")

#option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

#include(FetchContent)
#FetchContent_Declare(SFML
#   GIT_REPOSITORY https://github.com/SFML/SFML.git
#  GIT_TAG 2.6.x)
#FetchContent_MakeAvailable(SFML)

add_executable(Desktop
Button.cpp
Food.cpp
GameScene.cpp
Hero.cpp
lesson15.cpp
MainScene.cpp
Objects.cpp
Options.cpp
Scene.cpp
SceneManager.cpp
SettingScene.cpp
Thorn.cpp
ResourceManager.cpp
Message.cpp
Bot.cpp "Piece.h" "Piece.cpp" "Feed.cpp" "Feed.h" "MoveObject.h" "MoveObject.cpp" "JsonParser.cpp")

target_link_libraries(Desktop PRIVATE BaseFunctionLib)
target_compile_features(Desktop PRIVATE cxx_std_17)
target_include_directories(Desktop  PUBLIC ..\\jsonLib\\include)

install(TARGETS Desktop)

#add_custom_command(
#    TARGET Desktop
#    COMMENT "Copy SFML DLLs"
#    PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_BIN_DIR}/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/sfml-graphics-d.dll $<TARGET_FILE_DIR:Desktop>
#    VERBATIM)
#
#
#add_custom_command(
#    TARGET Desktop
#    COMMENT "Copy SFML DLLs"
#    PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_BIN_DIR}/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/sfml-window-d.dll $<TARGET_FILE_DIR:Desktop>
#    VERBATIM)
#
#add_custom_command(
#    TARGET Desktop
#    COMMENT "Copy SFML DLLs"
#    PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_BIN_DIR}/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/sfml-network-d.dll $<TARGET_FILE_DIR:Desktop>
#    VERBATIM)
#
#add_custom_command(
#    TARGET Desktop
#    COMMENT "Copy SFML DLLs"
#    PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_BIN_DIR}/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/sfml-audio-d.dll $<TARGET_FILE_DIR:Desktop>
#    VERBATIM)

if(WIN32)
    add_custom_command(
        TARGET Desktop
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:Desktop>
        VERBATIM)
endif()


#if(WIN32)
#file(GLOB MY_PUBLIC_DLL
#  "C:/smfl/SFML-2.6.1/bin/*.dll"
#)
#file(COPY ${MY_PUBLIC_DLL} DESTINATION /)
#   add_custom_command(
#       TARGET Desktop
#       COMMENT "Copy OpenAL DLL"
#       PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy  $<TARGET_FILE_DIR:Desktop>
#       VERBATIM)
#endif()

add_custom_command(TARGET Desktop POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${RESOURCE_SOURCE_DIR}
    ${RESOURCE_DEST_DIR}
)